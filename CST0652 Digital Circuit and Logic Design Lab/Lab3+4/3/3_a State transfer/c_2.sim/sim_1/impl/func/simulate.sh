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
ExecStep $xv_path/bin/xsim c_fz_func_impl -key {Post-Implementation:sim_1:Functional:c_fz} -tclbatch c_fz.tcl -log simulate.log
