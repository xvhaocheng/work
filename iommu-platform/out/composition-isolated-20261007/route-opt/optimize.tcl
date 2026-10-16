# Explore is supported post-route in AMD UG904 2023.1:
# https://docs.amd.com/r/2023.1-English/ug904-vivado-implementation/Directives-Used-by-phys_opt_design-and-route_design-in-Implementation-Strategies
# =============================================================================
# out/composition-trim-20261005/route-opt/optimize.tcl      (Vivado 2023.1)
#
# 目的：对现有已布线的 ariane_xilinx checkpoint 做一次 post-route 物理优化，
#       保持原 RTL / 50 MHz / 约束 / ABI / I-O 接口不变，仅当全部验收 gate
#       通过后才生成 bitstream 与 cfgmem。
#
# 保证：
#   * 不修改 RTL、时钟频率、XDC 约束；不新增 set_false_path / multicycle。
#   * 不修改 DRC severity；不隐藏失败。
#   * 不读取/加载任何之前未通过验收的 BIT。
#   * 最多一次 phys_opt_design(Explore) 与一次 route_design(Explore)，无重试循环。
#   * 任一 gate 失败 -> 保留报告、result.json 记录、exit 非 0。
# =============================================================================

# ---- 本次运行的实际路径（会写入 result.json / optimize.log）------------------
set out      /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/route-opt
set src_dcp  /home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_routed.dcp
set rpt      $out/reports
set dcp_out  $out/ariane_xilinx_routed_opt.dcp
set bit_out  $out/ariane_xilinx.bit
set mcs_out  $out/ariane_xilinx.mcs
set json_out $out/result.json
set log_out  $out/optimize.log

if {[file exists $json_out] || [file exists $dcp_out] || [file exists $bit_out] || [file exists $mcs_out]} {error {Existing optimization output; refusing duplicate run}}
file mkdir $rpt

# ---- 仅行级解析的小工具（非框架）--------------------------------------------

# 读取 "| Design Timing Summary" 之后的 12 列数值行：
#   WNS TNS TNS_fail TNS_total WHS THS THS_fail THS_total WPWS TPWS TPWS_fail TPWS_total
# (timing-evidence.txt:155-161)
proc read_timing_summary {rptfile} {
    if {![file exists $rptfile]} { error "report missing: $rptfile" }
    set fh    [open $rptfile r]
    set lines [split [read $fh] "\n"]
    close $fh
    set idx -1
    for {set i 0} {$i < [llength $lines]} {incr i} {
        if {[regexp {^\s*\|\s*Design Timing Summary} [lindex $lines $i]]} { set idx $i; break }
    }
    if {$idx < 0} { error "no '| Design Timing Summary' header in $rptfile" }
    for {set i $idx} {$i < [llength $lines]} {incr i} {
        set toks [regexp -all -inline {\S+} [lindex $lines $i]]
        if {[llength $toks] != 12} { continue }
        set numeric 1
        foreach t $toks {
            if {![regexp {^[-+]?[0-9]+(\.[0-9]+)?$} $t]} { set numeric 0; break }
        }
        if {!$numeric} { continue }
        return $toks
    }
    error "no 12-column numeric Design Timing Summary line in $rptfile"
}

# 从真实 check_timing 报告核验 loops；无法识别格式时**报错**，绝不当作 0。
# (格式依据 timing-evidence.txt:80 / 133 / 135)
proc read_loops {ctfile} {
    if {![file exists $ctfile]} { error "report missing: $ctfile" }
    set fh    [open $ctfile r]
    set lines [split [read $fh] "\n"]
    close $fh
    set hdr ""
    set sentence ""
    foreach line $lines {
        if {[regexp {checking loops \(([0-9]+)\)} $line -> n]} {
            if {$hdr eq ""} { set hdr $n }
        }
        if {[regexp {There are ([0-9]+) combinational loops} $line -> n]} { set sentence $n }
    }
    if {$hdr eq ""}      { error "cannot parse 'checking loops (N)' from $ctfile; refusing to assume 0" }
    if {$sentence eq ""} { error "cannot parse 'There are N combinational loops' from $ctfile; refusing to assume 0" }
    if {$hdr ne $sentence} { error "inconsistent loop counts in $ctfile: header=$hdr text=$sentence" }
    return $hdr
}

# 从 report_route_status 取 routing errors / fully routed / routable
# (格式依据 ariane_xilinx_route_status.rpt:1-12)
proc read_route_status {rsfile} {
    if {![file exists $rsfile]} { error "report missing: $rsfile" }
    set fh    [open $rsfile r]
    set lines [split [read $fh] "\n"]
    close $fh
    set errors ""; set fully ""; set routable ""
    foreach line $lines {
        if {[regexp {routing errors[^:]*:\s*([0-9]+)}        $line -> n]} { set errors   $n }
        if {[regexp {fully routed nets[^:]*:\s*([0-9]+)}     $line -> n]} { set fully    $n }
        if {[regexp {routable nets[^:]*:\s*([0-9]+)}         $line -> n]} { set routable $n }
    }
    if {$errors eq "" || $fully eq "" || $routable eq ""} {
        error "cannot parse route status from $rsfile"
    }
    return [dict create errors $errors fully $fully routable $routable]
}

