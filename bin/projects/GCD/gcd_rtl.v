
module gcd_rtl (reset, clock, load_n, a, b, done, y);
  parameter width = 8;

  input reset;
  input clock;
  input load_n;
  input [width-1:0] a;
  input [width-1:0] b;
  output done;
  output [width-1:0] y;

  reg a_equalto_b, a_lessthan_b;
  reg a_greaterthan_b, done_hold;
  reg [width-1:0] a_hold;
  reg [width-1:0] b_hold;

  assign y = a_hold;
  assign done = done_hold;

  always @(posedge clock or negedge reset)
    begin: SUB_PROC
    if (!reset)
      begin
      a_hold <= {width{1'b0}};
      b_hold <= {width{1'b0}};
      done_hold <= 1'b1;
      end
    else if (!load_n)
      begin
      a_hold <= a;
      b_hold <= b;
      done_hold <= 1'b0;
      end
    else if (a_equalto_b)
      begin
      done_hold <= 1'b1;
      end
    else if (a_greaterthan_b)
      begin
      a_hold <= a_hold - b_hold;
      done_hold <= 1'b0;
      end
    else if (a_lessthan_b)
      begin
      b_hold <= b_hold - a_hold;
      done_hold <= 1'b0;
      end
    end

  always @(a_hold or b_hold)
    begin: COMP_PROC
    if (a_hold == b_hold)
      begin
      a_equalto_b = 1'b1;
      a_lessthan_b = 1'b0;
      a_greaterthan_b = 1'b0;
      end
    else if (a_hold > b_hold)
      begin
      a_equalto_b = 1'b0;
      a_lessthan_b = 1'b0;
      a_greaterthan_b = 1'b1;
      end
    else if (a_hold < b_hold)
      begin
      a_equalto_b = 1'b0;
      a_lessthan_b = 1'b1;
      a_greaterthan_b = 1'b0;
      end
    end
endmodule
