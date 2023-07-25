module NumDisplay(output reg [6:0]Displayer,input [3:0] DisplayNum);

always @ (DisplayNum)
begin
	case(DisplayNum)
	 0:
		Displayer = 7'b0000001;
	 1:
		Displayer = 7'b1001111;
	 2:
		Displayer = 7'b0010010;
	 3:
		Displayer = 7'b0000110;
	 4:
		Displayer = 7'b1001100;
	 5:
		Displayer = 7'b0100100;
	 6:
		Displayer = 7'b0100000;
	 7:
		Displayer = 7'b0001111;
	 8:
		Displayer = 7'b0000000;
	 9:
		Displayer = 7'b0000100;
	10:
		Displayer = 7'b1111110;
	default:
		Displayer = 7'b1111111;
	endcase
end


endmodule 