# get_timing_paths 结果 -> 真实 slack；无路径/取属性失败 -> ""（调用方报错）
proc first_path_slack {paths} {
    if {[llength $paths] == 0} { return "" }
    set p [lindex $paths 0]
    if {[catch {get_property SLACK $p} s]} { return "" }
    return $s
}

# ---- 最小 JSON 写出工具 ------------------------------------------------------
proc jbool {v} { if {$v} { return true } ; return false }
proc jnum  {v} {
    if {$v eq ""} { return null }
    if {![regexp {^[+-]?[0-9]+([.][0-9]+)?([eE][+-]?[0-9]+)?$} $v]} { return null }
    return $v
}
proc jstr {s} {
    set s [string map [list "\\" "\\\\" "\"" "\\\"" "\n" "\\n" "\r" "\\r" "\t" "\\t"] $s]
    return "\"$s\""
}

# =============================================================================
# 主流程
# =============================================================================
set errmsg     ""
set wns        "";  set whs        ""
set wns_real   "";  set whs_real   ""
set loops      "";  set setup_fail ""; set hold_fail ""; set pulse_fail ""
set route_errors ""

set gates [dict create \
    loops_zero 0 setup_failing_zero 0 hold_failing_zero 0 pulse_failing_zero 0 \
    route_errors_zero 0 routing_complete 0 wns_nonneg 0 whs_nonneg 0 artifacts_written 0]

set rc [catch {

    set_param general.maxThreads 8

    # (1) 打开既有 routed checkpoint（输入路径已记录在上方变量中）
    if {![file exists $src_dcp] || [file size $src_dcp] <= 0} {
        error "source checkpoint missing or empty: $src_dcp"
    }
    open_checkpoint $src_dcp

    # (2) 单次 post-route 物理优化
    phys_opt_design -directive Explore

    # (3) 至多一次 re-route：仅当设计不再完全布线时执行
    report_route_status -file $rpt/route_status_post_physopt.rpt
    set post [read_route_status $rpt/route_status_post_physopt.rpt]
    if {[dict get $post errors] != 0 || [dict get $post fully] != [dict get $post routable]} {
        route_design -directive Explore
    }

    # (4) 写出优化后的 routed checkpoint
    write_checkpoint -force $dcp_out

    # (5) 新报告
    report_timing_summary -delay_type min_max -max_paths 10 -report_unconstrained \
        -file $rpt/timing_summary_routed.rpt
    report_route_status  -file $rpt/route_status.routed.rpt
    report_utilization   -file $rpt/utilization_routed.rpt
    report_drc           -file $rpt/drc_routed.rpt
    check_timing -verbose -file $rpt/check_timing_routed.rpt

    # (6) 解析 12 列 Design Timing Summary
    set ts         [read_timing_summary $rpt/timing_summary_routed.rpt]
    set wns        [lindex $ts 0]    ;# WNS(ns)
    set setup_fail [lindex $ts 2]    ;# TNS Failing Endpoints
    set whs        [lindex $ts 4]    ;# WHS(ns)
    set hold_fail  [lindex $ts 6]    ;# THS Failing Endpoints
    set pulse_fail [lindex $ts 10]   ;# TPWS Failing Endpoints

    # (7) loops 来自真实 check_timing 报告（不识别就报错）
    set loops [read_loops $rpt/check_timing_routed.rpt]

    # (8) routing errors
    set rs           [read_route_status $rpt/route_status.routed.rpt]
    set route_errors [dict get $rs errors]

    # (9) 用 get_timing_paths 复核真实 slack（必须为有限数值）
    set setup_paths [get_timing_paths -quiet -delay_type max -max_paths 1 -nworst 1]
    set hold_paths  [get_timing_paths -quiet -delay_type min -max_paths 1 -nworst 1]
    set wns_real [first_path_slack $setup_paths]
    set whs_real [first_path_slack $hold_paths]
    if {$wns_real eq ""} { error "get_timing_paths returned no setup path" }
    if {$whs_real eq ""} { error "get_timing_paths returned no hold path" }
    if {![regexp {^[+-]?[0-9]+([.][0-9]+)?([eE][+-]?[0-9]+)?$} $wns_real]} { error "non-numeric setup slack: '$wns_real'" }
    if {![regexp {^[+-]?[0-9]+([.][0-9]+)?([eE][+-]?[0-9]+)?$} $whs_real]} { error "non-numeric hold slack: '$whs_real'" }

    # (10) 验收 gate
    dict set gates loops_zero         [expr {$loops == 0}]
    dict set gates setup_failing_zero [expr {$setup_fail == 0}]
    dict set gates hold_failing_zero  [expr {$hold_fail == 0}]
    dict set gates pulse_failing_zero [expr {$pulse_fail == 0}]
    dict set gates route_errors_zero  [expr {$route_errors == 0}]
    dict set gates routing_complete   [expr {[dict get $rs fully] == [dict get $rs routable]}]
    dict set gates wns_nonneg         [expr {$wns_real >= 0.0}]
    dict set gates whs_nonneg         [expr {$whs_real >= 0.0}]

    foreach g {loops_zero setup_failing_zero hold_failing_zero pulse_failing_zero \
               route_errors_zero routing_complete wns_nonneg whs_nonneg} {
        if {![dict get $gates $g]} {
            error "acceptance gate FAILED: $g (loops=$loops setup_fail=$setup_fail hold_fail=$hold_fail pulse_fail=$pulse_fail route_errors=$route_errors wns_real=$wns_real whs_real=$whs_real report_wns=$wns report_whs=$whs)"
        }
    }

    # (11) 仅在全部 gate 通过后：bitstream(原生 DRC) + cfgmem(Genesys2 SPIx4 256)
    write_bitstream -force $bit_out
    write_cfgmem -format mcs -interface SPIx4 -size 256 \
        -loadbit "up 0x0 $bit_out" -file $mcs_out -force

    foreach f [list $bit_out $mcs_out] {
        if {![file exists $f] || [file size $f] <= 0} { error "missing/empty artifact: $f" }
    }
    dict set gates artifacts_written 1

} errmsg]
if {$rc == 0} { set errmsg "" }

