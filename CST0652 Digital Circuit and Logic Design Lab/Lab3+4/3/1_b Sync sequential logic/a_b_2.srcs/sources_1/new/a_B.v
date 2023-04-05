`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2018/06/06 21:39:34
// Design Name: 
// Module Name: a_b
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


module a_b(
    input x,CP,
    output reg y,Q0,W1,Q1
    );
initial
begin
    Q0<=1'b0;
    Q1<=1'b1;
    W1=1'b0;
    y=1'b0;
end
always @(posedge CP)   
    begin
        Q0 <= ~Q0;
        W1 <= Q0^x;
        case (W1)
            1'b0:   Q1<=Q1;
            1'b1:   Q1<=~Q1;
        endcase;
        y <= x & (~Q1);
    end
endmodule 
