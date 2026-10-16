#=====================================================================
# out/paper-route-review-20261004/recovery-v1/recover-route.tcl
#
# Non-project Vivado batch recovery of the interrupted
# out/paper-route-review-20261004 build.  Nothing here touches the
# original run directory, the original project, or the original state file.
#
#   ariane_xilinx_physopt.dcp
#     -> route_design -directive Explore   (timing-driven; NO constraint edit)
#     -> recovery-v1/ariane_xilinx_routed.dcp
#     -> recovery-v1/reports/{timing_summary_routed, timing_worst_setup,
#                             timing_worst_hold, route_status,
#                             utilization_routed_hier, utilization_routed,
#                             drc_routed, check_timing_routed}.rpt
#     -> gates (fail-closed): WNS>=0, WHS>=0, setup+hold paths exist,
#        no unconstrained pins/paths, routing complete, DRC errors=0,
#        DRC critical warnings=0
#     -> only if ALL gates pass: write_bitstream + write_cfgmem (genesys2 SPIx4)
#
# No project file is created or opened.  No GUI.  No hardware connection and
# no device programming.  Exit 0 only on full success, otherwise exit 1.
# Everything is wrapped in catch, so no interactive prompt can appear.
#=====================================================================

namespace eval ::rec {}
set ::rec::NL [format %c 10]

set ::PROJ_ROOT /home/xvhaocheng/work/iommu-platform
set ::OUT_DIR   $::PROJ_ROOT/out/paper-route-review-20261004/recovery-v1
set ::LOG_DIR   $::OUT_DIR/logs
set ::RPT_DIR   $::OUT_DIR/reports

# Candidate input checkpoints (first existing, non-empty one wins).
# The overlay path is tried first, then the plain profile path.
set ::IMPL1_OVERLAY   $::PROJ_ROOT/out/paper-route-review-20261004/fpga-src/cva6/corev_apu/fpga/ariane.runs/impl_1
set ::IMPL1_MIGRATION $::PROJ_ROOT/out/migration-4dma-composition/fpga-src/cva6/corev_apu/fpga/ariane.runs/impl_1
set ::DCP_CANDIDATES [list $::IMPL1_OVERLAY/ariane_xilinx_physopt.dcp $::IMPL1_MIGRATION/ariane_xilinx_physopt.dcp]
if {[info exists ::env(RECOVERY_PHYSOPT_DCP)] && $::env(RECOVERY_PHYSOPT_DCP) ne {}} {
  set ::DCP_CANDIDATES [linsert $::DCP_CANDIDATES 0 $::env(RECOVERY_PHYSOPT_DCP)]
}

set ::ROUTED_DCP  $::OUT_DIR/ariane_xilinx_routed.dcp
set ::BIT_FILE    $::OUT_DIR/ariane_xilinx.bit
set ::MCS_FILE    $::OUT_DIR/ariane_xilinx.mcs
set ::RESULT_JSON $::OUT_DIR/recovery-tcl-result.json

set ::MAX_THREADS 8
set ::ROUTE_DIRECTIVE Explore
set ::NET_ROUTE_STATUS_CROSSCHECK 1
set ::DRC_API_CROSSCHECK 1

# genesys2 flash programming follows the supplied script
# (out/migration-4dma-composition/.../scripts/write_cfgmem.tcl, genesys2 branch:
# -format mcs -interface SPIx4 -size 256).  Change this single value to 128
# only if the board really carries a 16 MB QSPI device.
set ::CFGMEM_SIZE 256
set ::CFGMEM_INTERFACE SPIx4

#---------------------------------------------------------------------
# helpers
#---------------------------------------------------------------------
proc ::rec::mkdirs {} {
  file mkdir $::OUT_DIR
  file mkdir $::LOG_DIR
  file mkdir $::RPT_DIR
}

proc ::rec::now {} {
  return [clock format [clock seconds] -format {%Y-%m-%dT%H:%M:%SZ} -gmt true]
}

proc ::rec::log {msg} {
  set line [format {%s %s} [::rec::now] $msg]
  puts $line
  flush stdout
  if {[catch {
    set fh [open $::OUT_DIR/recovery-tcl.log a]
    puts $fh $line
    close $fh
  } e]} {
    puts [format {WARN: cannot append to recovery-tcl.log: %s} $e]
  }
}

