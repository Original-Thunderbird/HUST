`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2018/06/07 14:32:33
// Design Name: 
// Module Name: a_B_sim
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

module a_B_sim; 
  reg  X;
  wire  Y,q1,w1,q0;
  reg clk=0;
  initial  
  begin
  X <= 1;
  #500 X<=0;
  end
  a_b  real_a_B(.x(X),.CP(clk),.Q0(q0),.W1(w1),.Q1(q1),.y(Y)); 
  always #50 clk = ~clk;
endmodule