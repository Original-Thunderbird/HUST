`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2018/06/07 16:23:19
// Design Name: 
// Module Name: c
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


module c(
    input sig_nsyn,
    input clk,
    output q1,
    output q2,
    output sig_syn
    );
reg q1;
reg q2;
reg sig_syn;
initial
    begin
        q1=0;
        q2=0;
        sig_syn=0;
    end
always@(posedge clk)     q1<=sig_nsyn;
always@(posedge clk)    q2<=q1;
always@(posedge clk)    sig_syn<=q2;
endmodule
