`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2018/06/12 09:03:48
// Design Name: 
// Module Name: c_A
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
module c_A(a,b,F1,F2,F3);
	input a,b;
	output F1,F2,F3;
	reg F1,F2,F3;
	always@(a,b)
	if(a>b)
        begin  F1=1;  F2=0;  F3=0;  end
	else if(a==b)
        begin  F1=0;  F2=1;  F3=0;  end
	else if(a<b)
        begin  F1=0;  F2=0;  F3=1;  end
endmodule