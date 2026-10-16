set_param board.repoPaths [list /home/xvhaocheng/work/iommu-platform/configs/boards]
set boards [get_board_parts -quiet digilentinc.com:genesys2:part0:1.1]
if {[llength $boards] != 1} { error "Expected the pinned Genesys 2 board definition" }
create_project -in_memory -part xc7k325tffg900-2
set_property board_part [lindex $boards 0] [current_project]
puts "PASS pinned board: [get_property board_part [current_project]]"
close_project
