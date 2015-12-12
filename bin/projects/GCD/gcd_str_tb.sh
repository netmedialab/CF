echo vcd file gcd_str_tb.vcd -nomap >> gcd_str_tb.run
echo vcd add /gcd_str_tb/a >> gcd_str_tb.run
echo vcd add /gcd_str_tb/b >> gcd_str_tb.run
echo vcd add /gcd_str_tb/clk >> gcd_str_tb.run
echo vcd add /gcd_str_tb/done >> gcd_str_tb.run
echo vcd add /gcd_str_tb/load >> gcd_str_tb.run
echo vcd add /gcd_str_tb/rst_n >> gcd_str_tb.run
echo vcd add /gcd_str_tb/y >> gcd_str_tb.run
echo vcd on >> gcd_str_tb.run
echo run 10000ns >> gcd_str_tb.run
echo vcd off >> gcd_str_tb.run
echo quit -f >> gcd_str_tb.run
vlib ./work
vmap FDLIV $FLOWRIANII_HOME/lib/FDLIV/FDLIV
vlog  gcd_str.v
vlog  gcd_str_tb.v
vsim -novopt -t ns -c gcd_str_tb -do gcd_str_tb.run
rm -f gcd_str_tb.run
echo Finish simulation !!
