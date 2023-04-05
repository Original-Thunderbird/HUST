`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2018/06/13 23:12:40
// Design Name: 
// Module Name: c_b
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


module c_b( input [2:0]a,[2:0]b, output F1,F2,F3 ); 
wire [1:3]y,n,s;
c C3(.A(a[2]),.B(b[2]),.F1(s[1]),.F2(s[2]),.F3(s[3])); 
c C2(.A(a[1]),.B(b[1]),.F1(n[1]),.F2(n[2]),.F3(n[3])); 
c C1(.A(a[0]),.B(b[0]),.F1(y[1]),.F2(y[2]),.F3(y[3])); 
assign F1 = s[1] | (s[2]&n[1]) | (s[2]&n[2]&y[1]);
assign F2 = s[2] & n[2] & y[2];
assign F3 = ~(F1|F2); 
endmodule 

module c( input A,B, output reg F1,F2,F3 ); 
initial 
	begin 
	F1 <=0; 
	F2 <=0; 
	F3 <=0; 
	end 
always @(A or B) 
	begin 
		if(A>B) 
			begin F1 <=1; F2 <=0; F3 <=0; end 
		else if(A==B) 
			begin F1 <=0; F2 <=1; F3 <=0; end 
		else 
			begin F1 <=0; F2 <=0; F3 <=1; end 
	end 
endmodule 