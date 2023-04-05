`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2018/06/13 13:40:37
// Design Name: 
// Module Name: b_sim
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


module b_sim;
reg in,clk; 
wire out; 
wire p[1:3];
b mealy(in,clk,out,p); 
always #10 clk = ~clk; 
initial 
begin 
in <= 0; 
clk = 1; 
#12 in<=1;     #20 in<=0;     #20 in<=1;     #20 in<=1;     #20 in<=0; 
#20 in<=1;     #20 in<=0;     #20 in<=1;     #20 in<=1;     #20 in<=1; 
#20 in<=0;     #20 in<=1;     #20 in<=1;     #20 in<=0;     #20 in<=1;
#20 in<=0;     #20 in<=1;     #20 in<=0;     #20 in<=1;     #20 in<=1;
#20 in<=0;     #20 in<=1;     #20 in<=1;     #20 in<=0;
end 
endmodule 