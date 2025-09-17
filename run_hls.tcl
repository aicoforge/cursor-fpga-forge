open_project proj -reset
set_top adder_kernel
add_files adder_kernel.cpp
add_files -tb tb_adder.cpp
open_solution "sol1" -reset
set_part {xcu50-fsvh2104-2-e}  ;# use u50 as default
create_clock -period 5 -name default
csim_design -O
csynth_design
exit
