# Nexys4 Pin Assignments
############################
# On-board Slide Switches  #
############################
set_property PACKAGE_PIN T16 [get_ports sig_nsyn]
set_property IOSTANDARD LVCMOS33 [get_ports sig_nsyn]
set_property PACKAGE_PIN E3 [get_ports cp]
set_property IOSTANDARD LVCMOS33 [get_ports cp]

############################
# On-board led             #
############################
set_property PACKAGE_PIN P2 [get_ports q1]
set_property IOSTANDARD LVCMOS33 [get_ports q1]
set_property PACKAGE_PIN U1 [get_ports q2]
set_property IOSTANDARD LVCMOS33 [get_ports q2]
set_property PACKAGE_PIN R1 [get_ports sig_syn]

set_property IOSTANDARD LVCMOS33 [get_ports sig_syn]
