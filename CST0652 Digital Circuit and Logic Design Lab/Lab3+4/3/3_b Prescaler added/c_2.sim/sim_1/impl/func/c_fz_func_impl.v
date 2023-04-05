// Copyright 1986-2015 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2015.2 (lin64) Build 1266856 Fri Jun 26 16:35:25 MDT 2015
// Date        : Sat Jun  9 00:00:28 2018
// Host        : orithu-Lenovo-V480s running 64-bit Ubuntu 17.10
// Command     : write_verilog -mode funcsim -nolib -force -file
//               /home/orithu/桌面/第三次/c_2/c_2.sim/sim_1/impl/func/c_fz_func_impl.v
// Design      : c
// Purpose     : This verilog netlist is a functional simulation representation of the design and should not be modified
//               or synthesized. This netlist cannot be used for SDF annotated simulation.
// Device      : xc7a100tcsg324-1
// --------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

(* ECO_CHECKSUM = "33cd2f8a" *) 
(* NotValidForBitStream *)
module c
   (sig_nsyn,
    cp,
    q1,
    q2,
    sig_syn);
  input sig_nsyn;
  input cp;
  output q1;
  output q2;
  output sig_syn;

  wire clk;
  wire cp;
  wire cp_IBUF;
  wire cp_IBUF_BUFG;
  wire q1;
  wire q1_OBUF;
  wire q2;
  wire q2_OBUF;
  wire sig_nsyn;
  wire sig_nsyn_IBUF;
  wire sig_syn;
  wire sig_syn_OBUF;

  BUFG cp_IBUF_BUFG_inst
       (.I(cp_IBUF),
        .O(cp_IBUF_BUFG));
  IBUF cp_IBUF_inst
       (.I(cp),
        .O(cp_IBUF));
  div dv1
       (.clk(clk),
        .cp(cp_IBUF_BUFG));
  OBUF q1_OBUF_inst
       (.I(q1_OBUF),
        .O(q1));
  FDRE #(
    .INIT(1'b0)) 
    q1_reg
       (.C(clk),
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
       (.C(clk),
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
       (.C(clk),
        .CE(1'b1),
        .D(q2_OBUF),
        .Q(sig_syn_OBUF),
        .R(1'b0));
endmodule

module div
   (clk,
    cp);
  output clk;
  input cp;

  wire clk;
  wire clk_i_1_n_0;
  wire \count[0]_i_10_n_0 ;
  wire \count[0]_i_16_n_0 ;
  wire \count[0]_i_17_n_0 ;
  wire \count[0]_i_1_n_0 ;
  wire \count[0]_i_3_n_0 ;
  wire \count[0]_i_4_n_0 ;
  wire \count[0]_i_5_n_0 ;
  wire \count[0]_i_6_n_0 ;
  wire [31:0]count_reg;
  wire \count_reg[0]_i_11_n_0 ;
  wire \count_reg[0]_i_11_n_4 ;
  wire \count_reg[0]_i_11_n_5 ;
  wire \count_reg[0]_i_11_n_6 ;
  wire \count_reg[0]_i_11_n_7 ;
  wire \count_reg[0]_i_12_n_0 ;
  wire \count_reg[0]_i_12_n_4 ;
  wire \count_reg[0]_i_12_n_5 ;
  wire \count_reg[0]_i_12_n_6 ;
  wire \count_reg[0]_i_12_n_7 ;
  wire \count_reg[0]_i_13_n_0 ;
  wire \count_reg[0]_i_13_n_4 ;
  wire \count_reg[0]_i_13_n_5 ;
  wire \count_reg[0]_i_13_n_6 ;
  wire \count_reg[0]_i_13_n_7 ;
  wire \count_reg[0]_i_14_n_0 ;
  wire \count_reg[0]_i_14_n_4 ;
  wire \count_reg[0]_i_14_n_5 ;
  wire \count_reg[0]_i_14_n_6 ;
  wire \count_reg[0]_i_14_n_7 ;
  wire \count_reg[0]_i_15_n_0 ;
  wire \count_reg[0]_i_15_n_4 ;
  wire \count_reg[0]_i_15_n_5 ;
  wire \count_reg[0]_i_15_n_6 ;
  wire \count_reg[0]_i_15_n_7 ;
  wire \count_reg[0]_i_18_n_5 ;
  wire \count_reg[0]_i_18_n_6 ;
  wire \count_reg[0]_i_18_n_7 ;
  wire \count_reg[0]_i_19_n_0 ;
  wire \count_reg[0]_i_19_n_4 ;
  wire \count_reg[0]_i_19_n_5 ;
  wire \count_reg[0]_i_19_n_6 ;
  wire \count_reg[0]_i_19_n_7 ;
  wire \count_reg[0]_i_2_n_0 ;
  wire \count_reg[0]_i_2_n_4 ;
  wire \count_reg[0]_i_2_n_5 ;
  wire \count_reg[0]_i_2_n_6 ;
  wire \count_reg[0]_i_2_n_7 ;
  wire \count_reg[0]_i_40_n_0 ;
  wire \count_reg[0]_i_40_n_4 ;
  wire \count_reg[0]_i_40_n_5 ;
  wire \count_reg[0]_i_40_n_6 ;
  wire \count_reg[0]_i_40_n_7 ;
  wire \count_reg[12]_i_1_n_0 ;
  wire \count_reg[12]_i_1_n_4 ;
  wire \count_reg[12]_i_1_n_5 ;
  wire \count_reg[12]_i_1_n_6 ;
  wire \count_reg[12]_i_1_n_7 ;
  wire \count_reg[16]_i_1_n_0 ;
  wire \count_reg[16]_i_1_n_4 ;
  wire \count_reg[16]_i_1_n_5 ;
  wire \count_reg[16]_i_1_n_6 ;
  wire \count_reg[16]_i_1_n_7 ;
  wire \count_reg[20]_i_1_n_0 ;
  wire \count_reg[20]_i_1_n_4 ;
  wire \count_reg[20]_i_1_n_5 ;
  wire \count_reg[20]_i_1_n_6 ;
  wire \count_reg[20]_i_1_n_7 ;
  wire \count_reg[24]_i_1_n_0 ;
  wire \count_reg[24]_i_1_n_4 ;
  wire \count_reg[24]_i_1_n_5 ;
  wire \count_reg[24]_i_1_n_6 ;
  wire \count_reg[24]_i_1_n_7 ;
  wire \count_reg[28]_i_1_n_4 ;
  wire \count_reg[28]_i_1_n_5 ;
  wire \count_reg[28]_i_1_n_6 ;
  wire \count_reg[28]_i_1_n_7 ;
  wire \count_reg[4]_i_1_n_0 ;
  wire \count_reg[4]_i_1_n_4 ;
  wire \count_reg[4]_i_1_n_5 ;
  wire \count_reg[4]_i_1_n_6 ;
  wire \count_reg[4]_i_1_n_7 ;
  wire \count_reg[8]_i_1_n_0 ;
  wire \count_reg[8]_i_1_n_4 ;
  wire \count_reg[8]_i_1_n_5 ;
  wire \count_reg[8]_i_1_n_6 ;
  wire \count_reg[8]_i_1_n_7 ;
  wire cp;
  wire [2:0]\NLW_count_reg[0]_i_11_CO_UNCONNECTED ;
  wire [2:0]\NLW_count_reg[0]_i_12_CO_UNCONNECTED ;
  wire [2:0]\NLW_count_reg[0]_i_13_CO_UNCONNECTED ;
  wire [2:0]\NLW_count_reg[0]_i_14_CO_UNCONNECTED ;
  wire [2:0]\NLW_count_reg[0]_i_15_CO_UNCONNECTED ;
  wire [3:0]\NLW_count_reg[0]_i_18_CO_UNCONNECTED ;
  wire [3:3]\NLW_count_reg[0]_i_18_O_UNCONNECTED ;
  wire [2:0]\NLW_count_reg[0]_i_19_CO_UNCONNECTED ;
  wire [2:0]\NLW_count_reg[0]_i_2_CO_UNCONNECTED ;
  wire [2:0]\NLW_count_reg[0]_i_40_CO_UNCONNECTED ;
  wire [2:0]\NLW_count_reg[12]_i_1_CO_UNCONNECTED ;
  wire [2:0]\NLW_count_reg[16]_i_1_CO_UNCONNECTED ;
  wire [2:0]\NLW_count_reg[20]_i_1_CO_UNCONNECTED ;
  wire [2:0]\NLW_count_reg[24]_i_1_CO_UNCONNECTED ;
  wire [3:0]\NLW_count_reg[28]_i_1_CO_UNCONNECTED ;
  wire [2:0]\NLW_count_reg[4]_i_1_CO_UNCONNECTED ;
  wire [2:0]\NLW_count_reg[8]_i_1_CO_UNCONNECTED ;

  LUT5 #(
    .INIT(32'h6AAAAAAA)) 
    clk_i_1
       (.I0(clk),
        .I1(\count[0]_i_6_n_0 ),
        .I2(\count[0]_i_5_n_0 ),
        .I3(\count[0]_i_4_n_0 ),
        .I4(\count[0]_i_3_n_0 ),
        .O(clk_i_1_n_0));
  FDRE #(
    .INIT(1'b0)) 
    clk_reg
       (.C(cp),
        .CE(1'b1),
        .D(clk_i_1_n_0),
        .Q(clk),
        .R(1'b0));
  LUT4 #(
    .INIT(16'h8000)) 
    \count[0]_i_1 
       (.I0(\count[0]_i_3_n_0 ),
        .I1(\count[0]_i_4_n_0 ),
        .I2(\count[0]_i_5_n_0 ),
        .I3(\count[0]_i_6_n_0 ),
        .O(\count[0]_i_1_n_0 ));
  LUT1 #(
    .INIT(2'h1)) 
    \count[0]_i_10 
       (.I0(count_reg[0]),
        .O(\count[0]_i_10_n_0 ));
  LUT4 #(
    .INIT(16'h0001)) 
    \count[0]_i_16 
       (.I0(\count_reg[0]_i_14_n_4 ),
        .I1(\count_reg[0]_i_14_n_5 ),
        .I2(\count_reg[0]_i_18_n_6 ),
        .I3(\count_reg[0]_i_18_n_7 ),
        .O(\count[0]_i_16_n_0 ));
  LUT6 #(
    .INIT(64'h0000000000100000)) 
    \count[0]_i_17 
       (.I0(\count_reg[0]_i_19_n_6 ),
        .I1(\count_reg[0]_i_19_n_7 ),
        .I2(\count_reg[0]_i_40_n_7 ),
        .I3(\count_reg[0]_i_40_n_6 ),
        .I4(\count_reg[0]_i_40_n_5 ),
        .I5(\count_reg[0]_i_40_n_4 ),
        .O(\count[0]_i_17_n_0 ));
  LUT6 #(
    .INIT(64'h0000000000000001)) 
    \count[0]_i_3 
       (.I0(\count_reg[0]_i_11_n_6 ),
        .I1(\count_reg[0]_i_11_n_5 ),
        .I2(\count_reg[0]_i_12_n_7 ),
        .I3(\count_reg[0]_i_12_n_6 ),
        .I4(\count_reg[0]_i_11_n_4 ),
        .I5(\count_reg[0]_i_11_n_7 ),
        .O(\count[0]_i_3_n_0 ));
  LUT6 #(
    .INIT(64'h0000000000000001)) 
    \count[0]_i_4 
       (.I0(\count_reg[0]_i_12_n_5 ),
        .I1(\count_reg[0]_i_12_n_4 ),
        .I2(\count_reg[0]_i_13_n_7 ),
        .I3(\count_reg[0]_i_13_n_5 ),
        .I4(\count_reg[0]_i_13_n_4 ),
        .I5(\count_reg[0]_i_13_n_6 ),
        .O(\count[0]_i_4_n_0 ));
  LUT6 #(
    .INIT(64'h0000000000000001)) 
    \count[0]_i_5 
       (.I0(\count_reg[0]_i_14_n_7 ),
        .I1(\count_reg[0]_i_14_n_6 ),
        .I2(\count_reg[0]_i_15_n_6 ),
        .I3(\count_reg[0]_i_15_n_4 ),
        .I4(\count_reg[0]_i_15_n_7 ),
        .I5(\count_reg[0]_i_15_n_5 ),
        .O(\count[0]_i_5_n_0 ));
  LUT6 #(
    .INIT(64'h0000000000000800)) 
    \count[0]_i_6 
       (.I0(\count[0]_i_16_n_0 ),
        .I1(\count[0]_i_17_n_0 ),
        .I2(\count_reg[0]_i_18_n_5 ),
        .I3(count_reg[0]),
        .I4(\count_reg[0]_i_19_n_5 ),
        .I5(\count_reg[0]_i_19_n_4 ),
        .O(\count[0]_i_6_n_0 ));
  FDRE #(
    .INIT(1'b0)) 
    \count_reg[0] 
       (.C(cp),
        .CE(1'b1),
        .D(\count_reg[0]_i_2_n_7 ),
        .Q(count_reg[0]),
        .R(\count[0]_i_1_n_0 ));
  CARRY4 \count_reg[0]_i_11 
       (.CI(\count_reg[0]_i_12_n_0 ),
        .CO({\count_reg[0]_i_11_n_0 ,\NLW_count_reg[0]_i_11_CO_UNCONNECTED [2:0]}),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O({\count_reg[0]_i_11_n_4 ,\count_reg[0]_i_11_n_5 ,\count_reg[0]_i_11_n_6 ,\count_reg[0]_i_11_n_7 }),
        .S(count_reg[20:17]));
  CARRY4 \count_reg[0]_i_12 
       (.CI(\count_reg[0]_i_13_n_0 ),
        .CO({\count_reg[0]_i_12_n_0 ,\NLW_count_reg[0]_i_12_CO_UNCONNECTED [2:0]}),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O({\count_reg[0]_i_12_n_4 ,\count_reg[0]_i_12_n_5 ,\count_reg[0]_i_12_n_6 ,\count_reg[0]_i_12_n_7 }),
        .S(count_reg[16:13]));
  CARRY4 \count_reg[0]_i_13 
       (.CI(\count_reg[0]_i_19_n_0 ),
        .CO({\count_reg[0]_i_13_n_0 ,\NLW_count_reg[0]_i_13_CO_UNCONNECTED [2:0]}),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O({\count_reg[0]_i_13_n_4 ,\count_reg[0]_i_13_n_5 ,\count_reg[0]_i_13_n_6 ,\count_reg[0]_i_13_n_7 }),
        .S(count_reg[12:9]));
  CARRY4 \count_reg[0]_i_14 
       (.CI(\count_reg[0]_i_15_n_0 ),
        .CO({\count_reg[0]_i_14_n_0 ,\NLW_count_reg[0]_i_14_CO_UNCONNECTED [2:0]}),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O({\count_reg[0]_i_14_n_4 ,\count_reg[0]_i_14_n_5 ,\count_reg[0]_i_14_n_6 ,\count_reg[0]_i_14_n_7 }),
        .S(count_reg[28:25]));
  CARRY4 \count_reg[0]_i_15 
       (.CI(\count_reg[0]_i_11_n_0 ),
        .CO({\count_reg[0]_i_15_n_0 ,\NLW_count_reg[0]_i_15_CO_UNCONNECTED [2:0]}),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O({\count_reg[0]_i_15_n_4 ,\count_reg[0]_i_15_n_5 ,\count_reg[0]_i_15_n_6 ,\count_reg[0]_i_15_n_7 }),
        .S(count_reg[24:21]));
  CARRY4 \count_reg[0]_i_18 
       (.CI(\count_reg[0]_i_14_n_0 ),
        .CO(\NLW_count_reg[0]_i_18_CO_UNCONNECTED [3:0]),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O({\NLW_count_reg[0]_i_18_O_UNCONNECTED [3],\count_reg[0]_i_18_n_5 ,\count_reg[0]_i_18_n_6 ,\count_reg[0]_i_18_n_7 }),
        .S({1'b0,count_reg[31:29]}));
  CARRY4 \count_reg[0]_i_19 
       (.CI(\count_reg[0]_i_40_n_0 ),
        .CO({\count_reg[0]_i_19_n_0 ,\NLW_count_reg[0]_i_19_CO_UNCONNECTED [2:0]}),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O({\count_reg[0]_i_19_n_4 ,\count_reg[0]_i_19_n_5 ,\count_reg[0]_i_19_n_6 ,\count_reg[0]_i_19_n_7 }),
        .S(count_reg[8:5]));
  CARRY4 \count_reg[0]_i_2 
       (.CI(1'b0),
        .CO({\count_reg[0]_i_2_n_0 ,\NLW_count_reg[0]_i_2_CO_UNCONNECTED [2:0]}),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b1}),
        .O({\count_reg[0]_i_2_n_4 ,\count_reg[0]_i_2_n_5 ,\count_reg[0]_i_2_n_6 ,\count_reg[0]_i_2_n_7 }),
        .S({count_reg[3:1],\count[0]_i_10_n_0 }));
  CARRY4 \count_reg[0]_i_40 
       (.CI(1'b0),
        .CO({\count_reg[0]_i_40_n_0 ,\NLW_count_reg[0]_i_40_CO_UNCONNECTED [2:0]}),
        .CYINIT(count_reg[0]),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O({\count_reg[0]_i_40_n_4 ,\count_reg[0]_i_40_n_5 ,\count_reg[0]_i_40_n_6 ,\count_reg[0]_i_40_n_7 }),
        .S(count_reg[4:1]));
  FDRE #(
    .INIT(1'b0)) 
    \count_reg[10] 
       (.C(cp),
        .CE(1'b1),
        .D(\count_reg[8]_i_1_n_5 ),
        .Q(count_reg[10]),
        .R(\count[0]_i_1_n_0 ));
  FDRE #(
    .INIT(1'b0)) 
    \count_reg[11] 
       (.C(cp),
        .CE(1'b1),
        .D(\count_reg[8]_i_1_n_4 ),
        .Q(count_reg[11]),
        .R(\count[0]_i_1_n_0 ));
  FDRE #(
    .INIT(1'b0)) 
    \count_reg[12] 
       (.C(cp),
        .CE(1'b1),
        .D(\count_reg[12]_i_1_n_7 ),
        .Q(count_reg[12]),
        .R(\count[0]_i_1_n_0 ));
  CARRY4 \count_reg[12]_i_1 
       (.CI(\count_reg[8]_i_1_n_0 ),
        .CO({\count_reg[12]_i_1_n_0 ,\NLW_count_reg[12]_i_1_CO_UNCONNECTED [2:0]}),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O({\count_reg[12]_i_1_n_4 ,\count_reg[12]_i_1_n_5 ,\count_reg[12]_i_1_n_6 ,\count_reg[12]_i_1_n_7 }),
        .S(count_reg[15:12]));
  FDRE #(
    .INIT(1'b0)) 
    \count_reg[13] 
       (.C(cp),
        .CE(1'b1),
        .D(\count_reg[12]_i_1_n_6 ),
        .Q(count_reg[13]),
        .R(\count[0]_i_1_n_0 ));
  FDRE #(
    .INIT(1'b0)) 
    \count_reg[14] 
       (.C(cp),
        .CE(1'b1),
        .D(\count_reg[12]_i_1_n_5 ),
        .Q(count_reg[14]),
        .R(\count[0]_i_1_n_0 ));
  FDRE #(
    .INIT(1'b0)) 
    \count_reg[15] 
       (.C(cp),
        .CE(1'b1),
        .D(\count_reg[12]_i_1_n_4 ),
        .Q(count_reg[15]),
        .R(\count[0]_i_1_n_0 ));
  FDRE #(
    .INIT(1'b0)) 
    \count_reg[16] 
       (.C(cp),
        .CE(1'b1),
        .D(\count_reg[16]_i_1_n_7 ),
        .Q(count_reg[16]),
        .R(\count[0]_i_1_n_0 ));
  CARRY4 \count_reg[16]_i_1 
       (.CI(\count_reg[12]_i_1_n_0 ),
        .CO({\count_reg[16]_i_1_n_0 ,\NLW_count_reg[16]_i_1_CO_UNCONNECTED [2:0]}),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O({\count_reg[16]_i_1_n_4 ,\count_reg[16]_i_1_n_5 ,\count_reg[16]_i_1_n_6 ,\count_reg[16]_i_1_n_7 }),
        .S(count_reg[19:16]));
  FDRE #(
    .INIT(1'b0)) 
    \count_reg[17] 
       (.C(cp),
        .CE(1'b1),
        .D(\count_reg[16]_i_1_n_6 ),
        .Q(count_reg[17]),
        .R(\count[0]_i_1_n_0 ));
  FDRE #(
    .INIT(1'b0)) 
    \count_reg[18] 
       (.C(cp),
        .CE(1'b1),
        .D(\count_reg[16]_i_1_n_5 ),
        .Q(count_reg[18]),
        .R(\count[0]_i_1_n_0 ));
  FDRE #(
    .INIT(1'b0)) 
    \count_reg[19] 
       (.C(cp),
        .CE(1'b1),
        .D(\count_reg[16]_i_1_n_4 ),
        .Q(count_reg[19]),
        .R(\count[0]_i_1_n_0 ));
  FDRE #(
    .INIT(1'b0)) 
    \count_reg[1] 
       (.C(cp),
        .CE(1'b1),
        .D(\count_reg[0]_i_2_n_6 ),
        .Q(count_reg[1]),
        .R(\count[0]_i_1_n_0 ));
  FDRE #(
    .INIT(1'b0)) 
    \count_reg[20] 
       (.C(cp),
        .CE(1'b1),
        .D(\count_reg[20]_i_1_n_7 ),
        .Q(count_reg[20]),
        .R(\count[0]_i_1_n_0 ));
  CARRY4 \count_reg[20]_i_1 
       (.CI(\count_reg[16]_i_1_n_0 ),
        .CO({\count_reg[20]_i_1_n_0 ,\NLW_count_reg[20]_i_1_CO_UNCONNECTED [2:0]}),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O({\count_reg[20]_i_1_n_4 ,\count_reg[20]_i_1_n_5 ,\count_reg[20]_i_1_n_6 ,\count_reg[20]_i_1_n_7 }),
        .S(count_reg[23:20]));
  FDRE #(
    .INIT(1'b0)) 
    \count_reg[21] 
       (.C(cp),
        .CE(1'b1),
        .D(\count_reg[20]_i_1_n_6 ),
        .Q(count_reg[21]),
        .R(\count[0]_i_1_n_0 ));
  FDRE #(
    .INIT(1'b0)) 
    \count_reg[22] 
       (.C(cp),
        .CE(1'b1),
        .D(\count_reg[20]_i_1_n_5 ),
        .Q(count_reg[22]),
        .R(\count[0]_i_1_n_0 ));
  FDRE #(
    .INIT(1'b0)) 
    \count_reg[23] 
       (.C(cp),
        .CE(1'b1),
        .D(\count_reg[20]_i_1_n_4 ),
        .Q(count_reg[23]),
        .R(\count[0]_i_1_n_0 ));
  FDRE #(
    .INIT(1'b0)) 
    \count_reg[24] 
       (.C(cp),
        .CE(1'b1),
        .D(\count_reg[24]_i_1_n_7 ),
        .Q(count_reg[24]),
        .R(\count[0]_i_1_n_0 ));
  CARRY4 \count_reg[24]_i_1 
       (.CI(\count_reg[20]_i_1_n_0 ),
        .CO({\count_reg[24]_i_1_n_0 ,\NLW_count_reg[24]_i_1_CO_UNCONNECTED [2:0]}),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O({\count_reg[24]_i_1_n_4 ,\count_reg[24]_i_1_n_5 ,\count_reg[24]_i_1_n_6 ,\count_reg[24]_i_1_n_7 }),
        .S(count_reg[27:24]));
  FDRE #(
    .INIT(1'b0)) 
    \count_reg[25] 
       (.C(cp),
        .CE(1'b1),
        .D(\count_reg[24]_i_1_n_6 ),
        .Q(count_reg[25]),
        .R(\count[0]_i_1_n_0 ));
  FDRE #(
    .INIT(1'b0)) 
    \count_reg[26] 
       (.C(cp),
        .CE(1'b1),
        .D(\count_reg[24]_i_1_n_5 ),
        .Q(count_reg[26]),
        .R(\count[0]_i_1_n_0 ));
  FDRE #(
    .INIT(1'b0)) 
    \count_reg[27] 
       (.C(cp),
        .CE(1'b1),
        .D(\count_reg[24]_i_1_n_4 ),
        .Q(count_reg[27]),
        .R(\count[0]_i_1_n_0 ));
  FDRE #(
    .INIT(1'b0)) 
    \count_reg[28] 
       (.C(cp),
        .CE(1'b1),
        .D(\count_reg[28]_i_1_n_7 ),
        .Q(count_reg[28]),
        .R(\count[0]_i_1_n_0 ));
  CARRY4 \count_reg[28]_i_1 
       (.CI(\count_reg[24]_i_1_n_0 ),
        .CO(\NLW_count_reg[28]_i_1_CO_UNCONNECTED [3:0]),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O({\count_reg[28]_i_1_n_4 ,\count_reg[28]_i_1_n_5 ,\count_reg[28]_i_1_n_6 ,\count_reg[28]_i_1_n_7 }),
        .S(count_reg[31:28]));
  FDRE #(
    .INIT(1'b0)) 
    \count_reg[29] 
       (.C(cp),
        .CE(1'b1),
        .D(\count_reg[28]_i_1_n_6 ),
        .Q(count_reg[29]),
        .R(\count[0]_i_1_n_0 ));
  FDRE #(
    .INIT(1'b0)) 
    \count_reg[2] 
       (.C(cp),
        .CE(1'b1),
        .D(\count_reg[0]_i_2_n_5 ),
        .Q(count_reg[2]),
        .R(\count[0]_i_1_n_0 ));
  FDRE #(
    .INIT(1'b0)) 
    \count_reg[30] 
       (.C(cp),
        .CE(1'b1),
        .D(\count_reg[28]_i_1_n_5 ),
        .Q(count_reg[30]),
        .R(\count[0]_i_1_n_0 ));
  FDRE #(
    .INIT(1'b0)) 
    \count_reg[31] 
       (.C(cp),
        .CE(1'b1),
        .D(\count_reg[28]_i_1_n_4 ),
        .Q(count_reg[31]),
        .R(\count[0]_i_1_n_0 ));
  FDRE #(
    .INIT(1'b0)) 
    \count_reg[3] 
       (.C(cp),
        .CE(1'b1),
        .D(\count_reg[0]_i_2_n_4 ),
        .Q(count_reg[3]),
        .R(\count[0]_i_1_n_0 ));
  FDRE #(
    .INIT(1'b0)) 
    \count_reg[4] 
       (.C(cp),
        .CE(1'b1),
        .D(\count_reg[4]_i_1_n_7 ),
        .Q(count_reg[4]),
        .R(\count[0]_i_1_n_0 ));
  CARRY4 \count_reg[4]_i_1 
       (.CI(\count_reg[0]_i_2_n_0 ),
        .CO({\count_reg[4]_i_1_n_0 ,\NLW_count_reg[4]_i_1_CO_UNCONNECTED [2:0]}),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O({\count_reg[4]_i_1_n_4 ,\count_reg[4]_i_1_n_5 ,\count_reg[4]_i_1_n_6 ,\count_reg[4]_i_1_n_7 }),
        .S(count_reg[7:4]));
  FDRE #(
    .INIT(1'b0)) 
    \count_reg[5] 
       (.C(cp),
        .CE(1'b1),
        .D(\count_reg[4]_i_1_n_6 ),
        .Q(count_reg[5]),
        .R(\count[0]_i_1_n_0 ));
  FDRE #(
    .INIT(1'b0)) 
    \count_reg[6] 
       (.C(cp),
        .CE(1'b1),
        .D(\count_reg[4]_i_1_n_5 ),
        .Q(count_reg[6]),
        .R(\count[0]_i_1_n_0 ));
  FDRE #(
    .INIT(1'b0)) 
    \count_reg[7] 
       (.C(cp),
        .CE(1'b1),
        .D(\count_reg[4]_i_1_n_4 ),
        .Q(count_reg[7]),
        .R(\count[0]_i_1_n_0 ));
  FDRE #(
    .INIT(1'b0)) 
    \count_reg[8] 
       (.C(cp),
        .CE(1'b1),
        .D(\count_reg[8]_i_1_n_7 ),
        .Q(count_reg[8]),
        .R(\count[0]_i_1_n_0 ));
  CARRY4 \count_reg[8]_i_1 
       (.CI(\count_reg[4]_i_1_n_0 ),
        .CO({\count_reg[8]_i_1_n_0 ,\NLW_count_reg[8]_i_1_CO_UNCONNECTED [2:0]}),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O({\count_reg[8]_i_1_n_4 ,\count_reg[8]_i_1_n_5 ,\count_reg[8]_i_1_n_6 ,\count_reg[8]_i_1_n_7 }),
        .S(count_reg[11:8]));
  FDRE #(
    .INIT(1'b0)) 
    \count_reg[9] 
       (.C(cp),
        .CE(1'b1),
        .D(\count_reg[8]_i_1_n_6 ),
        .Q(count_reg[9]),
        .R(\count[0]_i_1_n_0 ));
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