# ---- 失败时尽力保留报告（各自独立 catch，不掩盖原始错误）-------------------
if {$rc != 0} {
    catch { report_route_status  -file $rpt/route_status_fail.rpt }
    catch { check_timing -verbose -file $rpt/check_timing_fail.rpt }
    catch { report_timing_summary -delay_type min_max -max_paths 10 -report_unconstrained \
                -file $rpt/timing_summary_fail.rpt }
}

# ---- result.json（记录 gates / wns / whs / rc 及本次实际路径）---------------
set gparts {}
foreach g {loops_zero setup_failing_zero hold_failing_zero pulse_failing_zero \
           route_errors_zero routing_complete wns_nonneg whs_nonneg artifacts_written} {
    lappend gparts "[jstr $g]: [jbool [dict get $gates $g]]"
}

set items {}
lappend items "  \"rc\": $rc"
lappend items "  \"input_dcp\": [jstr $src_dcp]"
lappend items "  \"output_dcp\": [jstr $dcp_out]"
lappend items "  \"bitstream\": [jstr $bit_out]"
lappend items "  \"cfgmem\": [jstr $mcs_out]"
lappend items "  \"reports_dir\": [jstr $rpt]"
lappend items "  \"wns_ns\": [jnum $wns_real]"
lappend items "  \"whs_ns\": [jnum $whs_real]"
lappend items "  \"report_wns_ns\": [jnum $wns]"
lappend items "  \"report_whs_ns\": [jnum $whs]"
lappend items "  \"loops\": [jnum $loops]"
lappend items "  \"setup_failing_endpoints\": [jnum $setup_fail]"
lappend items "  \"hold_failing_endpoints\": [jnum $hold_fail]"
lappend items "  \"pulse_failing_endpoints\": [jnum $pulse_fail]"
lappend items "  \"route_errors\": [jnum $route_errors]"
lappend items "  \"gates\": {[join $gparts ", "]}"
lappend items "  \"error\": [jstr $errmsg]"
set body [join $items ",\n"]

set fh [open $json_out w]
puts $fh "{"
puts $fh $body
puts $fh "}"
close $fh

# ---- 运行日志 ----------------------------------------------------------------
set lf [open $log_out w]
puts $lf "optimize.tcl run report"
puts $lf "input_dcp   = $src_dcp"
puts $lf "output_dir  = $out"
puts $lf "output_dcp  = $dcp_out"
puts $lf "bitstream   = $bit_out"
puts $lf "cfgmem      = $mcs_out"
puts $lf "rc          = $rc"
puts $lf "gates       = $gates"
puts $lf "wns_report  = $wns"
puts $lf "whs_report  = $whs"
puts $lf "wns_real    = $wns_real"
puts $lf "whs_real    = $whs_real"
puts $lf "loops       = $loops"
puts $lf "setup_failing_endpoints = $setup_fail"
puts $lf "hold_failing_endpoints  = $hold_fail"
puts $lf "pulse_failing_endpoints = $pulse_fail"
puts $lf "route_errors = $route_errors"
if {$rc != 0} { puts $lf "ERROR: $errmsg" }
close $lf

puts "optimize.tcl rc=$rc wns=$wns_real whs=$whs_real loops=$loops"
exit $rc
