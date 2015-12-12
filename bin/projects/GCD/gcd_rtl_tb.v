module gcd_rtl_tb;

parameter width = 8;

reg [7:0] a;
reg [7:0] b;
reg clock;
reg load_n;
reg reset;
wire done;
wire [7:0] y;

gcd_rtl
 #(
  .width(8))
 U0 (
  .a(a),
  .b(b),
  .clock(clock),
  .load_n(load_n),
  .reset(reset),
  .done(done),
  .y(y));

  initial
  begin
    a = 8'b00110010;
  end

  initial
  begin
    b = 8'b00010100;
  end

  initial
    clock = 1'b0;

  always
  begin : Genclock
    #50 clock = 1'b1;
    #50 clock = 1'b0;
  end

  initial
  begin
    load_n = 1'b1;
    #300 load_n = 1'b0;
    #100 load_n = 1'b1;
  end

  initial
  begin
    reset = 1'b0;
    #100 reset = 1'b1;
  end

endmodule
