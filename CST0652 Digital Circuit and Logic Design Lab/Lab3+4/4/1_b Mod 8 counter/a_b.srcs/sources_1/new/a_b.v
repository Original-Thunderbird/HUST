`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2018/06/14 13:24:41
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


module a_b( Qcc,out,M,CP );
    input M,CP;
    output reg[3:0]out;
    output reg Qcc;
    reg CLR;
    reg Ld;
    wire [3:0]in;
    wire [3:0]put;
    assign in=4'b1001;    
    initial 
    begin 
    out <= 4'b0010;
    Qcc=1;
    Ld<=1;
    CLR<=1;
    end
    a rlz(in,CP,CLR,Ld,M,put,Qoc); 
    always@(posedge CP)
        begin
//            if(CLR==0)  CLR<=1;
//            if(CLR==1)  
                CLR<=~(put[3]&(~put[2])&(~put[1])&put[0]&M);
                #5 CLR<=1;
//            if(Ld==0)   Ld<=1;
//            if(Ld==1)  
            Ld<=~((~put[3])&(~put[2])&put[1]&(~put[0])&(~M));
            #5 Ld <=1;
            if((put==4'b0010&M)|(put==4'b1001&(~M)))    Qcc<=0;
            else Qcc<=1;
            out=put;
        end
endmodule

module a(in,CP,CLR,Ld,M,out,Qcc);
        input[3:0] in;
        input CP,CLR,Ld,M;
        output[3:0] out;
        output Qcc;
        reg[3:0] out;
        reg Qcc;
        initial Qcc=1;
        initial out=4'b0010;
        always@(posedge CP or negedge Ld or negedge CLR)
            begin
                if(~CLR)
                    begin          out<=4'b0010;       Qcc<=1;      end
                else if(~Ld)
                    begin     out<=in;    Qcc<=1;      end
                else if(~M)
                    begin     out<=out-4'b0001;    Qcc<=1;    end
                else if((out==4'b1111)&&M)
                    begin    out<=4'b0000;    Qcc<=0;
                    end
                else if((out==4'b0000)&&~M)
                    begin    out<=4'b0000;    Qcc<=1;
                    end
                else if(M)
                    begin    out<=out+4'b0001;    Qcc<=1;
                    end
            end
        endmodule