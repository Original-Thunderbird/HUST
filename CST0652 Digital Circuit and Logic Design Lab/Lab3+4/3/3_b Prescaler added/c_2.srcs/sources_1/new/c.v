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
module c(input sig_nsyn,cp,output q1,q2,sig_syn);
wire clk; 
reg q1;
reg q2;
reg sig_syn;
div dv1(cp,clk);
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

module div(input cp,output reg clk);
 parameter t=10000000;
       integer count;
       initial begin
           count <= 0;
           clk <= 0;
       end
       always @(posedge cp) begin
           count = count + 1;
           if(count == t) begin
               clk = ~clk;
               count = 0;
           end
       end
endmodule