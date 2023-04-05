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
reg IN,CP;
wire Q1,Q2, OUT;
initial  
    begin
        IN<=1;
        CP<=0;
    end
always  #30000000 IN=~IN;
always #4 CP =~CP;
c uut(.cp(CP),.sig_nsyn(IN),.q1(Q1),.q2(Q2),.sig_syn(OUT));
endmodule