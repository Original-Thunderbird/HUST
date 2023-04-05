// Copyright 1986-2015 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2015.2 (lin64) Build 1266856 Fri Jun 26 16:35:25 MDT 2015
// Date        : Thu Jun 14 10:11:57 2018
// Host        : orithu-Lenovo-V480s running 64-bit Ubuntu 17.10
// Command     : write_verilog -mode funcsim -nolib -force -file /home/orithu/b/b.sim/sim_1/impl/func/b_sim_func_impl.v
// Design      : b
// Purpose     : This verilog netlist is a functional simulation representation of the design and should not be modified
//               or synthesized. This netlist cannot be used for SDF annotated simulation.
// Device      : xc7a100tcsg324-1
// --------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

(* A = "0" *) (* B = "1" *) (* C = "2" *) 
(* D = "3" *) (* E = "4" *) (* ECO_CHECKSUM = "be83e0ca" *) 
(* NotValidForBitStream *)
module b
   (in,
    clk,
    out);
  input in;
  input clk;
  output out;

  wire \FSM_sequential_n_state_reg[0]_i_1_n_0 ;
  wire \FSM_sequential_n_state_reg[1]_i_1_n_0 ;
  wire \FSM_sequential_n_state_reg[2]_i_1_n_0 ;
  wire clk;
  wire clk_IBUF;
  wire clk_IBUF_BUFG;
  wire in;
  wire in_IBUF;
  wire [1:3]n_state;
  wire out;
  wire out_OBUF;
  wire out_reg_i_1_n_0;
  wire out_reg_i_2_n_0;
  (* RTL_KEEP = "yes" *) wire [1:3]p_state;

  (* XILINX_LEGACY_PRIM = "LD" *) 
  LDCE #(
    .INIT(1'b0)) 
    \FSM_sequential_n_state_reg[0] 
       (.CLR(1'b0),
        .D(\FSM_sequential_n_state_reg[0]_i_1_n_0 ),
        .G(out_reg_i_2_n_0),
        .GE(1'b1),
        .Q(n_state[3]));
  LUT4 #(
    .INIT(16'h022A)) 
    \FSM_sequential_n_state_reg[0]_i_1 
       (.I0(in_IBUF),
        .I1(p_state[3]),
        .I2(p_state[1]),
        .I3(p_state[2]),
        .O(\FSM_sequential_n_state_reg[0]_i_1_n_0 ));
  (* XILINX_LEGACY_PRIM = "LD" *) 
  LDCE #(
    .INIT(1'b0)) 
    \FSM_sequential_n_state_reg[1] 
       (.CLR(1'b0),
        .D(\FSM_sequential_n_state_reg[1]_i_1_n_0 ),
        .G(out_reg_i_2_n_0),
        .GE(1'b1),
        .Q(n_state[2]));
  LUT4 #(
    .INIT(16'h0062)) 
    \FSM_sequential_n_state_reg[1]_i_1 
       (.I0(p_state[3]),
        .I1(in_IBUF),
        .I2(p_state[2]),
        .I3(p_state[1]),
        .O(\FSM_sequential_n_state_reg[1]_i_1_n_0 ));
  (* XILINX_LEGACY_PRIM = "LD" *) 
  LDCE #(
    .INIT(1'b0)) 
    \FSM_sequential_n_state_reg[2] 
       (.CLR(1'b0),
        .D(\FSM_sequential_n_state_reg[2]_i_1_n_0 ),
        .G(out_reg_i_2_n_0),
        .GE(1'b1),
        .Q(n_state[1]));
  LUT4 #(
    .INIT(16'h2000)) 
    \FSM_sequential_n_state_reg[2]_i_1 
       (.I0(p_state[2]),
        .I1(p_state[1]),
        .I2(in_IBUF),
        .I3(p_state[3]),
        .O(\FSM_sequential_n_state_reg[2]_i_1_n_0 ));
  (* KEEP = "yes" *) 
  FDRE #(
    .INIT(1'b0),
    .IS_C_INVERTED(1'b1)) 
    \FSM_sequential_p_state_reg[0] 
       (.C(clk_IBUF_BUFG),
        .CE(1'b1),
        .D(n_state[3]),
        .Q(p_state[3]),
        .R(1'b0));
  (* KEEP = "yes" *) 
  FDRE #(
    .INIT(1'b0),
    .IS_C_INVERTED(1'b1)) 
    \FSM_sequential_p_state_reg[1] 
       (.C(clk_IBUF_BUFG),
        .CE(1'b1),
        .D(n_state[2]),
        .Q(p_state[2]),
        .R(1'b0));
  (* KEEP = "yes" *) 
  FDRE #(
    .INIT(1'b0),
    .IS_C_INVERTED(1'b1)) 
    \FSM_sequential_p_state_reg[2] 
       (.C(clk_IBUF_BUFG),
        .CE(1'b1),
        .D(n_state[1]),
        .Q(p_state[1]),
        .R(1'b0));
  BUFG clk_IBUF_BUFG_inst
       (.I(clk_IBUF),
        .O(clk_IBUF_BUFG));
  IBUF clk_IBUF_inst
       (.I(clk),
        .O(clk_IBUF));
  IBUF in_IBUF_inst
       (.I(in),
        .O(in_IBUF));
  OBUF out_OBUF_inst
       (.I(out_OBUF),
        .O(out));
  (* XILINX_LEGACY_PRIM = "LD" *) 
  LDCE #(
    .INIT(1'b0)) 
    out_reg
       (.CLR(1'b0),
        .D(out_reg_i_1_n_0),
        .G(out_reg_i_2_n_0),
        .GE(1'b1),
        .Q(out_OBUF));
  LUT4 #(
    .INIT(16'h0010)) 
    out_reg_i_1
       (.I0(p_state[3]),
        .I1(p_state[2]),
        .I2(p_state[1]),
        .I3(in_IBUF),
        .O(out_reg_i_1_n_0));
  LUT3 #(
    .INIT(8'h57)) 
    out_reg_i_2
       (.I0(p_state[1]),
        .I1(p_state[2]),
        .I2(p_state[3]),
        .O(out_reg_i_2_n_0));
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
