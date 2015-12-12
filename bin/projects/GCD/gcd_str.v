`define GND 1'b0
`define ZERO8 8'b00000000

module gcd (a, b, y, clk, rst_n, load, done);
  input [7:0] a;
  input [7:0] b;
  output [7:0] y;
  input clk;
  input rst_n;
  input load;
  output done;

  wire [7:0] a_next, b_next, a_m, b_m, a_hold, b_hold, aminusb, a_new, y_s;
  wire regen, bneq0, alessthanb, done_s;

  assign regen = bneq0 | load;
  assign done = done_s;
  assign y = y_s;

  \FDLIV.fd_mux2i #(.width(8))
         U_MUXA (.mux_in0(a_next), .mux_in1(a), .sel(load), .mux_out(a_m));

  \FDLIV.fd_reg #(.width(8), .reset_sync(0), .reset_val(0))
         U_REGA (.rst_n(rst_n), .clk(clk), .en(regen), .reg_in(a_m), .reg_out(a_hold));

  \FDLIV.fd_comp6 #(.width(8), .addtype(0))
         U_COMP (.tc(`GND), .a(a_hold), .b(b_hold), .lt(alessthanb), .gt(), .eq(), .le(), .ge(), .ne());

  \FDLIV.fd_reg #(.width(8), .reset_sync(0), .reset_val(0))
         U_REGB (.rst_n(rst_n), .clk(clk), .en(regen), .reg_in(b_m), .reg_out(b_hold));

  \FDLIV.fd_mux2i #(.width(8))
         U_MUX1 (.mux_in0(aminusb), .mux_in1(a_hold), .sel(alessthanb), .mux_out(a_new));

  \FDLIV.fd_mux2i #(.width(8))
         U_MUX2U (.mux_in0(a_new), .mux_in1(b_hold), .sel(alessthanb), .mux_out(a_next));

  \FDLIV.fd_mux2i #(.width(8))
         U_MUX2D (.mux_in0(b_hold), .mux_in1(a_new), .sel(alessthanb), .mux_out(b_next));

  \FDLIV.fd_sub #(.width(8), .addtype(0))
         U_SUB (.bi(`GND), .a(a_hold), .b(b_hold), .diff(aminusb), .bo());

  \FDLIV.fd_mux2i #(.width(8))
         U_MUXB (.mux_in0(b_next), .mux_in1(b), .sel(load), .mux_out(b_m));

  \FDLIV.fd_comp6 #(.width(8), .addtype(0))
         U_DONE (.tc(`GND), .a(b_hold), .b(`ZERO8), .lt(), .gt(), .eq(done_s), .le(), .ge(), .ne(bneq0));

  \FDLIV.fd_mux2i #(.width(8))
         U_Y (.mux_in0(`ZERO8), .mux_in1(a_hold), .sel(done_s), .mux_out(y_s));

endmodule

