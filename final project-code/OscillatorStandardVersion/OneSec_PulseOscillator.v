module OneSec_PulseOscillator(
	input clk,
	output reg pulse
);

reg [32:0]count;

always@(posedge clk) begin

	count <= count + 1;
	if(count == 25000000) begin
		
		pulse <= ~pulse;
		count <= 0;
		
	end
	
end

endmodule