proc ::rec::init_escape {} {
  set bs [format %c 92]
  set dq [format %c 34]
  set nl [format %c 10]
  set cr [format %c 13]
  set tb [format %c 9]
  set ::rec::esc_map [list $bs [format %s%s $bs $bs] $dq [format %s%s $bs $dq] $nl [format %s%s $bs n] $cr [format %s%s $bs r] $tb [format %s%s $bs t]]
}

proc ::rec::json_escape {s} {
  if {![info exists ::rec::esc_map]} { ::rec::init_escape }
  return [string map $::rec::esc_map $s]
}

proc ::rec::write_json {path d} {
  set ob [format %c 123]
  set cb [format %c 125]
  set dq [format %c 34]
  set fh [open $path w]
  puts $fh $ob
  set keys [lsort [dict keys $d]]
  set n [llength $keys]
  set i 0
  foreach k $keys {
    incr i
    set v [dict get $d $k]
    if {[string is integer -strict $v] || [string is double -strict $v]} {
      set jv $v
    } else {
      set jv [format %s%s%s $dq [::rec::json_escape $v] $dq]
    }
    set comma {}
    if {$i < $n} { set comma , }
    puts $fh [format {  %s%s%s: %s%s} $dq [::rec::json_escape $k] $dq $jv $comma]
  }
  puts $fh $cb
  close $fh
}

proc ::rec::slurp {path} {
  set d {}
  if {![file exists $path]} { return {} }
  if {[catch {
    set fh [open $path r]
    set d [read $fh]
    close $fh
  } e]} {
    ::rec::log [format {WARN: cannot read %s: %s} $path $e]
    set d {}
  }
  return $d
}

proc ::rec::last_int {line} {
  set nums [regexp -all -inline {[0-9]+} $line]
  if {[llength $nums] == 0} { return {} }
  return [lindex $nums end]
}

proc ::rec::try_report {label script} {
  set rc 0
  if {[catch {uplevel 1 $script} e]} {
    ::rec::log [format {WARN: %s failed: %s} $label $e]
    set rc 1
  } else {
    ::rec::log [format {OK: %s} $label]
  }
  return $rc
}

#---------------------------------------------------------------------
# report parsers (all text based; a parse miss is treated as UNKNOWN -> FAIL)
#---------------------------------------------------------------------
# Returns {wns tns whs ths} from the Design Timing Summary table, or {}.
# Assumption: that table is the FIRST place where a header line carries both
# WNS(ns) and WHS(ns) (true for report_timing_summary ordering: Design Timing
# Summary precedes Clock Summary / Intra Clock / Inter Clock / per-path-group
# tables).  Column order assumed: WNS TNS TNS-FEP TNS-EP WHS THS ...
proc ::rec::parse_design_timing_summary {text} {
  set nl $::rec::NL
  set lines [split $text $nl]
  set n [llength $lines]
  set hdr -1
  for {set i 0} {$i < $n} {incr i} {
    set l [lindex $lines $i]
    if {[string first {WNS(ns)} $l] >= 0 && [string first {WHS(ns)} $l] >= 0} {
      set hdr $i
      break
    }
  }
  if {$hdr < 0} { return {} }
  for {set j [expr {$hdr + 1}]} {$j < $n} {incr j} {
    set l [string trim [lindex $lines $j]]
    if {$l eq {}} { continue }
    if {[regexp {^-[[:space:]-]*$} $l]} { continue }
    if {![regexp {^-?[0-9]+[.]?[0-9]*} $l]} { continue }
    set toks [regexp -all -inline {[-+]?[0-9]+[.]?[0-9]*} $l]
    if {[llength $toks] >= 6} {
      return [list [lindex $toks 0] [lindex $toks 1] [lindex $toks 4] [lindex $toks 5]]
    }
    break
  }
  return {}
}

