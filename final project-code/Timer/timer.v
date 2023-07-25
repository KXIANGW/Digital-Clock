module Timer(input clk, input reset, input stop, input enable, 
             output wire[6:0] firstSegment, output wire[6:0] secondSegment, output wire[6:0] thirdSegment, output wire[0:6] fourthSegment);
// 1,2,3,4
reg[3:0] first = 0, second = 0, third = 0, fourth = 0;
reg[1:0] status = 1;


always@ (posedge clk) begin
	if(enable)begin
		if(reset) begin
			if(status)
				fourth = fourth + 1;
			if(fourth>9) begin
				fourth = 0;
				third  = third + 1; 
			end
			if(third>5) begin
				third = 0;
				second = second + 1;
			end
			if(second>9) begin
				second = 0;
				first = first + 1;
			end
			if(first>5) begin
				first = 0;
				//status = status + 1;
			end
		end
		else begin        
			first = 0;
			second = 0;
			third = 0;
			fourth = 0;
		end
	end
end
always@(negedge stop)
begin
	if(enable)begin
		status = 1-status;
	end
end
NumDisplay Seg1(firstSegment,first);
NumDisplay Seg2(secondSegment,second);
NumDisplay Seg3(thirdSegment,third);
NumDisplay Seg4(fourthSegment,fourth);


endmodule
