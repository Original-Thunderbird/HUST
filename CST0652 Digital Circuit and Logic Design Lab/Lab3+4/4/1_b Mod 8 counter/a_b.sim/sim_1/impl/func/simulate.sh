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
ExecStep $xv_path/bin/xsim a_b_sim_func_impl -key {Post-Implementation:sim_1:Functional:a_b_sim} -tclbatch a_b_sim.tcl -log simulate.log
