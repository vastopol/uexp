`timescale 1ns / 1ps
//
// CSE141L Lab 4, Part 2: Execution Unit Control and Test
// University of California, San Diego
// 
// Written by Donghwan Jeon, 5/21/2007
//
// Top test module
//
module top#(parameter D_WIDTH = 34, PA_WIDTH = 4)
(
	input  clk,
	input  reset_i
); 

wire read_req, write_req;
wire read_ack, write_ack;
wire [PA_WIDTH-1 : 0] read_addr;
wire [PA_WIDTH-1 : 0] write_addr;
wire [D_WIDTH-1 : 0] from_proc;
wire [D_WIDTH-1 : 0] to_proc;
   

core processor
(
	.clk(clk),
	.reset_i(reset_i),
    
	// I/O interface
	.in_req_o(read_req),
	.out_req_o(write_req),
	.in_addr_o(read_addr),
	.out_addr_o(write_addr),
	.in_data_i(to_proc),
	.out_data_o(from_proc),
	.in_ack_i(read_ack),
	.out_ack_i(write_ack)	 
); 


io_devices dev
(
	.clk(clk),
	.reset_i(reset_i),
	
	.read_req_i(read_req),
	.write_req_i(write_req),
	.read_addr_i(read_addr),
	.write_addr_i(write_addr),
	.dout_o(to_proc),
	.din_i(from_proc),
	.read_ack_o(read_ack),
	.write_ack_o(write_ack)	
);

endmodule
