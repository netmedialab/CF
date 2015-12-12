module gcd_str_tb;

reg [7:0] a;
reg [7:0] b;
reg clk;
reg rst_n;
reg load;
wire [7:0] y;
wire done;

gcd U0 (
  .a(a),
  .b(b),
  .clk(clk),
  .rst_n(rst_n),
  .load(load),
  .y(y),
  .done(done));

  initial
  begin
    a = 8'b00110010;
  end

  initial
  begin
    b = 8'b00010100;
  end

  initial
    clk = 1'b0;

  always
  begin : Genclk
    #50 clk = 1'b1;
    #50 clk = 1'b0;
  end

  initial
  begin
    rst_n = 1'b0;
    #100 rst_n = 1'b1;
  end

  initial
  begin
    load = 1'b0;
    #300 load = 1'b1;
    #100 load = 1'b0;
  end

endmodule
