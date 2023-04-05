// Copyright 1986-2015 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2015.2 (lin64) Build 1266856 Fri Jun 26 16:35:25 MDT 2015
// Date        : Wed Jun 13 14:51:41 2018
// Host        : orithu-Lenovo-V480s running 64-bit Ubuntu 17.10
// Command     : write_verilog -mode funcsim -nolib -force -file
//               /home/orithu/桌面/第三次/b_2/b_2.sim/sim_1/impl/func/b_sim_func_impl.v
// Design      : b
// Purpose     : This verilog netlist is a functional simulation representation of the design and should not be modified
//               or synthesized. This netlist cannot be used for SDF annotated simulation.
// Device      : xc7a100tcsg324-1
// --------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

(* ECO_CHECKSUM = "77f8a33a" *) 
(* NotValidForBitStream *)
module b
   (x,
    z,
    q1,
    q2,
    q3,
    q4);
  input x;
  output z;
  output q1;
  output q2;
  output q3;
  output q4;

  wire jk2_n_1;
  wire jk4_n_2;
  wire q1;
  wire q1_OBUF;
  wire q2;
  wire q2_OBUF;
  wire q3;
  wire q3_OBUF;
  wire q4;
  wire q4_OBUF;
  wire x;
  wire x_IBUF;
  wire x_IBUF_BUFG;
  wire z;
  wire z_OBUF;

  jk jk1
       (.q1_OBUF(q1_OBUF),
        .x_IBUF_BUFG(x_IBUF_BUFG));
  jk_0 jk2
       (.q1_OBUF(q1_OBUF),
        .q2_OBUF(q2_OBUF),
        .q3_OBUF(q3_OBUF),
        .q4_OBUF(q4_OBUF),
        .q_reg_0(jk2_n_1),
        .q_reg_1(jk4_n_2));
  jk_1 jk3
       (.q2_OBUF(q2_OBUF),
        .q3_OBUF(q3_OBUF));
  jk_2 jk4
       (.q1_OBUF(q1_OBUF),
        .q2_OBUF(q2_OBUF),
        .q4_OBUF(q4_OBUF),
        .q_reg_0(jk4_n_2),
        .q_reg_1(jk2_n_1),
        .x_IBUF(x_IBUF),
        .z_OBUF(z_OBUF));
  OBUF q1_OBUF_inst
       (.I(q1_OBUF),
        .O(q1));
  OBUF q2_OBUF_inst
       (.I(q2_OBUF),
        .O(q2));
  OBUF q3_OBUF_inst
       (.I(q3_OBUF),
        .O(q3));
  OBUF q4_OBUF_inst
       (.I(q4_OBUF),
        .O(q4));
  BUFG x_IBUF_BUFG_inst
       (.I(x_IBUF),
        .O(x_IBUF_BUFG));
  IBUF x_IBUF_inst
       (.I(x),
        .O(x_IBUF));
  OBUF z_OBUF_inst
       (.I(z_OBUF),
        .O(z));
endmodule

module jk
   (q1_OBUF,
    x_IBUF_BUFG);
  output q1_OBUF;
  input x_IBUF_BUFG;

  wire q1_OBUF;
  wire q_i_1_n_0;
  wire x_IBUF_BUFG;

  LUT1 #(
    .INIT(2'h1)) 
    q_i_1
       (.I0(q1_OBUF),
        .O(q_i_1_n_0));
  FDRE #(
    .INIT(1'b0),
    .IS_C_INVERTED(1'b1)) 
    q_reg
       (.C(x_IBUF_BUFG),
        .CE(1'b1),
        .D(q_i_1_n_0),
        .Q(q1_OBUF),
        .R(1'b0));
endmodule

(* ORIG_REF_NAME = "jk" *) 
module jk_0
   (q2_OBUF,
    q_reg_0,
    q_reg_1,
    q1_OBUF,
    q3_OBUF,
    q4_OBUF);
  output q2_OBUF;
  output q_reg_0;
  input q_reg_1;
  input q1_OBUF;
  input q3_OBUF;
  input q4_OBUF;

  wire q1_OBUF;
  wire q2_OBUF;
  wire q3_OBUF;
  wire q4_OBUF;
  wire q_reg_0;
  wire q_reg_1;

  LUT3 #(
    .INIT(8'h08)) 
    q_i_1__1
       (.I0(q2_OBUF),
        .I1(q3_OBUF),
        .I2(q4_OBUF),
        .O(q_reg_0));
  FDRE #(
    .INIT(1'b0),
    .IS_C_INVERTED(1'b1)) 
    q_reg
       (.C(q1_OBUF),
        .CE(1'b1),
        .D(q_reg_1),
        .Q(q2_OBUF),
        .R(1'b0));
endmodule

(* ORIG_REF_NAME = "jk" *) 
module jk_1
   (q3_OBUF,
    q2_OBUF);
  output q3_OBUF;
  input q2_OBUF;

  wire q2_OBUF;
  wire q3_OBUF;
  wire q_i_1__0_n_0;

  LUT1 #(
    .INIT(2'h1)) 
    q_i_1__0
       (.I0(q3_OBUF),
        .O(q_i_1__0_n_0));
  FDRE #(
    .INIT(1'b0),
    .IS_C_INVERTED(1'b1)) 
    q_reg
       (.C(q2_OBUF),
        .CE(1'b1),
        .D(q_i_1__0_n_0),
        .Q(q3_OBUF),
        .R(1'b0));
endmodule

(* ORIG_REF_NAME = "jk" *) 
module jk_2
   (q4_OBUF,
    z_OBUF,
    q_reg_0,
    q_reg_1,
    q1_OBUF,
    x_IBUF,
    q2_OBUF);
  output q4_OBUF;
  output z_OBUF;
  output q_reg_0;
  input q_reg_1;
  input q1_OBUF;
  input x_IBUF;
  input q2_OBUF;

  wire q1_OBUF;
  wire q2_OBUF;
  wire q4_OBUF;
  wire q_reg_0;
  wire q_reg_1;
  wire x_IBUF;
  wire z_OBUF;

  LUT2 #(
    .INIT(4'h1)) 
    q_i_1__2
       (.I0(q4_OBUF),
        .I1(q2_OBUF),
        .O(q_reg_0));
  FDRE #(
    .INIT(1'b0),
    .IS_C_INVERTED(1'b1)) 
    q_reg
       (.C(q1_OBUF),
        .CE(1'b1),
        .D(q_reg_1),
        .Q(q4_OBUF),
        .R(1'b0));
  LUT3 #(
    .INIT(8'h80)) 
    z_OBUF_inst_i_1
       (.I0(q4_OBUF),
        .I1(q1_OBUF),
        .I2(x_IBUF),
        .O(z_OBUF));
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
