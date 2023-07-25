module MasterClock(input clk,
output reg [0:6] FirstSegment,output reg [0:6] SecondSegment,
output reg [0:6] ThirdSegment,output reg [0:6] FourthSegment,
input [2:0]Button,input [9:0]Switch,output reg[9:0] LED);

reg pause;
reg [1:0]mode=0;
wire [0:6]ClockFirstSegment,ClockSecondSegment,ClockThirdSegment,ClockFourthSegment;
wire [9:0]ClockLED;
wire [0:6]TerminatorFirstSegment,TerminatorSecondSegment,TerminatorThirdSegment,TerminatorFourthSegment;
wire [9:0]TerminatorLED;
wire [0:6]MarqueeFirstSegment,MarqueeSecondSegment,MarqueeThirdSegment,MarqueeFourthSegment;
wire [0:6]TimerFirstSegment,TimerSecondSegment,TimerThirdSegment,TimerFourthSegment;
reg RealClockEnable=1,TerminateEnable=0,MarqueeEnable=0,TimerEnable=0;

OneSec_PulseOscillator count(clk,pause);


Time_Terminator terminate(
	.clk(clk),.reset(Switch[0]),.enable(Switch[1]),.SetSecond(Button[0]),.SetMinute(Button[1]), .ModeEnable(TerminateEnable),
	.first_seg(TerminatorFirstSegment),.second_seg(TerminatorSecondSegment),
	.third_seg(TerminatorThirdSegment),.fourth_seg(TerminatorFourthSegment));
	
RealClock clock(.clk(pause),.enable(RealClockEnable),.LED(ClockLED),.isStop(Switch[0]),.isIncreas(Switch[1]),
		.TenSec(Switch[2]),.OneMin(Switch[3]),.TenMin(Switch[4]),.HalfHour(Switch[5]),.OneHour(Switch[6]),.FiveHour(Switch[7]),
		.FirstDisplay(ClockFourthSegment),.SecondDisplay(ClockThirdSegment),
		.ThirdDisplay(ClockSecondSegment),.FourthDisplay(ClockFirstSegment));


Marquee #(20230823) marquee (
.clk(pause),
.FirstDisplay(MarqueeFirstSegment),.SecondDisplay(MarqueeSecondSegment),.ThirdDisplay(MarqueeThirdSegment),.FourthDisplay(MarqueeFourthSegment));

Timer(.clk(pause), .reset(Button[0]), .stop(Button[1]),.enable(TimerEnable),
      .firstSegment(TimerFourthSegment),.secondSegment(TimerThirdSegment),.thirdSegment(TimerSecondSegment),.fourthSegment(TimerFirstSegment));

always@(negedge pause)
begin
	
	case(mode)
	0:
	begin
	TimerEnable=0;
	RealClockEnable=1;
	FourthSegment=ClockFourthSegment;
	ThirdSegment=ClockThirdSegment;
	SecondSegment=ClockSecondSegment;
	FirstSegment=ClockFirstSegment;
	LED=ClockLED;
	end
	1:
	begin
	RealClockEnable=0;
	TerminateEnable=1;
	FourthSegment=TerminatorFourthSegment;
	ThirdSegment=TerminatorThirdSegment;
	SecondSegment=TerminatorSecondSegment;
	FirstSegment=TerminatorFirstSegment;
	LED=TerminatorLED;
	end
	2:
	begin
	TerminateEnable=0;
	MarqueeEnable=1;
	FourthSegment=MarqueeFourthSegment;
	ThirdSegment=MarqueeThirdSegment;
	SecondSegment=MarqueeSecondSegment;
	FirstSegment=MarqueeFirstSegment;
	end
	3:
	begin
	MarqueeEnable=0;
	TimerEnable=1;
	FourthSegment=TimerFourthSegment;
	ThirdSegment=TimerThirdSegment;
	SecondSegment=TimerSecondSegment;
	FirstSegment=TimerFirstSegment;
	end
	default:
	begin
	FourthSegment=0;
	ThirdSegment=0;
	SecondSegment=0;
	FirstSegment=0;
	end
	endcase
end
always@(negedge Button[2])
begin
	if(mode<3)
		mode<=mode+1;
	else
		mode<=0;
end

endmodule
