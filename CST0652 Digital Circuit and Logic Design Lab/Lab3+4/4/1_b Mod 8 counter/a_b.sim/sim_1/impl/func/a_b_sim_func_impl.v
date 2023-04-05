// Copyright 1986-2015 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2015.2 (lin64) Build 1266856 Fri Jun 26 16:35:25 MDT 2015
// Date        : Thu Jun 14 17:40:15 2018
// Host        : orithu-Lenovo-V480s running 64-bit Ubuntu 17.10
// Command     : write_verilog -mode funcsim -nolib -force -file
//               /home/orithu/a_b/a_b.sim/sim_1/impl/func/a_b_sim_func_impl.v
// Design      : a_b
// Purpose     : This verilog netlist is a functional simulation representation of the design and should not be modified
//               or synthesized. This netlist cannot be used for SDF annotated simulation.
// Device      : xc7a100tcsg324-1
// --------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

module a
   (Qcc_reg,
    put,
    M_IBUF,
    CP_IBUF_BUFG);
  output Qcc_reg;
  output [3:0]put;
  input M_IBUF;
  input CP_IBUF_BUFG;

  wire CP_IBUF_BUFG;
  wire M_IBUF;
  wire Qcc_reg;
  wire \out[0]_i_1_n_0 ;
  wire \out[1]_i_1_n_0 ;
  wire \out[2]_i_1_n_0 ;
  wire \out[3]_i_1_n_0 ;
  wire [3:0]put;

  (* SOFT_HLUTNM = "soft_lutpair0" *) 
  LUT5 #(
    .INIT(32'hFFEFFDFF)) 
    Qcc_i_1
       (.I0(M_IBUF),
        .I1(put[2]),
        .I2(put[3]),
        .I3(put[1]),
        .I4(put[0]),
        .O(Qcc_reg));
  LUT1 #(
    .INIT(2'h1)) 
    \out[0]_i_1 
       (.I0(put[0]),
        .O(\out[0]_i_1_n_0 ));
  (* SOFT_HLUTNM = "soft_lutpair1" *) 
  LUT3 #(
    .INIT(8'h69)) 
    \out[1]_i_1 
       (.I0(M_IBUF),
        .I1(put[1]),
        .I2(put[0]),
        .O(\out[1]_i_1_n_0 ));
  (* SOFT_HLUTNM = "soft_lutpair1" *) 
  LUT4 #(
    .INIT(16'h7E81)) 
    \out[2]_i_1 
       (.I0(M_IBUF),
        .I1(put[1]),
        .I2(put[0]),
        .I3(put[2]),
        .O(\out[2]_i_1_n_0 ));
  (* SOFT_HLUTNM = "soft_lutpair0" *) 
  LUT5 #(
    .INIT(32'h6CCCCCC9)) 
    \out[3]_i_1 
       (.I0(M_IBUF),
        .I1(put[3]),
        .I2(put[2]),
        .I3(put[1]),
        .I4(put[0]),
        .O(\out[3]_i_1_n_0 ));
  FDRE #(
    .INIT(1'b0)) 
    \out_reg[0] 
       (.C(CP_IBUF_BUFG),
        .CE(1'b1),
        .D(\out[0]_i_1_n_0 ),
        .Q(put[0]),
        .R(1'b0));
  FDRE #(
    .INIT(1'b1)) 
    \out_reg[1] 
       (.C(CP_IBUF_BUFG),
        .CE(1'b1),
        .D(\out[1]_i_1_n_0 ),
        .Q(put[1]),
        .R(1'b0));
  FDRE #(
    .INIT(1'b0)) 
    \out_reg[2] 
       (.C(CP_IBUF_BUFG),
        .CE(1'b1),
        .D(\out[2]_i_1_n_0 ),
        .Q(put[2]),
        .R(1'b0));
  FDRE #(
    .INIT(1'b0)) 
    \out_reg[3] 
       (.C(CP_IBUF_BUFG),
        .CE(1'b1),
        .D(\out[3]_i_1_n_0 ),
        .Q(put[3]),
        .R(1'b0));
endmodule

(* ECO_CHECKSUM = "358ab928" *) 
(* NotValidForBitStream *)
module a_b
   (Qcc,
    out,
    M,
    CP);
  output Qcc;
  output [3:0]out;
  input M;
  input CP;

  wire CP;
  wire CP_IBUF;
  wire CP_IBUF_BUFG;
  wire M;
  wire M_IBUF;
  wire Qcc;
  wire Qcc_OBUF;
  wire [3:0]out;
  wire [3:0]out_OBUF;
  wire [3:0]put;
  wire rlz_n_0;

  BUFG CP_IBUF_BUFG_inst
       (.I(CP_IBUF),
        .O(CP_IBUF_BUFG));
  IBUF CP_IBUF_inst
       (.I(CP),
        .O(CP_IBUF));
  IBUF M_IBUF_inst
       (.I(M),
        .O(M_IBUF));
  OBUF Qcc_OBUF_inst
       (.I(Qcc_OBUF),
        .O(Qcc));
  FDRE #(
    .INIT(1'b1)) 
    Qcc_reg
       (.C(CP_IBUF_BUFG),
        .CE(1'b1),
        .D(rlz_n_0),
        .Q(Qcc_OBUF),
        .R(1'b0));
  OBUF \out_OBUF[0]_inst 
       (.I(out_OBUF[0]),
        .O(out[0]));
  OBUF \out_OBUF[1]_inst 
       (.I(out_OBUF[1]),
        .O(out[1]));
  OBUF \out_OBUF[2]_inst 
       (.I(out_OBUF[2]),
        .O(out[2]));
  OBUF \out_OBUF[3]_inst 
       (.I(out_OBUF[3]),
        .O(out[3]));
  FDRE #(
    .INIT(1'b0)) 
    \out_reg[0] 
       (.C(CP_IBUF_BUFG),
        .CE(1'b1),
        .D(put[0]),
        .Q(out_OBUF[0]),
        .R(1'b0));
  FDRE #(
    .INIT(1'b1)) 
    \out_reg[1] 
       (.C(CP_IBUF_BUFG),
        .CE(1'b1),
        .D(put[1]),
        .Q(out_OBUF[1]),
        .R(1'b0));
  FDRE #(
    .INIT(1'b0)) 
    \out_reg[2] 
       (.C(CP_IBUF_BUFG),
        .CE(1'b1),
        .D(put[2]),
        .Q(out_OBUF[2]),
        .R(1'b0));
  FDRE #(
    .INIT(1'b0)) 
    \out_reg[3] 
       (.C(CP_IBUF_BUFG),
        .CE(1'b1),
        .D(put[3]),
        .Q(out_OBUF[3]),
        .R(1'b0));
  a rlz
       (.CP_IBUF_BUFG(CP_IBUF_BUFG),
        .M_IBUF(M_IBUF),
        .Qcc_reg(rlz_n_0),
        .put(put));
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
