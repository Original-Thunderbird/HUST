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
ExecStep $xv_path/bin/xsim c_b_sim_func_impl -key {Post-Implementation:sim_1:Functional:c_b_sim} -tclbatch c_b_sim.tcl -log simulate.log
