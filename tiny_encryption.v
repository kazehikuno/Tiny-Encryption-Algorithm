/* 	Christopher Hays
	Implementation of TEA encryption algorithm in verilog.
	Encrypts or decrypts a 64-bit block of data in 2 32-bit sections.
	inputs: 
		control - controls the current action of the module
		input_bus_a - the first 32-bit chunk of data
		input_bus_b - the second 32-bit chunk of data
	outputs:
		output_bus_a - first half of processed output data
		output_bus_b - second half of processed output data
		status - the current state of module
*/

module tiny_encryption (input [1:0] control,
				 input [31:0] input_bus_a, input_bus_b,
				 output reg [31:0] output_bus_a, output_bus_b,
				 output reg [1:0] status
				 );
	// internal registers
	reg [31:0] v0, v1, sum;		// registers for processing
	reg [31:0] delta = 32'h9e3779b9;	// delta is a constant determined by the authors of the algorithm
	reg [31:0] k0 = 32'h95a8882c;	// the four encryption keys
	reg [31:0] k1 = 32'h9d2cc113;
	reg [31:0] k2 = 32'h815aa0cd; 
	reg [31:0] k3 = 32'ha1c489f7;
	integer i;				// for loop control

	// setup
	initial begin
		status = 0;		// status = 0, idle state
	end

	// control
	always @(control) begin			// control signal changed by the C program
		case (control)			// check the value and take appropriate action
			0: begin			// control = 0
				status = 0;	// no action, status = 0
			end
			1: begin				// control = 1, start encryption
				v0 = input_bus_a;	// read bus a into v0
				v1 = input_bus_b;	// read bus b into v1
				sum = 0;			// start the sum at 0
				for (i=0; i<32; i = i+1) begin	// run the algorithm
					sum = sum + delta;	// delta increments
					v0 = v0 + (((v1 <<< 4) + k0) ^ (v1 + sum) ^ ((v1 >> 5) + k1));
					v1 = v1 + (((v0 <<< 4) + k2) ^ (v0 + sum) ^ ((v0 >> 5) + k3));
				end
				output_bus_a = v0;	// output to bus a
				output_bus_b = v1;	// output to bus b
				status = 1;		// status = 1, data is encrypted and ready
			end

			2: begin				// control = 2, start decryption
				v0 = input_bus_a;	// read bus a into v0
				v1 = input_bus_b;	// read bus b into v1
				sum = 32'hc6ef3720;	// the sum value after 32 cycles
				for (i=0; i<32; i = i+1) begin	// run the algorithm
					v1 = v1 - (((v0 <<< 4) + k2) ^ (v0 + sum) ^ ((v0 >> 5) + k3));
					v0 = v0 - (((v1 <<< 4) + k0) ^ (v1 + sum) ^ ((v1 >> 5) + k1));
					sum = sum - delta;	// delta decrements this time
				end
				output_bus_a = v0;	// output to bus a
				output_bus_b = v1;	// output to bus b
				status = 2;		// status = 2, data is decrypted
			end
			default: begin
				status = 0;	// any other control signals will result in no action
			end
		endcase
	end
endmodule  // end tiny_encryption
