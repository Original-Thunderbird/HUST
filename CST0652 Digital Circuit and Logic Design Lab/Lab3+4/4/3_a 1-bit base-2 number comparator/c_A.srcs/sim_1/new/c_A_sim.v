`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2018/06/12 09:05:35
// Design Name: 
// Module Name: c_A_sim
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
module c_A_sim;
	reg a,b;
	wire F1,F2,F3;
	c_A uut (.a(a), .b(b), .F1(F1), .F2(F2), .F3(F3));
	initial begin
		a = 1;
        b = 0;
		#100  a=0; b=0;
		#100  a=0; b=1;
		#100  a=1; b=1;
	end
endmodule