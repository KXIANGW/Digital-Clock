module RealClock(input clk,input enable,
input isStop,
input isIncreas,
input TenSec,
input OneMin,
input TenMin,
input HalfHour,
input OneHour,
input FiveHour,
output reg[9:0] LED,
output wire[6:0] FirstDisplay,
output wire[6:0] SecondDisplay,
output wire[6:0] ThirdDisplay,
output wire[6:0] FourthDisplay);

reg signed [7:0] sec=0;
reg signed [7:0] min=25;
reg signed [6:0] hour=4;
reg unit=1;
reg status=0;
always@(posedge clk)
begin
	if(~enable)
		sec=sec;
	else if(~isStop)
	begin
		sec=sec+1;
	end
	else
	begin
		if(isIncreas)
		begin
			if(TenSec)
				sec=sec+10;
			if(OneMin)
				min=min+1;
			if(TenMin)
				min=min+10;
			if(HalfHour)
				min=min+30;
			if(OneHour)
				hour=hour+1;
			if(FiveHour)
				hour=hour+5;
		end
		else
		begin
			if(TenSec)
				sec=sec-10;
			if(OneMin)
				min=min-1;
			if(TenMin)
				min=min-10;
			if(HalfHour)
				min=min-30;
			if(OneHour)
				hour=hour-1;
			if(FiveHour)
				hour=hour-5;
		end
	end
	if(sec>=60)
	begin
		sec=sec-60;
		min=min+1;
	end
	else if(sec<0)
	begin
		min=min-1;
		sec=sec+60;
	end
	if(min>=60)
	begin
		min=min-60;
		hour=hour+1;
	end
	else if(min<0)
	begin
		hour=hour-1;
		min=min+60;
	end
	if(hour>=24)
		hour=hour-24;
	else if(hour<0)
		hour=hour+24;
end
reg [6:0] pos;
always@(sec)
begin
for(pos=0;pos<9;pos=pos+1)
begin
	if(sec>pos*6)
		LED[pos]=1;
	else
		LED[pos]=0;
end
end
NumDisplay LED0(FirstDisplay,hour/10);

NumDisplay LED1(SecondDisplay,hour%10);

NumDisplay LED2(ThirdDisplay,min/10);

NumDisplay LED3(FourthDisplay,min%10);


endmodule

