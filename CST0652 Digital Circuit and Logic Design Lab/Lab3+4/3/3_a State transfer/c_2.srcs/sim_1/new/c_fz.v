`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2018/06/07 16:33:07
// Design Name: 
// Module Name: c_fz
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module c_fz;
reg IN;
reg CLK;
wire Q1;
wire Q2;
wire OUT;
initial  
    begin
        IN<=1;
        CLK<=0;
    end

always  #450 IN=~IN;

always
    begin
        #300 CLK =~CLK;
//        CLK_IN<=CLK_OUT;
    end

c uut(.clk(CLK),.sig_nsyn(IN),.q1(Q1),.q2(Q2),.sig_syn(OUT));
endmodule
