// Copyright 1986-2015 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2015.2 (lin64) Build 1266856 Fri Jun 26 16:35:25 MDT 2015
// Date        : Tue Jun 12 16:53:54 2018
// Host        : orithu-Lenovo-V480s running 64-bit Ubuntu 17.10
// Command     : write_verilog -mode funcsim -nolib -force -file
//               /home/orithu/桌面/第三次/c_1/c_2.sim/sim_1/impl/func/c_fz_func_impl.v
// Design      : c
// Purpose     : This verilog netlist is a functional simulation representation of the design and should not be modified
//               or synthesized. This netlist cannot be used for SDF annotated simulation.
// Device      : xc7a100tcsg324-1
// --------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

(* ECO_CHECKSUM = "d7c85a1f" *) 
(* NotValidForBitStream *)
module c
   (sig_nsyn,
    clk,
    q1,
    q2,
    sig_syn);
  input sig_nsyn;
  input clk;
  output q1;
  output q2;
  output sig_syn;

  wire clk;
  wire clk_IBUF;
  wire clk_IBUF_BUFG;
  wire q1;
  wire q1_OBUF;
  wire q2;
  wire q2_OBUF;
  wire sig_nsyn;
  wire sig_nsyn_IBUF;
  wire sig_syn;
  wire sig_syn_OBUF;

  BUFG clk_IBUF_BUFG_inst
       (.I(clk_IBUF),
        .O(clk_IBUF_BUFG));
  IBUF clk_IBUF_inst
       (.I(clk),
        .O(clk_IBUF));
  OBUF q1_OBUF_inst
       (.I(q1_OBUF),
        .O(q1));
  FDRE #(
    .INIT(1'b0)) 
    q1_reg
       (.C(clk_IBUF_BUFG),
        .CE(1'b1),
        .D(sig_nsyn_IBUF),
        .Q(q1_OBUF),
        .R(1'b0));
  OBUF q2_OBUF_inst
       (.I(q2_OBUF),
        .O(q2));
  FDRE #(
    .INIT(1'b0)) 
    q2_reg
       (.C(clk_IBUF_BUFG),
        .CE(1'b1),
        .D(q1_OBUF),
        .Q(q2_OBUF),
        .R(1'b0));
  IBUF sig_nsyn_IBUF_inst
       (.I(sig_nsyn),
        .O(sig_nsyn_IBUF));
  OBUF sig_syn_OBUF_inst
       (.I(sig_syn_OBUF),
        .O(sig_syn));
  FDRE #(
    .INIT(1'b0)) 
    sig_syn_reg
       (.C(clk_IBUF_BUFG),
        .CE(1'b1),
        .D(q2_OBUF),
        .Q(sig_syn_OBUF),
        .R(1'b0));
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
