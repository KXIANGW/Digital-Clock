module Timer(input clk, input reset, input stop, 
             output reg[6:0] firstSegment, output reg[6:0] secondSegment, output reg[6:0] thirdSegment, output reg[6:0] fourthSegment);
// 1,2,3,4
reg[3:0] first = 0, second = 0, third = 0, fourth = 0;
reg[1:0] status = 1;

always@ (posedge clk, posedge reset) begin
	if(reset) begin
        first <= 0;
        second <= 0;
        third <= 0;
        fourth <= 0;
    end
	else begin
		if(stop)
			status <= status + 1;
		if(status > 1)
			status <= 0;
		if(status)
			fourth <= fourth + 1;

		if(fourth>9) begin
			fourth <= 0;
			third  <= third + 1; 
		end
		if(third>9) begin
			third <= 0;
			second <= second + 1;
		end
		if(second>9) begin
			second <= 0;
			first <= first + 1;
		end
		if(first>9)
			first <= 0;
	end
end


    NumDisplay Seg1(FisrtSegment,first);
    NumDisplay Seg2(secondSegment,second);
    NumDisplay Seg3(thirdSegment,third);
    NumDisplay Seg4(fourthSegment,fourth);


endmodule
