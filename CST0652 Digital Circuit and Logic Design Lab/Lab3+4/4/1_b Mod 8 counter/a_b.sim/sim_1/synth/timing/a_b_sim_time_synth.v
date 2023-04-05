// Copyright 1986-2015 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2015.2 (lin64) Build 1266856 Fri Jun 26 16:35:25 MDT 2015
// Date        : Thu Jun 14 16:03:21 2018
// Host        : orithu-Lenovo-V480s running 64-bit Ubuntu 17.10
// Command     : write_verilog -mode timesim -nolib -sdf_anno true -force -file
//               /home/orithu/a_b/a_b.sim/sim_1/synth/timing/a_b_sim_time_synth.v
// Design      : a_b
// Purpose     : This verilog netlist is a timing simulation representation of the design and should not be modified or
//               synthesized. Please ensure that this netlist is used with the corresponding SDF file.
// Device      : xc7a100tcsg324-1
// --------------------------------------------------------------------------------
`timescale 1 ps / 1 ps
`define XIL_TIMING

module a
   (Qcc_reg,
    p_0_in,
    p_1_in,
    p_3_in,
    \out_reg[0] ,
    CLR_reg,
    Ld_reg,
    CP_IBUF_BUFG,
    M_IBUF,
    CLR,
    Ld);
  output Qcc_reg;
  output p_0_in;
  output p_1_in;
  output p_3_in;
  output \out_reg[0] ;
  output CLR_reg;
  output Ld_reg;
  input CP_IBUF_BUFG;
  input M_IBUF;
  input CLR;
  input Ld;

  wire CLR;
  wire CLR_reg;
  wire CP_IBUF_BUFG;
  wire Ld;
  wire Ld_reg;
  wire M_IBUF;
  wire Qcc_reg;
  wire \out[0]_C_i_1_n_0 ;
  wire \out[1]_C_i_1_n_0 ;
  wire \out[2]_i_1_n_0 ;
  wire \out[2]_i_2_n_0 ;
  wire \out[3]_C_i_1_n_0 ;
  wire \out_reg[0] ;
  wire \out_reg[0]_C_n_0 ;
  wire \out_reg[0]_P_n_0 ;
  wire \out_reg[1]_C_n_0 ;
  wire \out_reg[1]_LDC_n_0 ;
  wire \out_reg[1]_P_n_0 ;
  wire \out_reg[3]_C_n_0 ;
  wire \out_reg[3]_LDC_i_1_n_0 ;
  wire \out_reg[3]_LDC_i_2_n_0 ;
  wire \out_reg[3]_LDC_n_0 ;
  wire \out_reg[3]_P_n_0 ;
  wire p_0_in;
  wire p_1_in;
  wire p_3_in;

  LUT6 #(
    .INIT(64'hFFF7FFFFFFFFFFFF)) 
    CLR_i_1
       (.I0(\out_reg[0] ),
        .I1(p_1_in),
        .I2(p_3_in),
        .I3(p_0_in),
        .I4(M_IBUF),
        .I5(CLR),
        .O(CLR_reg));
  LUT6 #(
    .INIT(64'hFFFFFFFBFFFFFFFF)) 
    Ld_i_1
       (.I0(p_1_in),
        .I1(p_3_in),
        .I2(M_IBUF),
        .I3(p_0_in),
        .I4(\out_reg[0] ),
        .I5(Ld),
        .O(Ld_reg));
  (* SOFT_HLUTNM = "soft_lutpair0" *) 
  LUT5 #(
    .INIT(32'hFFEFFDFF)) 
    Qcc_i_1
       (.I0(M_IBUF),
        .I1(p_0_in),
        .I2(p_1_in),
        .I3(p_3_in),
        .I4(\out_reg[0] ),
        .O(Qcc_reg));
  LUT1 #(
    .INIT(2'h1)) 
    \out[0]_C_i_1 
       (.I0(\out_reg[0] ),
        .O(\out[0]_C_i_1_n_0 ));
  (* SOFT_HLUTNM = "soft_lutpair2" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \out[0]_i_1 
       (.I0(\out_reg[0]_P_n_0 ),
        .I1(\out_reg[3]_LDC_n_0 ),
        .I2(\out_reg[0]_C_n_0 ),
        .O(\out_reg[0] ));
  (* SOFT_HLUTNM = "soft_lutpair1" *) 
  LUT3 #(
    .INIT(8'h69)) 
    \out[1]_C_i_1 
       (.I0(M_IBUF),
        .I1(p_3_in),
        .I2(\out_reg[0] ),
        .O(\out[1]_C_i_1_n_0 ));
  LUT3 #(
    .INIT(8'hB8)) 
    \out[1]_i_1 
       (.I0(\out_reg[1]_P_n_0 ),
        .I1(\out_reg[1]_LDC_n_0 ),
        .I2(\out_reg[1]_C_n_0 ),
        .O(p_3_in));
  (* SOFT_HLUTNM = "soft_lutpair1" *) 
  LUT4 #(
    .INIT(16'h7E81)) 
    \out[2]_i_1 
       (.I0(M_IBUF),
        .I1(p_3_in),
        .I2(\out_reg[0] ),
        .I3(p_0_in),
        .O(\out[2]_i_1_n_0 ));
  LUT2 #(
    .INIT(4'h7)) 
    \out[2]_i_2 
       (.I0(CLR),
        .I1(Ld),
        .O(\out[2]_i_2_n_0 ));
  (* SOFT_HLUTNM = "soft_lutpair0" *) 
  LUT5 #(
    .INIT(32'h6CCCCCC9)) 
    \out[3]_C_i_1 
       (.I0(M_IBUF),
        .I1(p_1_in),
        .I2(p_3_in),
        .I3(\out_reg[0] ),
        .I4(p_0_in),
        .O(\out[3]_C_i_1_n_0 ));
  (* SOFT_HLUTNM = "soft_lutpair2" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \out[3]_i_1 
       (.I0(\out_reg[3]_P_n_0 ),
        .I1(\out_reg[3]_LDC_n_0 ),
        .I2(\out_reg[3]_C_n_0 ),
        .O(p_1_in));
  FDCE #(
    .INIT(1'b0)) 
    \out_reg[0]_C 
       (.C(CP_IBUF_BUFG),
        .CE(1'b1),
        .CLR(\out_reg[3]_LDC_i_2_n_0 ),
        .D(\out[0]_C_i_1_n_0 ),
        .Q(\out_reg[0]_C_n_0 ));
  FDPE #(
    .INIT(1'b1)) 
    \out_reg[0]_P 
       (.C(CP_IBUF_BUFG),
        .CE(1'b1),
        .D(\out[0]_C_i_1_n_0 ),
        .PRE(\out_reg[3]_LDC_i_1_n_0 ),
        .Q(\out_reg[0]_P_n_0 ));
  FDCE #(
    .INIT(1'b0)) 
    \out_reg[1]_C 
       (.C(CP_IBUF_BUFG),
        .CE(1'b1),
        .CLR(\out_reg[3]_LDC_i_1_n_0 ),
        .D(\out[1]_C_i_1_n_0 ),
        .Q(\out_reg[1]_C_n_0 ));
  (* XILINX_LEGACY_PRIM = "LDC" *) 
  LDCE #(
    .INIT(1'b1),
    .IS_G_INVERTED(1'b1)) 
    \out_reg[1]_LDC 
       (.CLR(\out_reg[3]_LDC_i_1_n_0 ),
        .D(1'b1),
        .G(CLR),
        .GE(1'b1),
        .Q(\out_reg[1]_LDC_n_0 ));
  FDPE #(
    .INIT(1'b1)) 
    \out_reg[1]_P 
       (.C(CP_IBUF_BUFG),
        .CE(1'b1),
        .D(\out[1]_C_i_1_n_0 ),
        .PRE(\out_reg[3]_LDC_i_2_n_0 ),
        .Q(\out_reg[1]_P_n_0 ));
  FDCE #(
    .INIT(1'b0)) 
    \out_reg[2] 
       (.C(CP_IBUF_BUFG),
        .CE(1'b1),
        .CLR(\out[2]_i_2_n_0 ),
        .D(\out[2]_i_1_n_0 ),
        .Q(p_0_in));
  FDCE #(
    .INIT(1'b0)) 
    \out_reg[3]_C 
       (.C(CP_IBUF_BUFG),
        .CE(1'b1),
        .CLR(\out_reg[3]_LDC_i_2_n_0 ),
        .D(\out[3]_C_i_1_n_0 ),
        .Q(\out_reg[3]_C_n_0 ));
  (* XILINX_LEGACY_PRIM = "LDC" *) 
  LDCE #(
    .INIT(1'b0)) 
    \out_reg[3]_LDC 
       (.CLR(\out_reg[3]_LDC_i_2_n_0 ),
        .D(1'b1),
        .G(\out_reg[3]_LDC_i_1_n_0 ),
        .GE(1'b1),
        .Q(\out_reg[3]_LDC_n_0 ));
  LUT2 #(
    .INIT(4'h2)) 
    \out_reg[3]_LDC_i_1 
       (.I0(CLR),
        .I1(Ld),
        .O(\out_reg[3]_LDC_i_1_n_0 ));
  LUT1 #(
    .INIT(2'h1)) 
    \out_reg[3]_LDC_i_2 
       (.I0(CLR),
        .O(\out_reg[3]_LDC_i_2_n_0 ));
  FDPE #(
    .INIT(1'b1)) 
    \out_reg[3]_P 
       (.C(CP_IBUF_BUFG),
        .CE(1'b1),
        .D(\out[3]_C_i_1_n_0 ),
        .PRE(\out_reg[3]_LDC_i_1_n_0 ),
        .Q(\out_reg[3]_P_n_0 ));
endmodule

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

  wire CLR;
  wire CP;
  wire CP_IBUF;
  wire CP_IBUF_BUFG;
  wire Ld;
  wire M;
  wire M_IBUF;
  wire Qcc;
  wire Qcc_OBUF;
  wire [3:0]out;
  wire [3:0]out_OBUF;
  wire p_0_in;
  wire p_1_in;
  wire p_3_in;
  wire rlz_n_0;
  wire rlz_n_4;
  wire rlz_n_5;
  wire rlz_n_6;

initial begin
 $sdf_annotate("a_b_sim_time_synth.sdf",,,,"tool_control");
end
  FDRE #(
    .INIT(1'b1)) 
    CLR_reg
       (.C(CP_IBUF_BUFG),
        .CE(1'b1),
        .D(rlz_n_5),
        .Q(CLR),
        .R(1'b0));
  BUFG CP_IBUF_BUFG_inst
       (.I(CP_IBUF),
        .O(CP_IBUF_BUFG));
  IBUF CP_IBUF_inst
       (.I(CP),
        .O(CP_IBUF));
  FDRE #(
    .INIT(1'b1)) 
    Ld_reg
       (.C(CP_IBUF_BUFG),
        .CE(1'b1),
        .D(rlz_n_6),
        .Q(Ld),
        .R(1'b0));
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
        .D(rlz_n_4),
        .Q(out_OBUF[0]),
        .R(1'b0));
  FDRE #(
    .INIT(1'b1)) 
    \out_reg[1] 
       (.C(CP_IBUF_BUFG),
        .CE(1'b1),
        .D(p_3_in),
        .Q(out_OBUF[1]),
        .R(1'b0));
  FDRE #(
    .INIT(1'b0)) 
    \out_reg[2] 
       (.C(CP_IBUF_BUFG),
        .CE(1'b1),
        .D(p_0_in),
        .Q(out_OBUF[2]),
        .R(1'b0));
  FDRE #(
    .INIT(1'b0)) 
    \out_reg[3] 
       (.C(CP_IBUF_BUFG),
        .CE(1'b1),
        .D(p_1_in),
        .Q(out_OBUF[3]),
        .R(1'b0));
  a rlz
       (.CLR(CLR),
        .CLR_reg(rlz_n_5),
        .CP_IBUF_BUFG(CP_IBUF_BUFG),
        .Ld(Ld),
        .Ld_reg(rlz_n_6),
        .M_IBUF(M_IBUF),
        .Qcc_reg(rlz_n_0),
        .\out_reg[0] (rlz_n_4),
        .p_0_in(p_0_in),
        .p_1_in(p_1_in),
        .p_3_in(p_3_in));
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
