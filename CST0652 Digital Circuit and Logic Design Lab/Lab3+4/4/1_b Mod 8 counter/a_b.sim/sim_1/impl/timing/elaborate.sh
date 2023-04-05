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
ExecStep $xv_path/bin/xelab -wto bcdd3b5fdca84f018f484b9a752c60f9 -m64 --debug typical --relax --mt 8 --maxdelay -L xil_defaultlib -L simprims_ver -L secureip --snapshot a_b_sim_time_impl -transport_int_delays -pulse_r 0 -pulse_int_r 0 xil_defaultlib.a_b_sim xil_defaultlib.glbl -log elaborate.log