# Returns {status sections total} for the unconstrained checks of check_timing.
# status = zero | nonzero | unknown
proc ::rec::parse_unconstrained_check_timing {text} {
  set nl $::rec::NL
  set cur {}
  set found 0
  set total 0
  set nonzero 0
  foreach l [split $text $nl] {
    if {[regexp -nocase {checking[[:space:]]+([^[:space:]]+)} $l -> chk]} {
      set cur $chk
      continue
    }
    if {$cur eq {}} { continue }
    if {[string first unconstrained [string tolower $cur]] >= 0} {
      if {[regexp -nocase {there are[[:space:]]+([0-9]+)} $l -> m]} {
        incr found
        incr total $m
        if {$m > 0} { set nonzero 1 }
      }
    }
  }
  if {!$found} { return [list unknown 0 0] }
  if {$nonzero} { return [list nonzero $found $total] }
  return [list zero $found $total]
}

# Returns {bad matched evidence} from report_route_status text.
# Any summary line whose text mentions error/unrouted/partial/no route/
# not fully routed/incomplete and whose trailing integer is > 0 counts as bad.
# Detail tables (if any) would count as bad too, which can only cause a false
# FAIL, never a false PASS.
proc ::rec::parse_route_status_bad {text} {
  set nl $::rec::NL
  set bad 0
  set matched 0
  set ev {}
  foreach l [split $text $nl] {
    set t [string trim $l]
    if {$t eq {}} { continue }
    set ll [string tolower $t]
    if {![regexp {error|unrout|partial|no rout|not fully|incomplete} $ll]} { continue }
    set m [::rec::last_int $t]
    if {$m eq {}} { continue }
    incr matched
    if {[llength $ev] < 12} { lappend ev $t }
    if {$m > 0} { incr bad }
  }
  return [list $bad $matched $ev]
}

# Returns {errors crit warnings infos matched evidence} from report_drc text.
# Pipe-formatted violation/table lines are skipped; the summary footer is used.
proc ::rec::parse_drc {text} {
  set nl $::rec::NL
  set errors {}
  set crit {}
  set warn {}
  set infos {}
  set matched 0
  set ev {}
  foreach l [split $text $nl] {
    set t [string trim $l]
    if {$t eq {}} { continue }
    if {[string length $t] > 120} { continue }
    if {[string first | $t] >= 0} { continue }
    set ll [string tolower $t]
    if {[regexp {severity|checking|description|rule name} $ll]} { continue }
    set m [::rec::last_int $t]
    if {$m eq {}} { continue }
    if {[regexp {critical[[:space:]]+warning} $ll]} {
      set crit $m
      incr matched
      if {[llength $ev] < 12} { lappend ev $t }
      continue
    }
    if {[regexp {error} $ll]} {
      set errors $m
      incr matched
      if {[llength $ev] < 12} { lappend ev $t }
      continue
    }
    if {[regexp {warning} $ll]} {
      set warn $m
      incr matched
      if {[llength $ev] < 12} { lappend ev $t }
      continue
    }
    if {[regexp {info} $ll]} {
      set infos $m
      incr matched
      if {[llength $ev] < 12} { lappend ev $t }
      continue
    }
  }
  return [list $errors $crit $warn $infos $matched $ev]
}

# Evidence only (never a gate).  Returns {lut lut_avail lut_pct ff bram dsp}.
proc ::rec::parse_utilization {text} {
  set nl $::rec::NL
  set lut {}
  set lut_avail {}
  set lut_pct {}
  set ff {}
  set bram {}
  set dsp {}
  foreach l [split $text $nl] {
    if {[string first {LUTs} $l] >= 0 && ([string first {Slice LUTs} $l] >= 0 || [string first {CLB LUTs} $l] >= 0)} {
      set p [string first {LUTs} $l]
      set nums [regexp -all -inline {[0-9]+} [string range $l $p end]]
      if {[llength $nums] >= 4} {
        set lut [lindex $nums 0]
        set lut_avail [lindex $nums 3]
        if {[llength $nums] >= 5} { set lut_pct [lindex $nums 4] }
      }
      continue
    }
    if {[string first {Slice Registers} $l] >= 0} {
      set nums [regexp -all -inline {[0-9]+} [string range $l [string first {Slice Registers} $l] end]]
      if {[llength $nums] >= 1} { set ff [lindex $nums 0] }
      continue
    }
    if {[string first {Block RAM Tile} $l] >= 0} {
      set nums [regexp -all -inline {[0-9]+} [string range $l [string first {Block RAM Tile} $l] end]]
      if {[llength $nums] >= 1} { set bram [lindex $nums 0] }
      continue
    }
    if {[string first {DSPs} $l] >= 0} {
      set nums [regexp -all -inline {[0-9]+} [string range $l [string first {DSPs} $l] end]]
      if {[llength $nums] >= 1} { set dsp [lindex $nums 0] }
    }
  }
  return [list $lut $lut_avail $lut_pct $ff $bram $dsp]
}

