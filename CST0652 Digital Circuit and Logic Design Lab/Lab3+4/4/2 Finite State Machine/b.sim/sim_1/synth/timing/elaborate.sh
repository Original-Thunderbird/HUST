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
ExecStep $xv_path/bin/xelab -wto 75935e59fe584ddf9f8e8eb232e07e30 -m64 --debug typical --relax --mt 8 --maxdelay -L xil_defaultlib -L simprims_ver -L secureip --snapshot b_sim_time_synth -transport_int_delays -pulse_r 0 -pulse_int_r 0 xil_defaultlib.b_sim xil_defaultlib.glbl -log elaborate.log
