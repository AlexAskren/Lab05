// Updated Hazard Detection Unit with instruction memory support alignment
module hazard_detection_unit(
    input wire ID_EX_MemRead,
    input wire [4:0] ID_EX_Rd,
    input wire [4:0] IF_ID_Rs1,
    input wire [4:0] IF_ID_Rs2,
    output reg stall,
    output reg PCWrite,
    output reg IF_ID_Write
);

    always @(*) begin
        // Default values
        stall       = 0;
        PCWrite     = 1;
        IF_ID_Write = 1;

        // Load-use hazard detection
        if (ID_EX_MemRead &&
            (ID_EX_Rd != {REG_ADDR_WIDTH{1'b0}}) &&
            ((ID_EX_Rd == IF_ID_Rs1) || (ID_EX_Rd == IF_ID_Rs2))) begin
            stall       = 1;
            PCWrite     = 0;
            IF_ID_Write = 0;
            $display("[HAZARD] Load-use hazard detected: Stall asserted.");
        end
    end

endmodule
