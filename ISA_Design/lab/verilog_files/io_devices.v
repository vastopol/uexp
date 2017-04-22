`timescale 1ns / 1ps
//
// CSE141L Lab 3, Part B: Execution Unit Control and Test
// University of California, San Diego
// 
// Written by Donghwan Jeon, 5/21/2007
// Updated by Sat Garcia, 11/25/2008
// Updated by Vikram Bhatt, 11/25/2011
//
// I/O Devices Module
//
// parameters:
// 	D_WIDTH: port(channel) data width
// 	PA_WIDTH: port(channel) address width

// read_addr (in channel):
//  0: reserved
//  1: request the next word of the selected app
//  2: request the current clock counter value
//  3: input data from an input file
//  others: reserved

//
// write_addr (out channel):
//  0: reserved
//  1: select a SuperGarbage app (0: app0, 1: app1, ...)
//  2: set the current clock counter value
//  3: debug output console
//  others: reserved. simply print with $display command

module io_devices#(parameter D_WIDTH = 34, PA_WIDTH = 4)
(
	input 	reset_i,
	input 	clk,
	
	input	read_req_i,
	input	write_req_i,
	input	[PA_WIDTH-1 : 0] read_addr_i,
	input	[PA_WIDTH-1 : 0] write_addr_i,
	input	[D_WIDTH-1 : 0] din_i,
	output	[D_WIDTH-1 : 0] dout_o,
	output	read_ack_o,
	output	write_ack_o
);
`define ADDR_SIZE           	4
`define READ_ADDR_DATA			`ADDR_SIZE'b0001
`define READ_ADDR_COUNTER		`ADDR_SIZE'b0010
`define READ_ADDR_CONSOLE		`ADDR_SIZE'b0011

`define WRITE_ADDR_OPEN			`ADDR_SIZE'b0001
`define WRITE_ADDR_COUNTER		`ADDR_SIZE'b0010
`define WRITE_ADDR_CONSOLE		`ADDR_SIZE'b0011

`define DATA_SIZE					34
`define SG_APP0					`DATA_SIZE'b0000
`define SG_APP1					`DATA_SIZE'b0001
`define SG_APP2					`DATA_SIZE'b0010
`define SG_APP3					`DATA_SIZE'b0011

`define SG_APP0_NAME				"app0.bin"
`define SG_APP1_NAME				"app1.bin"
`define SG_APP2_NAME				"app2.bin"
`define SG_APP3_NAME				"app3.bin"
`define SG_APP0_IN				"app0.in"
`define SG_APP1_IN				"app1.in"
`define SG_APP2_IN				"app2.in"
`define SG_APP3_IN				"app3.in"

reg read_ack_r;
reg write_ack_r;

reg [D_WIDTH-1 : 0] counter;
reg [D_WIDTH-1 : 0] data;
integer file;
integer stdin;
integer rc;

assign dout_o = data[D_WIDTH-1 : 0];
assign read_ack_o = read_ack_r;
assign write_ack_o = write_ack_r;


// handles write requests
always@(posedge clk)
begin
	write_ack_r <= write_req_i;	
	
	if (write_req_i) begin
		$display("write> data 0x%x, channel 0x%x\n", din_i, write_addr_i);
		
		case(write_addr_i)
		`WRITE_ADDR_OPEN:
			case(din_i)
				`SG_APP0: 
				begin
					file = $fopen(`SG_APP0_NAME,"r"); 
					stdin = $fopen(`SG_APP0_IN, "r");
				end
				
				`SG_APP1: 
				begin
					file = $fopen(`SG_APP1_NAME,"r"); 
					stdin = $fopen(`SG_APP1_IN, "r");
				end
				
				`SG_APP2: 
				begin
					file = $fopen(`SG_APP2_NAME,"r"); 
					stdin = $fopen(`SG_APP2_IN, "r");
				end
				
				`SG_APP3: 
				begin
					file = $fopen(`SG_APP3_NAME,"r"); 
					stdin = $fopen(`SG_APP3_IN, "r");
				end
				default: $display("invalid file number 0x%x\n", din_i);
			endcase
			
		`WRITE_ADDR_CONSOLE: 
		   begin
			 $display("Debug Console> : 0x%x", din_i);			 
			end
		endcase
	end
end


// handles counter
always@(posedge clk)
begin
	if (reset_i) 
		counter <= 34'b0;
	else if (write_req_i & write_addr_i == `WRITE_ADDR_COUNTER)
		counter <= din_i;
	else
		counter <= counter + 1;
end
	
// handles read requests
always@(posedge clk)
begin
	read_ack_r <= read_req_i;
	if (reset_i) begin
		file = 0;		
	end
		
	if (read_req_i) begin
		$display("read> channel %d\n", read_addr_i);
		case(read_addr_i)
		`READ_ADDR_DATA:
			if (file == 0) begin
				$display("First open a program\n");
				data = {{D_WIDTH}{1'b1}};
			end 
			else begin
				rc = $fscanf(file, "%x", data);
				$display("%x\n", data);
			end
		
		`READ_ADDR_COUNTER:
			data <= counter;
			
		`READ_ADDR_CONSOLE: 
			begin
				rc = $fscanf(stdin, "%x", data);
				$display("Input data for ch 3: %x\n", data);
	         	end
		endcase
	end
end 

endmodule
