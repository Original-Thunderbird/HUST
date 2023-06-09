# 
# Synthesis run script generated by Vivado
# 

debug::add_scope template.lib 1
set_msg_config -id {Common-41} -limit 4294967295
set_msg_config -id {HDL 9-1061} -limit 100000
set_msg_config -id {HDL 9-1654} -limit 100000
create_project -in_memory -part xc7a100tcsg324-1

set_param project.compositeFile.enableAutoGeneration 0
set_param synth.vivado.isSynthRun true
set_property webtalk.parent_dir /home/orithu/桌面/第三次/c_2/c_2.cache/wt [current_project]
set_property parent.project_path /home/orithu/桌面/第三次/c_2/c_2.xpr [current_project]
set_property default_lib xil_defaultlib [current_project]
set_property target_language Verilog [current_project]
set_property ip_repo_paths /home/orithu/桌面/第三次/C/C.cache/ip [current_project]
set_property ip_output_repo /home/orithu/桌面/第三次/C/C.cache/ip [current_project]
set_property ip_cache_permissions {read write} [current_project]
read_verilog -library xil_defaultlib /home/orithu/桌面/第三次/c_2/c_2.srcs/sources_1/new/c.v
read_xdc /home/orithu/桌面/第三次/c_2/c_2.srcs/constrs_1/new/lab1_nexys4.xdc
set_property used_in_implementation false [get_files /home/orithu/桌面/第三次/c_2/c_2.srcs/constrs_1/new/lab1_nexys4.xdc]

synth_design -top c -part xc7a100tcsg324-1
write_checkpoint -noxdef c.dcp
catch { report_utilization -file c_utilization_synth.rpt -pb c_utilization_synth.pb }