#---------------------------------------------------------------------
# main body
#---------------------------------------------------------------------
proc ::rec::run {} {
  set res [dict create]

  # ---- 1. choose source checkpoint -------------------------------------
  set dcp {}
  foreach c $::DCP_CANDIDATES {
    if {[file exists $c] && [file size $c] > 0} { set dcp $c ; break }
  }
  if {$dcp eq {}} {
    error [format {no usable physopt checkpoint found; candidates: %s} $::DCP_CANDIDATES]
  }
  dict set res physopt_dcp $dcp
  dict set res physopt_dcp_bytes [file size $dcp]
  dict set res vivado_version [version -short]
  dict set res max_threads $::MAX_THREADS
  dict set res route_directive $::ROUTE_DIRECTIVE
  dict set res cfgmem_size_mbit $::CFGMEM_SIZE
  dict set res cfgmem_interface $::CFGMEM_INTERFACE
  ::rec::log [format {INFO: vivado %s} [version -short]]
  ::rec::log [format {INFO: source checkpoint = %s (%d bytes)} $dcp [file size $dcp]]
  ::rec::log [format {INFO: set_param general.maxThreads %d ; route_design -directive %s} $::MAX_THREADS $::ROUTE_DIRECTIVE]
  ::rec::log {INFO: no timing constraint is added, removed or modified}

  set_param general.maxThreads $::MAX_THREADS

  # ---- 2. open checkpoint (non-project) --------------------------------
  ::rec::log {STEP open_checkpoint}
  open_checkpoint $dcp
  set des {}
  set part {}
  catch {set des [get_property NAME [current_design]]}
  catch {set part [get_property PART [current_design]]}
  dict set res design_name $des
  dict set res part $part
  ::rec::log [format {INFO: current_design = %s ; part = %s} $des $part]

  # ---- 3. timing-driven route ------------------------------------------
  ::rec::log {STEP route_design -directive Explore}
  set rc_route 0
  set msg_route {}
  if {[catch {route_design -directive $::ROUTE_DIRECTIVE} msg_route]} { set rc_route 1 }
  dict set res route_rc $rc_route
  dict set res route_message $msg_route
  ::rec::log [format {INFO: route_design catch-rc = %d ; message = %s} $rc_route $msg_route]

  # ---- 4. save routed checkpoint (kept even if later gates fail) --------
  ::rec::log {STEP write_checkpoint (routed)}
  set rc_dcp 0
  set msg_dcp {}
  if {[catch {write_checkpoint -force $::ROUTED_DCP} msg_dcp]} { set rc_dcp 1 }
  dict set res routed_dcp_rc $rc_dcp
  if {[file exists $::ROUTED_DCP]} {
    ::rec::log [format {INFO: routed checkpoint = %s (%d bytes)} $::ROUTED_DCP [file size $::ROUTED_DCP]]
  } else {
    ::rec::log [format {ERROR: routed checkpoint not written: %s} $msg_dcp]
  }

  # ---- 5. reports -------------------------------------------------------
  ::rec::log {STEP reports}
  ::rec::try_report {report_timing_summary} {report_timing_summary -max_paths 10 -report_unconstrained -file [file join $::RPT_DIR timing_summary_routed.rpt]}
  ::rec::try_report {report_timing setup} {report_timing -delay_type max -max_paths 10 -nworst 10 -sort_by slack -file [file join $::RPT_DIR timing_worst_setup.rpt]}
  ::rec::try_report {report_timing hold} {report_timing -delay_type min -max_paths 10 -nworst 10 -sort_by slack -file [file join $::RPT_DIR timing_worst_hold.rpt]}
  ::rec::try_report {report_route_status} {report_route_status -file [file join $::RPT_DIR route_status.rpt]}
  ::rec::try_report {report_utilization hierarchical} {report_utilization -hierarchical -file [file join $::RPT_DIR utilization_routed_hier.rpt]}
  ::rec::try_report {report_utilization flat} {report_utilization -file [file join $::RPT_DIR utilization_routed.rpt]}
  ::rec::try_report {report_drc} {report_drc -file [file join $::RPT_DIR drc_routed.rpt]}
  ::rec::try_report {check_timing} {check_timing -verbose -file [file join $::RPT_DIR check_timing_routed.rpt]}

  # ---- 6. gather gate inputs -------------------------------------------
  set tvals [::rec::parse_design_timing_summary [::rec::slurp [file join $::RPT_DIR timing_summary_routed.rpt]]]
  set wns {}
  set tns {}
  set whs {}
  set ths {}
  if {[llength $tvals] >= 4} { lassign $tvals wns tns whs ths }
  dict set res wns_ns $wns
  dict set res tns_ns $tns
  dict set res whs_ns $whs
  dict set res ths_ns $ths
  dict set res timing_summary_parsed [expr {[llength $tvals] >= 4 ? 1 : 0}]

  set n_setup -1
  set n_hold -1
  set path_wns {}
  set path_whs {}
  if {[catch {
    set tp [get_timing_paths -quiet -delay_type max -max_paths 1 -nworst 1]
    set n_setup [llength $tp]
    if {$n_setup > 0} { set path_wns [get_property SLACK [lindex $tp 0]] }
  } e]} {
    ::rec::log [format {WARN: setup path probe failed: %s} $e]
  }
  if {[catch {
    set tp [get_timing_paths -quiet -delay_type min -max_paths 1 -nworst 1]
    set n_hold [llength $tp]
    if {$n_hold > 0} { set path_whs [get_property SLACK [lindex $tp 0]] }
  } e]} {
    ::rec::log [format {WARN: hold path probe failed: %s} $e]
  }
  dict set res setup_paths_found $n_setup
  dict set res hold_paths_found $n_hold
  dict set res setup_worst_path_slack_ns $path_wns
  dict set res hold_worst_path_slack_ns $path_whs

  lassign [::rec::parse_unconstrained_check_timing [::rec::slurp [file join $::RPT_DIR check_timing_routed.rpt]]] unc_status unc_sections unc_total
  dict set res unconstrained_status $unc_status
  dict set res unconstrained_checks_parsed $unc_sections
  dict set res unconstrained_pin_total $unc_total

  lassign [::rec::parse_route_status_bad [::rec::slurp [file join $::RPT_DIR route_status.rpt]]] rs_bad rs_matched rs_ev
  dict set res route_status_matched_lines $rs_matched
  dict set res route_status_bad_lines $rs_bad
  dict set res route_status_evidence [join $rs_ev { || }]

  set net_unrouted -1
  set net_note {skipped}
  if {$::NET_ROUTE_STATUS_CROSSCHECK} {
    if {[catch {
      set ns [get_nets -quiet -hier -filter {ROUTE_STATUS == UNROUTED || ROUTE_STATUS == PARTIAL}]
      set net_unrouted [llength $ns]
      set net_note ok
    } e]} {
      set net_unrouted -1
      set net_note [format {unavailable: %s} $e]
      ::rec::log [format {WARN: net ROUTE_STATUS cross-check unavailable: %s} $e]
    }
  }
  dict set res net_route_status_note $net_note
  dict set res net_unrouted_or_partial $net_unrouted

  lassign [::rec::parse_drc [::rec::slurp [file join $::RPT_DIR drc_routed.rpt]]] drc_err drc_crit drc_warn drc_info drc_matched drc_ev
  dict set res drc_errors $drc_err
  dict set res drc_critical_warnings $drc_crit
  dict set res drc_warnings $drc_warn
  dict set res drc_infos $drc_info
  dict set res drc_summary_lines_matched $drc_matched
  dict set res drc_evidence [join $drc_ev { || }]

  set drc_api_err -1
  set drc_api_crit -1
  if {$::DRC_API_CROSSCHECK} {
    if {[catch {
      set viols [get_drc_violations -quiet]
      set drc_api_err 0
      set drc_api_crit 0
      foreach v $viols {
        set sev [get_property SEVERITY $v]
        if {$sev eq {Error}} { incr drc_api_err }
        if {$sev eq {Critical Warning}} { incr drc_api_crit }
      }
    } e]} {
      set drc_api_err -1
      set drc_api_crit -1
      ::rec::log [format {INFO: get_drc_violations cross-check unavailable: %s} $e]
    }
  }
  dict set res drc_api_errors $drc_api_err
  dict set res drc_api_critical_warnings $drc_api_crit

  lassign [::rec::parse_utilization [::rec::slurp [file join $::RPT_DIR utilization_routed.rpt]]] u_lut u_lut_avail u_lut_pct u_ff u_bram u_dsp
  dict set res util_lut_used $u_lut
  dict set res util_lut_available $u_lut_avail
  dict set res util_lut_pct $u_lut_pct
  dict set res util_ff_used $u_ff
  dict set res util_bram_used $u_bram
  dict set res util_dsp_used $u_dsp

  # ---- 7. gates ---------------------------------------------------------
  set fails [list]

  if {$rc_route != 0} {
    lappend fails [format {route_design returned error: %s} $msg_route]
  }
  if {![file exists $::ROUTED_DCP] || [file size $::ROUTED_DCP] <= 0} {
    lappend fails [format {routed checkpoint missing or empty: %s} $::ROUTED_DCP]
  }

  if {[llength $tvals] < 4} {
    lappend fails {WNS/WHS could not be parsed from timing_summary_routed.rpt (unknown) - refusing to guess}
  } else {
    if {![string is double -strict $wns]} {
      lappend fails [format {WNS is not numeric: %s} $wns]
    } elseif {$wns < 0.0} {
      lappend fails [format {WNS = %s ns < 0 (setup violation)} $wns]
    }
    if {![string is double -strict $whs]} {
      lappend fails [format {WHS is not numeric: %s} $whs]
    } elseif {$whs < 0.0} {
      lappend fails [format {WHS = %s ns < 0 (hold violation)} $whs]
    }
    if {[string is double -strict $wns] && [string is double -strict $path_wns]} {
      if {$wns >= 0.0 && $path_wns < 0.0} {
        lappend fails [format {WNS cross-check conflict: timing summary %s ns vs worst setup path %s ns} $wns $path_wns]
      }
    }
    if {[string is double -strict $whs] && [string is double -strict $path_whs]} {
      if {$whs >= 0.0 && $path_whs < 0.0} {
        lappend fails [format {WHS cross-check conflict: timing summary %s ns vs worst hold path %s ns} $whs $path_whs]
      }
    }
  }

  if {$n_setup <= 0} { lappend fails [format {no setup timing path reported (count %d) - timing not analyzable} $n_setup] }
  if {$n_hold <= 0}  { lappend fails [format {no hold timing path reported (count %d) - timing not analyzable} $n_hold] }

  if {$unc_status eq {unknown}} {
    lappend fails {unconstrained status unknown (check_timing summary could not be parsed)}
  } elseif {$unc_status eq {nonzero}} {
    lappend fails [format {unconstrained pins/paths reported (total %d over %d checks)} $unc_total $unc_sections]
  }

  if {$rs_matched == 0} {
    lappend fails {route completeness unknown (report_route_status could not be parsed)}
  } elseif {$rs_bad > 0} {
    lappend fails [format {routing incomplete: %d bad summary line(s) in report_route_status} $rs_bad]
  }
  if {$net_unrouted > 0} {
    lappend fails [format {net ROUTE_STATUS cross-check: %d net(s) UNROUTED/PARTIAL} $net_unrouted]
  }
  if {$drc_api_err > 0} {
    lappend fails [format {DRC API cross-check: %d Error violation(s)} $drc_api_err]
  }

  if {$drc_matched == 0} {
    lappend fails {DRC summary could not be parsed (unknown) - see reports/drc_routed.rpt}
  } else {
    if {![string is integer -strict $drc_err]} {
      lappend fails {DRC error count not found in report (unknown)}
    } elseif {$drc_err > 0} {
      lappend fails [format {DRC errors = %d} $drc_err]
    }
    if {![string is integer -strict $drc_crit]} {
      lappend fails {DRC critical-warning count not found in report (unknown)}
    } elseif {$drc_crit > 0} {
      lappend fails [format {DRC critical warnings = %d} $drc_crit]
    }
  }

  # ---- 8. artifacts, only on a fully clean result -----------------------
  if {[llength $fails] == 0} {
    ::rec::log {RECOVERY_GATES_PASSED - writing bitstream}
    set rc_bit 0
    if {[catch {write_bitstream -force $::BIT_FILE} e]} {
      set rc_bit 1
      lappend fails [format {write_bitstream failed: %s} $e]
    }
    dict set res bitstream_rc $rc_bit
    if {!$rc_bit && (![file exists $::BIT_FILE] || [file size $::BIT_FILE] <= 0)} {
      lappend fails [format {bitstream missing or empty: %s} $::BIT_FILE]
    }
    if {[llength $fails] == 0} {
      set rc_mcs 0
      if {[catch {
        write_cfgmem -format mcs -interface $::CFGMEM_INTERFACE -size $::CFGMEM_SIZE -loadbit [format {up 0x0 %s} $::BIT_FILE] -file $::MCS_FILE -force
      } e]} {
        set rc_mcs 1
        lappend fails [format {write_cfgmem failed: %s} $e]
      }
      dict set res cfgmem_rc $rc_mcs
      if {!$rc_mcs && (![file exists $::MCS_FILE] || [file size $::MCS_FILE] <= 0)} {
        lappend fails [format {mcs missing or empty: %s} $::MCS_FILE]
      }
    }
  } else {
    ::rec::log [format {RECOVERY_GATES_FAILED: %s} $fails]
  }

  dict set res bit_exists [expr {[file exists $::BIT_FILE] ? 1 : 0}]
  dict set res bit_bytes [expr {[file exists $::BIT_FILE] ? [file size $::BIT_FILE] : 0}]
  dict set res mcs_exists [expr {[file exists $::MCS_FILE] ? 1 : 0}]
  dict set res mcs_bytes [expr {[file exists $::MCS_FILE] ? [file size $::MCS_FILE] : 0}]
  dict set res routed_dcp_bytes [expr {[file exists $::ROUTED_DCP] ? [file size $::ROUTED_DCP] : 0}]
  dict set res failure_count [llength $fails]
  dict set res failures [join $fails { ; }]
  dict set res gates_passed [expr {[llength $fails] == 0 ? 1 : 0}]

  foreach k [lsort [dict keys $res]] {
    ::rec::log [format {RESULT %s = %s} $k [dict get $res $k]]
  }
  return $res
}

#---------------------------------------------------------------------
# driver: always terminates with an exit code, never drops into the
# interactive shell, and always leaves a machine readable result file.
#---------------------------------------------------------------------
::rec::mkdirs
::rec::log [format {===== recovery run start (pid %d, vivado %s) =====} [pid] [version -short]]

set res [dict create gates_passed 0 failures {}]
set fatal {}
if {[catch {set res [::rec::run]} fatal]} {
  ::rec::log [format {FATAL: %s} $fatal]
  set res [dict create gates_passed 0 failures [format {fatal: %s} $fatal]]
}

dict set res finished_at [::rec::now]
dict set res result_file $::RESULT_JSON
dict set res exit_code [expr {[dict get $res gates_passed] ? 0 : 1}]

if {[catch {::rec::write_json $::RESULT_JSON $res} e]} {
  puts [format {WARN: could not write %s : %s} $::RESULT_JSON $e]
}
::rec::log [format {===== recovery run end, exit_code = %d =====} [dict get $res exit_code]]
puts [format {RECOVERY_EXIT_CODE=%d} [dict get $res exit_code]]
exit [dict get $res exit_code]
