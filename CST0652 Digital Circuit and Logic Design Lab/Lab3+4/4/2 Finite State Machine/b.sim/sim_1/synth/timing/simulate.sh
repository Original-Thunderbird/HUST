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
ExecStep $xv_path/bin/xsim b_sim_time_synth -key {Post-Synthesis:sim_1:Timing:b_sim} -tclbatch b_sim.tcl -log simulate.log
