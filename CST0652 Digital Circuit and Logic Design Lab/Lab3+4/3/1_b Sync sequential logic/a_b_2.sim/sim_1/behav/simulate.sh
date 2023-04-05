#!/bin/sh -f
xv_path="/opt/Xilinx/Vivado/2015.2"
ExecStep()
{
"$@"
RETVAL=$?
if [ $RETVAL -ne 0 ]
then
exit $RETVAL
fi
}
ExecStep $xv_path/bin/xsim a_B_sim_behav -key {Behavioral:sim_1:Functional:a_B_sim} -tclbatch a_B_sim.tcl -log simulate.log
