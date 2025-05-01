`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 04/01/2025 11:30:37 AM
// Design Name: 
// Module Name: xpm_dpram_top
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


module xpm_dpram_top(clk, rst, dina, dinb, douta, doutb, we, addra, addrb);
parameter addr_width=8;
parameter data_width=32;

input clk, rst;
input [data_width-1:0] dina, dinb;
input we;
input [addr_width-1:0] addra, addrb;
output [data_width-1:0] douta, doutb;
    
   xpm_memory_tdpram #(
      .ADDR_WIDTH_A(addr_width), .ADDR_WIDTH_B(addr_width),
      .AUTO_SLEEP_TIME(0),            // DECIMAL
      .BYTE_WRITE_WIDTH_A(data_width), .BYTE_WRITE_WIDTH_B(data_width),        // DECIMAL
      .CASCADE_HEIGHT(0),             // DECIMAL
      .CLOCKING_MODE("common_clock"), // String
      .ECC_BIT_RANGE("7:0"),          // String
      .ECC_MODE("no_ecc"),            // String
      .ECC_TYPE("none"),              // String
      .IGNORE_INIT_SYNTH(0),          // DECIMAL
      .MEMORY_INIT_FILE("none"),      // String
      .MEMORY_INIT_PARAM("0"),        // String
      .MEMORY_OPTIMIZATION("true"),   // String
      .MEMORY_PRIMITIVE("auto"),      // String
      .MEMORY_SIZE(2**addr_width*data_width),             // DECIMAL
      .MESSAGE_CONTROL(0),            // DECIMAL
      .RAM_DECOMP("auto"),            // String
      .READ_DATA_WIDTH_A(data_width), .READ_DATA_WIDTH_B(data_width),   
	  .READ_LATENCY_A(1), .READ_LATENCY_B(1), 
      .WRITE_DATA_WIDTH_A(data_width), .WRITE_DATA_WIDTH_B(data_width),        // DECIMAL
	  .READ_RESET_VALUE_A("0"), .READ_RESET_VALUE_B("0"),       // String
      .RST_MODE_A("SYNC"), .RST_MODE_B("SYNC"),            // String
      .SIM_ASSERT_CHK(0),             // DECIMAL; 0=disable simulation messages, 1=enable simulation messages
      .USE_EMBEDDED_CONSTRAINT(0),    // DECIMAL
      .USE_MEM_INIT(1),               // DECIMAL
      .USE_MEM_INIT_MMI(0),           // DECIMAL
      .WAKEUP_TIME("disable_sleep"),  // String      
      .WRITE_MODE_A("no_change"),  .WRITE_MODE_B("no_change"),     // String
      .WRITE_PROTECT(1)               // DECIMAL
   )
   xpm_memory_tdpram_inst (      
      .douta(douta), .doutb(doutb),      
      .addra(addra),.addrb(addrb), 
      .clka(clk), .clkb(clkb),
      .dina(dina), .dinb(dinb), 
      .ena(1'b1), .enb(1'b1),      
      .regcea(1'b1), .regceb(1'b1),
      .rsta(rst), .rstb(rst),
	  .dbiterra(), .dbiterrb(), .sbiterra(), .sbiterrb(), .sleep(), .injectdbiterra(), .injectdbiterrb(), .injectsbiterra(), .injectsbiterrb(),  
      .wea(we),.web(we)
   );
    
endmodule
