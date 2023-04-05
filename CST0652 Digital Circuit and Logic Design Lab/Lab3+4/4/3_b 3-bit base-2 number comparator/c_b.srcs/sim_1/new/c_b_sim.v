`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2018/06/13 23:15:25
// Design Name: 
// Module Name: c_b_sim
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


module c_b_sim; 
reg [2:0]a,b; 
wire F1,F2,F3; 
c_b comp(.a(a),.b(b),.F1(F1),.F2(F2),.F3(F3)); 
initial 
	begin 
	a=3'b010;	   b=3'b010;
	#50 a=3'b010;  b=3'b110;
	#50 a=3'b010;  b=3'b100;
	#50 a=3'b100;  b=3'b111;
	#50 a=3'b001;  b=3'b000;    
	end 
endmodule