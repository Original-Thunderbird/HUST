`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2018/06/14 13:25:59
// Design Name: 
// Module Name: a_b_sim
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

module a_b_sim;
	reg M,CP;
	wire Qcc;
	wire [3:0] out;
	a_b uut (.Qcc(Qcc), .out(out), .M(M), .CP(CP));
   always #10 CP=~CP;
	initial begin
		M = 1;
		CP = 0;
        #400;      M=0;
        #400;      M=1;
	end
endmodule