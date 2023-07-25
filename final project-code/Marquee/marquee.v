
module Marquee #(parameter numss) (
input clk,
output wire[6:0] FirstDisplay,
output wire[6:0] SecondDisplay,
output wire[6:0] ThirdDisplay,
output wire[6:0] FourthDisplay);

// time encoding 20230505 -> 2023-05-05
reg[39:0] num;
reg[39:0] encodedTime;
reg[10:0] index,temp=24;
always@(posedge clk) begin
	num = numss;
	for(index = 0; index < 10; index = index + 1) begin
		if(index == 2 || index == 5) begin
			encodedTime[index*4 +: 4] = 10;
		end
		else begin
			encodedTime[index*4 +: 4] = num % 10;
			num = num / 10;
		end
	end
	index = 0;
end

reg[3:0] a,b,c,d;

always@(posedge clk)
begin
    a = encodedTime[temp +: 4];
    b = encodedTime[temp+4 +: 4];
    c = encodedTime[temp+8 +: 4];
    d = encodedTime[temp+12 +: 4];
    if (temp == 0) begin
        temp = 24;
    end else begin
        temp = temp - 4;
    end
end


NumDisplay LED0(.Displayer(FirstDisplay),.DisplayNum(a));

NumDisplay LED1(.Displayer(SecondDisplay),.DisplayNum(b));

NumDisplay LED2(.Displayer(ThirdDisplay),.DisplayNum(c));

NumDisplay LED3(.Displayer(FourthDisplay),.DisplayNum(d));


endmodule 