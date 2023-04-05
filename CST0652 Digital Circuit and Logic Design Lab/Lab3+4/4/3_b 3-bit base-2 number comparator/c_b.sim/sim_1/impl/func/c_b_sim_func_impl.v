// Copyright 1986-2015 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2015.2 (lin64) Build 1266856 Fri Jun 26 16:35:25 MDT 2015
// Date        : Fri Jun 15 17:07:42 2018
// Host        : orithu-Lenovo-V480s running 64-bit Ubuntu 17.10
// Command     : write_verilog -mode funcsim -nolib -force -file
//               /home/orithu/桌面/FORTH/c_2/c_b.sim/sim_1/impl/func/c_b_sim_func_impl.v
// Design      : c_b
// Purpose     : This verilog netlist is a functional simulation representation of the design and should not be modified
//               or synthesized. This netlist cannot be used for SDF annotated simulation.
// Device      : xc7a100tcsg324-1
// --------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

(* ECO_CHECKSUM = "c19234e1" *) 
(* NotValidForBitStream *)
module c_b
   (a,
    b,
    F1,
    F2,
    F3);
  input [2:0]a;
  input [2:0]b;
  output F1;
  output F2;
  output F3;

  wire F1;
  wire F1_OBUF;
  wire F2;
  wire F2_OBUF;
  wire F3;
  wire F3_OBUF;
  wire [2:0]a;
  wire [2:0]a_IBUF;
  wire [2:0]b;
  wire [2:0]b_IBUF;

  OBUF F1_OBUF_inst
       (.I(F1_OBUF),
        .O(F1));
  LUT6 #(
    .INIT(64'h20F220F2B0FB20F2)) 
    F1_OBUF_inst_i_1
       (.I0(a_IBUF[1]),
        .I1(b_IBUF[1]),
        .I2(a_IBUF[2]),
        .I3(b_IBUF[2]),
        .I4(a_IBUF[0]),
        .I5(b_IBUF[0]),
        .O(F1_OBUF));
  OBUF F2_OBUF_inst
       (.I(F2_OBUF),
        .O(F2));
  LUT6 #(
    .INIT(64'h9009000000009009)) 
    F2_OBUF_inst_i_1
       (.I0(a_IBUF[0]),
        .I1(b_IBUF[0]),
        .I2(b_IBUF[2]),
        .I3(a_IBUF[2]),
        .I4(b_IBUF[1]),
        .I5(a_IBUF[1]),
        .O(F2_OBUF));
  OBUF F3_OBUF_inst
       (.I(F3_OBUF),
        .O(F3));
  LUT6 #(
    .INIT(64'h20F200F0F0FF20F2)) 
    F3_OBUF_inst_i_1
       (.I0(b_IBUF[0]),
        .I1(a_IBUF[0]),
        .I2(b_IBUF[2]),
        .I3(a_IBUF[2]),
        .I4(b_IBUF[1]),
        .I5(a_IBUF[1]),
        .O(F3_OBUF));
  IBUF \a_IBUF[0]_inst 
       (.I(a[0]),
        .O(a_IBUF[0]));
  IBUF \a_IBUF[1]_inst 
       (.I(a[1]),
        .O(a_IBUF[1]));
  IBUF \a_IBUF[2]_inst 
       (.I(a[2]),
        .O(a_IBUF[2]));
  IBUF \b_IBUF[0]_inst 
       (.I(b[0]),
        .O(b_IBUF[0]));
  IBUF \b_IBUF[1]_inst 
       (.I(b[1]),
        .O(b_IBUF[1]));
  IBUF \b_IBUF[2]_inst 
       (.I(b[2]),
        .O(b_IBUF[2]));
endmodule
`ifndef GLBL
`define GLBL
`timescale  1 ps / 1 ps

module glbl ();

    parameter ROC_WIDTH = 100000;
    parameter TOC_WIDTH = 0;

//--------   STARTUP Globals --------------
    wire GSR;
    wire GTS;
    wire GWE;
    wire PRLD;
    tri1 p_up_tmp;
    tri (weak1, strong0) PLL_LOCKG = p_up_tmp;

    wire PROGB_GLBL;
    wire CCLKO_GLBL;
    wire FCSBO_GLBL;
    wire [3:0] DO_GLBL;
    wire [3:0] DI_GLBL;
   
    reg GSR_int;
    reg GTS_int;
    reg PRLD_int;

//--------   JTAG Globals --------------
    wire JTAG_TDO_GLBL;
    wire JTAG_TCK_GLBL;
    wire JTAG_TDI_GLBL;
    wire JTAG_TMS_GLBL;
    wire JTAG_TRST_GLBL;

    reg JTAG_CAPTURE_GLBL;
    reg JTAG_RESET_GLBL;
    reg JTAG_SHIFT_GLBL;
    reg JTAG_UPDATE_GLBL;
    reg JTAG_RUNTEST_GLBL;

    reg JTAG_SEL1_GLBL = 0;
    reg JTAG_SEL2_GLBL = 0 ;
    reg JTAG_SEL3_GLBL = 0;
    reg JTAG_SEL4_GLBL = 0;

    reg JTAG_USER_TDO1_GLBL = 1'bz;
    reg JTAG_USER_TDO2_GLBL = 1'bz;
    reg JTAG_USER_TDO3_GLBL = 1'bz;
    reg JTAG_USER_TDO4_GLBL = 1'bz;

    assign (weak1, weak0) GSR = GSR_int;
    assign (weak1, weak0) GTS = GTS_int;
    assign (weak1, weak0) PRLD = PRLD_int;

    initial begin
	GSR_int = 1'b1;
	PRLD_int = 1'b1;
	#(ROC_WIDTH)
	GSR_int = 1'b0;
	PRLD_int = 1'b0;
    end

    initial begin
	GTS_int = 1'b1;
	#(TOC_WIDTH)
	GTS_int = 1'b0;
    end

endmodule
`endif
