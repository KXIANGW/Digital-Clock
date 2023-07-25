module Time_Terminator(
	input clk,
	input reset,
	input enable,
	input SetSecond,
	input SetMinute,
	input ModeEnable,
	output wire [0:6]first_seg,
	output wire [0:6]second_seg,
	output wire [0:6]third_seg,
	output wire [0:6]fourth_seg
);

reg start = 0;
reg counting_start = 0;
reg z5pse = 0;
reg z5prev = 0;
reg [32:0]seconds = 0;
reg [32:0]minutes = 0;
reg [32:0]count = 0;

reg [3:0]button_state = 4'b1111;

reg init_state = 1;

always@(posedge clk) begin // according to normal logic

	if(ModeEnable) begin
	// debounce
		button_state[0] <= button_state[1];
		button_state[1] <= ~SetSecond;
		button_state[2] <= button_state[3];
		button_state[3] <= ~SetMinute;

		if(counting_start) begin
			
			count <= count + 1;
			if(count == 50000000) begin
				z5pse <= ~z5pse;
				count <= 0;
			end
			
		end
		
		if(enable == 1) begin // enable countdown
			start <= 1;
			counting_start <= 1;
			init_state <= 0;
		end
		if(button_state[0] == 0 && start == 0 && button_state[1] == 1) begin // init
			if(minutes < 60) begin
				seconds <= seconds + 1;
			end
			if(seconds >= 60) begin
				minutes <= minutes + 1;
				seconds <= 0;
			end
		end
		if(button_state[2] == 0 && start == 0 && button_state[3] == 1) begin // init
			if(minutes < 60) begin
				minutes <= minutes + 1;
			end
		end
		
		if(z5pse != z5prev) begin
			if(start) begin
				if(seconds == 0 && minutes > 0) begin
					minutes <= minutes - 1;
					seconds <= 59;
				end else
					seconds <= seconds - 1;
					
			end
			
			z5prev <= z5pse;
		end
		
		if(minutes == 0 && seconds == 0 && init_state == 0) begin // refresh status
			start <= 0;
			counting_start <= 0;
			z5pse <= 0;
			z5prev <= 0;
			count <= 0;
			init_state <= 1;
		end
		if(reset) begin
			start <= 0;
			init_state <= 1;
			minutes <= 0;
			seconds <= 0;
			z5pse <= 0;
			z5prev <= 0;
			count <= 0;
		end
	
	end

end

NumDisplay Num1(first_seg,seconds % 10);
NumDisplay Num2(second_seg,seconds / 10);
NumDisplay Num3(third_seg,minutes % 10);
NumDisplay Num4(fourth_seg,minutes / 10);

endmodule
