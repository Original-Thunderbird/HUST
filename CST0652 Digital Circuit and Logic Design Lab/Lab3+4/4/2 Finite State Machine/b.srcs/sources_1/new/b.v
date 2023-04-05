`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2018/06/13 13:39:30
// Design Name: 
// Module Name: b
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


module b(input in,clk,output reg out,output reg [1:3] p,n );
parameter A=0,B=1,C=2,D=3,E=4;
always@(negedge clk)	p<=n;
always @(p or in)
	begin
		case(p)
			A: begin 	 out<=0; 	  n<=in?B:A;	 end
			B: begin     out<=0;      n<=in?B:C;     end	
			C: begin     out<=0;      n<=in?D:A;     end
			D: begin     out<=0;      n<=in?E:C;	 end
			E: begin 	 out<=in?0:1; n<=in?B:A;     end
		endcase
	end
endmodule