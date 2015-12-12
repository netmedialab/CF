echo vcd file gcd_rtl_tb.vcd -nomap >> gcd_rtl_tb.run
echo vcd add /gcd_rtl_tb/a >> gcd_rtl_tb.run
echo vcd add /gcd_rtl_tb/b >> gcd_rtl_tb.run
echo vcd add /gcd_rtl_tb/clock >> gcd_rtl_tb.run
echo vcd add /gcd_rtl_tb/done >> gcd_rtl_tb.run
echo vcd add /gcd_rtl_tb/load_n >> gcd_rtl_tb.run
echo vcd add /gcd_rtl_tb/reset >> gcd_rtl_tb.run
echo vcd add /gcd_rtl_tb/y >> gcd_rtl_tb.run
echo vcd on >> gcd_rtl_tb.run
echo run 10000ns >> gcd_rtl_tb.run
echo vcd off >> gcd_rtl_tb.run
echo quit -f >> gcd_rtl_tb.run
vlib ./work
vlog  gcd_rtl.v
vlog  gcd_rtl_tb.v
vsim -novopt -t ns -c gcd_rtl_tb -do gcd_rtl_tb.run
rm -f gcd_rtl_tb.run
echo Finish simulation !!
