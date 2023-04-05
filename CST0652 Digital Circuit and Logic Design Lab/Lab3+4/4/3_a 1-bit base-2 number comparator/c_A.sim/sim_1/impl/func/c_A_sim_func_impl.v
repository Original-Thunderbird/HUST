// Copyright 1986-2015 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2015.2 (lin64) Build 1266856 Fri Jun 26 16:35:25 MDT 2015
// Date        : Tue Jun 12 09:44:06 2018
// Host        : orithu-Lenovo-V480s running 64-bit Ubuntu 17.10
// Command     : write_verilog -mode funcsim -nolib -force -file
//               /home/orithu/c_A/c_A.sim/sim_1/impl/func/c_A_sim_func_impl.v
// Design      : c_A
// Purpose     : This verilog netlist is a functional simulation representation of the design and should not be modified
//               or synthesized. This netlist cannot be used for SDF annotated simulation.
// Device      : xc7a100tcsg324-1
// --------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

(* ECO_CHECKSUM = "ac19a246" *) 
(* NotValidForBitStream *)
module c_A
   (a,
    b,
    F1,
    F2,
    F3);
  input a;
  input b;
  output F1;
  output F2;
  output F3;

  wire F1;
  wire F1_OBUF;
  wire F1_reg_i_1_n_0;
  wire F1_reg_i_2_n_0;
  wire F1_reg_i_3_n_0;
  wire F2;
  wire F2_OBUF;
  wire F3;
  wire F3_OBUF;
  wire F3_reg_i_1_n_0;
  wire a;
  wire a_IBUF;
  wire b;
  wire b_IBUF;

  OBUF F1_OBUF_inst
       (.I(F1_OBUF),
        .O(F1));
  LDCP #(
    .INIT(1'b0)) 
    F1_reg
       (.CLR(F1_reg_i_2_n_0),
        .D(1'b0),
        .G(F1_reg_i_1_n_0),
        .PRE(F1_reg_i_3_n_0),
        .Q(F1_OBUF));
  LUT2 #(
    .INIT(4'h2)) 
    F1_reg_i_1
       (.I0(b_IBUF),
        .I1(a_IBUF),
        .O(F1_reg_i_1_n_0));
  LUT2 #(
    .INIT(4'h9)) 
    F1_reg_i_2
       (.I0(b_IBUF),
        .I1(a_IBUF),
        .O(F1_reg_i_2_n_0));
  LUT2 #(
    .INIT(4'h2)) 
    F1_reg_i_3
       (.I0(a_IBUF),
        .I1(b_IBUF),
        .O(F1_reg_i_3_n_0));
  OBUF F2_OBUF_inst
       (.I(F2_OBUF),
        .O(F2));
  LDCP #(
    .INIT(1'b0)) 
    F2_reg
       (.CLR(F1_reg_i_3_n_0),
        .D(1'b0),
        .G(F1_reg_i_1_n_0),
        .PRE(F1_reg_i_2_n_0),
        .Q(F2_OBUF));
  OBUF F3_OBUF_inst
       (.I(F3_OBUF),
        .O(F3));
  (* XILINX_LEGACY_PRIM = "LDC" *) 
  LDCE #(
    .INIT(1'b0)) 
    F3_reg
       (.CLR(F3_reg_i_1_n_0),
        .D(F1_reg_i_1_n_0),
        .G(F1_reg_i_1_n_0),
        .GE(1'b1),
        .Q(F3_OBUF));
  LUT2 #(
    .INIT(4'hD)) 
    F3_reg_i_1
       (.I0(b_IBUF),
        .I1(a_IBUF),
        .O(F3_reg_i_1_n_0));
  IBUF a_IBUF_inst
       (.I(a),
        .O(a_IBUF));
  IBUF b_IBUF_inst
       (.I(b),
        .O(b_IBUF));
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
