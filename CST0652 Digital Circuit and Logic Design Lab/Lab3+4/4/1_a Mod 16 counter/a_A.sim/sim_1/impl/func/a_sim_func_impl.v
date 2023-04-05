// Copyright 1986-2015 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2015.2 (lin64) Build 1266856 Fri Jun 26 16:35:25 MDT 2015
// Date        : Mon Jun 11 17:12:53 2018
// Host        : orithu-Lenovo-V480s running 64-bit Ubuntu 17.10
// Command     : write_verilog -mode funcsim -nolib -force -file /home/orithu/a_A/a_A.sim/sim_1/impl/func/a_sim_func_impl.v
// Design      : a
// Purpose     : This verilog netlist is a functional simulation representation of the design and should not be modified
//               or synthesized. This netlist cannot be used for SDF annotated simulation.
// Device      : xc7a100tcsg324-1
// --------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

(* ECO_CHECKSUM = "5dee54e5" *) 
(* NotValidForBitStream *)
module a
   (in,
    CP,
    CLR,
    Ld,
    M,
    out,
    Qcc);
  input [3:0]in;
  input CP;
  input CLR;
  input Ld;
  input M;
  output [3:0]out;
  output Qcc;

  wire CLR;
  wire CLR_IBUF;
  wire CP;
  wire CP_IBUF;
  wire CP_IBUF_BUFG;
  wire Ld;
  wire Ld_IBUF;
  wire M;
  wire M_IBUF;
  wire Qcc;
  wire Qcc_OBUF;
  wire Qcc_i_1_n_0;
  wire Qcc_i_2_n_0;
  wire Qcc_i_3_n_0;
  wire [3:0]in;
  wire [3:0]in_IBUF;
  wire [3:0]out;
  wire \out[0]_C_i_1_n_0 ;
  wire \out[1]_C_i_1_n_0 ;
  wire \out[2]_C_i_1_n_0 ;
  wire \out[3]_C_i_1_n_0 ;
  wire [3:0]out_OBUF;
  wire \out_reg[0]_C_n_0 ;
  wire \out_reg[0]_LDC_i_1_n_0 ;
  wire \out_reg[0]_LDC_i_2_n_0 ;
  wire \out_reg[0]_LDC_n_0 ;
  wire \out_reg[0]_P_n_0 ;
  wire \out_reg[1]_C_n_0 ;
  wire \out_reg[1]_LDC_i_1_n_0 ;
  wire \out_reg[1]_LDC_i_2_n_0 ;
  wire \out_reg[1]_LDC_n_0 ;
  wire \out_reg[1]_P_n_0 ;
  wire \out_reg[2]_C_n_0 ;
  wire \out_reg[2]_LDC_i_1_n_0 ;
  wire \out_reg[2]_LDC_i_2_n_0 ;
  wire \out_reg[2]_LDC_n_0 ;
  wire \out_reg[2]_P_n_0 ;
  wire \out_reg[3]_C_n_0 ;
  wire \out_reg[3]_LDC_i_1_n_0 ;
  wire \out_reg[3]_LDC_i_2_n_0 ;
  wire \out_reg[3]_LDC_n_0 ;
  wire \out_reg[3]_P_n_0 ;

  IBUF CLR_IBUF_inst
       (.I(CLR),
        .O(CLR_IBUF));
  BUFG CP_IBUF_BUFG_inst
       (.I(CP_IBUF),
        .O(CP_IBUF_BUFG));
  IBUF CP_IBUF_inst
       (.I(CP),
        .O(CP_IBUF));
  IBUF Ld_IBUF_inst
       (.I(Ld),
        .O(Ld_IBUF));
  IBUF M_IBUF_inst
       (.I(M),
        .O(M_IBUF));
  OBUF Qcc_OBUF_inst
       (.I(Qcc_OBUF),
        .O(Qcc));
  LUT6 #(
    .INIT(64'hBFBFBFFFFFFFBFFF)) 
    Qcc_i_1
       (.I0(Qcc_i_3_n_0),
        .I1(out_OBUF[2]),
        .I2(M_IBUF),
        .I3(\out_reg[3]_C_n_0 ),
        .I4(\out_reg[3]_LDC_n_0 ),
        .I5(\out_reg[3]_P_n_0 ),
        .O(Qcc_i_1_n_0));
  LUT2 #(
    .INIT(4'h7)) 
    Qcc_i_2
       (.I0(Ld_IBUF),
        .I1(CLR_IBUF),
        .O(Qcc_i_2_n_0));
  LUT6 #(
    .INIT(64'h1D1D1DFFFFFF1DFF)) 
    Qcc_i_3
       (.I0(\out_reg[1]_C_n_0 ),
        .I1(\out_reg[1]_LDC_n_0 ),
        .I2(\out_reg[1]_P_n_0 ),
        .I3(\out_reg[0]_C_n_0 ),
        .I4(\out_reg[0]_LDC_n_0 ),
        .I5(\out_reg[0]_P_n_0 ),
        .O(Qcc_i_3_n_0));
  FDPE #(
    .INIT(1'b1)) 
    Qcc_reg
       (.C(CP_IBUF_BUFG),
        .CE(1'b1),
        .D(Qcc_i_1_n_0),
        .PRE(Qcc_i_2_n_0),
        .Q(Qcc_OBUF));
  IBUF \in_IBUF[0]_inst 
       (.I(in[0]),
        .O(in_IBUF[0]));
  IBUF \in_IBUF[1]_inst 
       (.I(in[1]),
        .O(in_IBUF[1]));
  IBUF \in_IBUF[2]_inst 
       (.I(in[2]),
        .O(in_IBUF[2]));
  IBUF \in_IBUF[3]_inst 
       (.I(in[3]),
        .O(in_IBUF[3]));
  LUT1 #(
    .INIT(2'h1)) 
    \out[0]_C_i_1 
       (.I0(out_OBUF[0]),
        .O(\out[0]_C_i_1_n_0 ));
  LUT3 #(
    .INIT(8'h69)) 
    \out[1]_C_i_1 
       (.I0(M_IBUF),
        .I1(out_OBUF[1]),
        .I2(out_OBUF[0]),
        .O(\out[1]_C_i_1_n_0 ));
  LUT4 #(
    .INIT(16'h7E81)) 
    \out[2]_C_i_1 
       (.I0(M_IBUF),
        .I1(out_OBUF[1]),
        .I2(out_OBUF[0]),
        .I3(out_OBUF[2]),
        .O(\out[2]_C_i_1_n_0 ));
  LUT5 #(
    .INIT(32'h6CCCCCC9)) 
    \out[3]_C_i_1 
       (.I0(M_IBUF),
        .I1(out_OBUF[3]),
        .I2(out_OBUF[2]),
        .I3(out_OBUF[1]),
        .I4(out_OBUF[0]),
        .O(\out[3]_C_i_1_n_0 ));
  OBUF \out_OBUF[0]_inst 
       (.I(out_OBUF[0]),
        .O(out[0]));
  LUT3 #(
    .INIT(8'hB8)) 
    \out_OBUF[0]_inst_i_1 
       (.I0(\out_reg[0]_P_n_0 ),
        .I1(\out_reg[0]_LDC_n_0 ),
        .I2(\out_reg[0]_C_n_0 ),
        .O(out_OBUF[0]));
  OBUF \out_OBUF[1]_inst 
       (.I(out_OBUF[1]),
        .O(out[1]));
  LUT3 #(
    .INIT(8'hB8)) 
    \out_OBUF[1]_inst_i_1 
       (.I0(\out_reg[1]_P_n_0 ),
        .I1(\out_reg[1]_LDC_n_0 ),
        .I2(\out_reg[1]_C_n_0 ),
        .O(out_OBUF[1]));
  OBUF \out_OBUF[2]_inst 
       (.I(out_OBUF[2]),
        .O(out[2]));
  LUT3 #(
    .INIT(8'hB8)) 
    \out_OBUF[2]_inst_i_1 
       (.I0(\out_reg[2]_P_n_0 ),
        .I1(\out_reg[2]_LDC_n_0 ),
        .I2(\out_reg[2]_C_n_0 ),
        .O(out_OBUF[2]));
  OBUF \out_OBUF[3]_inst 
       (.I(out_OBUF[3]),
        .O(out[3]));
  LUT3 #(
    .INIT(8'hB8)) 
    \out_OBUF[3]_inst_i_1 
       (.I0(\out_reg[3]_P_n_0 ),
        .I1(\out_reg[3]_LDC_n_0 ),
        .I2(\out_reg[3]_C_n_0 ),
        .O(out_OBUF[3]));
  FDCE #(
    .INIT(1'b0)) 
    \out_reg[0]_C 
       (.C(CP_IBUF_BUFG),
        .CE(1'b1),
        .CLR(\out_reg[0]_LDC_i_2_n_0 ),
        .D(\out[0]_C_i_1_n_0 ),
        .Q(\out_reg[0]_C_n_0 ));
  (* XILINX_LEGACY_PRIM = "LDC" *) 
  LDCE #(
    .INIT(1'b0)) 
    \out_reg[0]_LDC 
       (.CLR(\out_reg[0]_LDC_i_2_n_0 ),
        .D(1'b1),
        .G(\out_reg[0]_LDC_i_1_n_0 ),
        .GE(1'b1),
        .Q(\out_reg[0]_LDC_n_0 ));
  LUT3 #(
    .INIT(8'h08)) 
    \out_reg[0]_LDC_i_1 
       (.I0(in_IBUF[0]),
        .I1(CLR_IBUF),
        .I2(Ld_IBUF),
        .O(\out_reg[0]_LDC_i_1_n_0 ));
  LUT3 #(
    .INIT(8'h1F)) 
    \out_reg[0]_LDC_i_2 
       (.I0(Ld_IBUF),
        .I1(in_IBUF[0]),
        .I2(CLR_IBUF),
        .O(\out_reg[0]_LDC_i_2_n_0 ));
  FDPE #(
    .INIT(1'b1)) 
    \out_reg[0]_P 
       (.C(CP_IBUF_BUFG),
        .CE(1'b1),
        .D(\out[0]_C_i_1_n_0 ),
        .PRE(\out_reg[0]_LDC_i_1_n_0 ),
        .Q(\out_reg[0]_P_n_0 ));
  FDCE #(
    .INIT(1'b0)) 
    \out_reg[1]_C 
       (.C(CP_IBUF_BUFG),
        .CE(1'b1),
        .CLR(\out_reg[1]_LDC_i_2_n_0 ),
        .D(\out[1]_C_i_1_n_0 ),
        .Q(\out_reg[1]_C_n_0 ));
  (* XILINX_LEGACY_PRIM = "LDC" *) 
  LDCE #(
    .INIT(1'b0)) 
    \out_reg[1]_LDC 
       (.CLR(\out_reg[1]_LDC_i_2_n_0 ),
        .D(1'b1),
        .G(\out_reg[1]_LDC_i_1_n_0 ),
        .GE(1'b1),
        .Q(\out_reg[1]_LDC_n_0 ));
  LUT3 #(
    .INIT(8'h08)) 
    \out_reg[1]_LDC_i_1 
       (.I0(in_IBUF[1]),
        .I1(CLR_IBUF),
        .I2(Ld_IBUF),
        .O(\out_reg[1]_LDC_i_1_n_0 ));
  LUT3 #(
    .INIT(8'h1F)) 
    \out_reg[1]_LDC_i_2 
       (.I0(Ld_IBUF),
        .I1(in_IBUF[1]),
        .I2(CLR_IBUF),
        .O(\out_reg[1]_LDC_i_2_n_0 ));
  FDPE #(
    .INIT(1'b1)) 
    \out_reg[1]_P 
       (.C(CP_IBUF_BUFG),
        .CE(1'b1),
        .D(\out[1]_C_i_1_n_0 ),
        .PRE(\out_reg[1]_LDC_i_1_n_0 ),
        .Q(\out_reg[1]_P_n_0 ));
  FDCE #(
    .INIT(1'b0)) 
    \out_reg[2]_C 
       (.C(CP_IBUF_BUFG),
        .CE(1'b1),
        .CLR(\out_reg[2]_LDC_i_2_n_0 ),
        .D(\out[2]_C_i_1_n_0 ),
        .Q(\out_reg[2]_C_n_0 ));
  (* XILINX_LEGACY_PRIM = "LDC" *) 
  LDCE #(
    .INIT(1'b0)) 
    \out_reg[2]_LDC 
       (.CLR(\out_reg[2]_LDC_i_2_n_0 ),
        .D(1'b1),
        .G(\out_reg[2]_LDC_i_1_n_0 ),
        .GE(1'b1),
        .Q(\out_reg[2]_LDC_n_0 ));
  LUT3 #(
    .INIT(8'h08)) 
    \out_reg[2]_LDC_i_1 
       (.I0(in_IBUF[2]),
        .I1(CLR_IBUF),
        .I2(Ld_IBUF),
        .O(\out_reg[2]_LDC_i_1_n_0 ));
  LUT3 #(
    .INIT(8'h1F)) 
    \out_reg[2]_LDC_i_2 
       (.I0(Ld_IBUF),
        .I1(in_IBUF[2]),
        .I2(CLR_IBUF),
        .O(\out_reg[2]_LDC_i_2_n_0 ));
  FDPE #(
    .INIT(1'b1)) 
    \out_reg[2]_P 
       (.C(CP_IBUF_BUFG),
        .CE(1'b1),
        .D(\out[2]_C_i_1_n_0 ),
        .PRE(\out_reg[2]_LDC_i_1_n_0 ),
        .Q(\out_reg[2]_P_n_0 ));
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
  LUT3 #(
    .INIT(8'h08)) 
    \out_reg[3]_LDC_i_1 
       (.I0(in_IBUF[3]),
        .I1(CLR_IBUF),
        .I2(Ld_IBUF),
        .O(\out_reg[3]_LDC_i_1_n_0 ));
  LUT3 #(
    .INIT(8'h1F)) 
    \out_reg[3]_LDC_i_2 
       (.I0(Ld_IBUF),
        .I1(in_IBUF[3]),
        .I2(CLR_IBUF),
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
