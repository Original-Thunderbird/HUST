`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2018/06/07 21:28:30
// Design Name: 
// Module Name: div
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


module div(
   input cp,
   output reg clk
    );
 parameter t=10;
       integer count;
       initial begin
           count <= 0;
           clk <= 0;
       end
       always @(posedge cp) begin
           count = count + 1;
           if(count == t) begin
               clk = ~clk;
               count = 0;
           end
       end
endmodule
