`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2018/06/11 15:10:40
// Design Name: 
// Module Name: a
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


module a(in,CP,CLR,Ld,M,out,Qcc);
input[3:0] in;
input CP,CLR,Ld,M;
output[3:0] out;
output Qcc;
reg[3:0] out;
reg Qcc;
initial out=4'b0000;
initial Qcc=1;
always@(posedge CP or negedge Ld or negedge CLR)
    begin
        if(~CLR)
            begin   out<=4'b0000;   Qcc<=1; end
        else if(~Ld)
            begin   out<=in;    Qcc<=1; end
        else if((out==4'b1111)&&M)
            begin   out<=4'b0000;   Qcc<=0; end
        else if((out==4'b0000)&&~M)
            begin   out<=4'b1111;   Qcc<=0;  end
        else if(~M)
            begin   out<=out-4'b0001;   Qcc<=1; end
        else if(M)
            begin   out<=out+4'b0001;   Qcc<=1; end
    end
endmodule