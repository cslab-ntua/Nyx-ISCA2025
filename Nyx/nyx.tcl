open_hw_manager
connect_hw_server -url localhost:3121
current_hw_target
open_hw_target
set_property PROBES.FILE {./nyx.ltx} [get_hw_devices xcu250_0]
set_property FULL_PROBES.FILE {./nyx.ltx} [get_hw_devices xcu250_0]
set_property PROGRAM.FILE {./nyx.bit} [get_hw_devices xcu250_0]
program_hw_devices [get_hw_devices xcu250_0]
