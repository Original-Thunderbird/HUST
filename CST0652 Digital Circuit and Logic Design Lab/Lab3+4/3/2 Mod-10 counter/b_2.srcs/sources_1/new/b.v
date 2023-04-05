`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2018/06/07 15:18:58
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


module b(input in,output out,q1,q2,q3,q4);
 	wire j1, k1, ck1, j2, k2, ck2, j3, k3, ck3, j4, k4, ck4, nq4;    
    assign j1 = 1;
    assign k1 = 1;
    assign ck1 = in;
    assign j2 = nq4;
    assign k2 = 1;
    assign ck2 = q1;
    assign j3 = 1;
    assign k3 = 1;
    assign ck3 = q2;
    assign j4 = q2 & q3;
    assign k4 = 1;
    assign ck4 = q1;
    assign out = in & q1 & q4;
    jk uut1(.clk(ck1),.j(j1),.k(k1), .q(q1),.qb(nq1));
    jk uut2(.clk(ck2),.j(j2),.k(k2), .q(q2),.qb(nq2));
    jk uut3(.clk(ck3),.j(j3),.k(k3), .q(q3),.qb(nq3));
    jk uut4(.clk(ck4),.j(j4),.k(k4), .q(q4),.qb(nq4));
endmodule

module jk(input clk,j,k, output q,qb);
 	reg q; 
   assign qb=~q;
   initial q=1;
  	  always@(negedge clk)
  	  begin
   	   	 case({j,k})
       	    	2'b00:q<=q;
               	2'b01:q<=0;
               	2'b10:q<=1;
              	 2'b11:q<=~q;
              endcase
             end
endmodule
