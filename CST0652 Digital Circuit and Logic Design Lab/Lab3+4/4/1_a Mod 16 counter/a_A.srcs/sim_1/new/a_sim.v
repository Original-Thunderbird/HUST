`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2018/06/11 15:12:05
// Design Name: 
// Module Name: a_sim
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


module a_sim;
	reg [3:0] in;
	reg CP,CLR,Ld,M;
	wire [3:0] out;
	wire Qcc;
	a uut (.in(in), .CP(CP), .CLR(CLR), .Ld(Ld), .M(M), .out(out), .Qcc(Qcc));
	always #10 CP=~CP;
	initial begin
		in = 4'b0010;
		M =1;
		CLR = 1;
		CP = 0;
		Ld = 0;
		#50;		Ld = 1;
		#50;		Ld = 0;
		#50;		Ld = 1;
		#500;       CLR=0;
		#50;        CLR=1;
        #1000;      M=0;
        #1000;      M=1;
	end  
endmodule