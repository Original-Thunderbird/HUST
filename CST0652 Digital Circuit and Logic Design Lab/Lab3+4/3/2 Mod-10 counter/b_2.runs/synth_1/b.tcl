# 
# Synthesis run script generated by Vivado
# 

debug::add_scope template.lib 1
set_msg_config -id {HDL 9-1061} -limit 100000
set_msg_config -id {HDL 9-1654} -limit 100000
create_project -in_memory -part xc7a100tcsg324-1

set_param project.compositeFile.enableAutoGeneration 0
set_param synth.vivado.isSynthRun true
set_property webtalk.parent_dir /home/orithu/桌面/THIRD/b/b_2.cache/wt [current_project]
set_property parent.project_path /home/orithu/桌面/THIRD/b/b_2.xpr [current_project]
set_property default_lib xil_defaultlib [current_project]
set_property target_language Verilog [current_project]
set_property ip_repo_paths /home/orithu/桌面/THIRD/b/b.cache/ip [current_project]
set_property ip_output_repo /home/orithu/桌面/THIRD/b/b.cache/ip [current_project]
set_property ip_cache_permissions {read write} [current_project]
read_verilog -library xil_defaultlib /home/orithu/桌面/THIRD/b/b_2.srcs/sources_1/new/b.v
read_xdc /home/orithu/桌面/THIRD/b/b_2.srcs/constrs_1/new/b_con.xdc
set_property used_in_implementation false [get_files /home/orithu/桌面/THIRD/b/b_2.srcs/constrs_1/new/b_con.xdc]

synth_design -top b -part xc7a100tcsg324-1
write_checkpoint -noxdef b.dcp
catch { report_utilization -file b_utilization_synth.rpt -pb b_utilization_synth.pb }
