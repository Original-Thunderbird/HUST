`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2018/06/07 15:23:37
// Design Name: 
// Module Name: b_sim
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


module b_sim;
reg X;
wire Z,q1,q2,q3,q4;
b uut (.in(X),.out(Z),.q1(q1),.q2(q2),.q3(q3),.q4(q4));
initial  X = 0;
always #10 X=~X;
endmodule