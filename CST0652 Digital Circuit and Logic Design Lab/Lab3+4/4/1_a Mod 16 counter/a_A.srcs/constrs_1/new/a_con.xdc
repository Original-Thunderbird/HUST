# Nexys4 Pin Assignments
############################
# On-board Slide Switches  #
############################
set_property PACKAGE_PIN U9 [get_ports in[0]]
set_property IOSTANDARD LVCMOS33 [get_ports in[0]]
set_property PACKAGE_PIN U8 [get_ports in[1]]
set_property IOSTANDARD LVCMOS33 [get_ports in[1]]
set_property PACKAGE_PIN R7 [get_ports in[2]]
set_property IOSTANDARD LVCMOS33 [get_ports in[2]]
set_property PACKAGE_PIN R6 [get_ports in[3]]
set_property IOSTANDARD LVCMOS33 [get_ports in[3]]
set_property PACKAGE_PIN R5 [get_ports M]
set_property IOSTANDARD LVCMOS33 [get_ports M]
set_property PACKAGE_PIN R10 [get_ports CP]      
set_property IOSTANDARD LVCMOS33 [get_ports CP]
set_property PACKAGE_PIN V6 [get_ports CLR]
set_property IOSTANDARD LVCMOS33 [get_ports CLR]
set_property PACKAGE_PIN V5 [get_ports Ld]
set_property IOSTANDARD LVCMOS33 [get_ports Ld]

set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets CP_IBUF]
############################
# On-board led             #
############################
set_property PACKAGE_PIN T8 [get_ports out[0]]
set_property IOSTANDARD LVCMOS33 [get_ports out[0]]
set_property PACKAGE_PIN V9 [get_ports out[1]]
set_property IOSTANDARD LVCMOS33 [get_ports out[1]]
set_property PACKAGE_PIN R8 [get_ports out[2]]
set_property IOSTANDARD LVCMOS33 [get_ports out[2]]
set_property PACKAGE_PIN T6 [get_ports out[3]]
set_property IOSTANDARD LVCMOS33 [get_ports out[3]]
set_property PACKAGE_PIN T5 [get_ports Qcc]
set_property IOSTANDARD LVCMOS33 [get_ports Qcc]



