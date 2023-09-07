#!/bin/bash
CWD=$(pwd)

Help()
{
   # Display Help
   echo "Build the Kyber scheme with a defined security level test and RISC-V core."
   echo
   echo "Syntax: ./build_kyber_riscv.sh -t <test> -c <core>"
   echo
   echo "options for -t: test_kyber512, test_kyber768, test_kyber1024"
   echo "options for -c: steel, orca"
   echo
   echo "Example: ./build_kyber_riscv.sh -t test_kyber1024 -c orca"
}

while getopts t:c:h flag
do
    case "${flag}" in
        t) TEST=${OPTARG};;
        c) CORE=${OPTARG};;
        h) # display Help
           Help
           exit;;
    esac
done

if [[ $TEST == "test_kyber512" ]] || [[ $TEST == "test_kyber768" ]] || [[ $TEST == "test_kyber1024" ]]
then
if [[ $CORE == "orca" ]]
then
  echo "Building $TEST for the ORCA core."
  sudo make clean
  sudo make $TEST ORCA=1
  sudo ./elf2hex.sh $TEST
  sudo make $TEST.ihex ORCA=1 # used for simulation
  sudo make $TEST.coe ORCA=1  # used for hw implementation
  sudo /opt/riscv/bin/riscv32-unknown-elf-objdump -d -S $CWD/$TEST > $CWD/generated_asm_$TEST.txt
  sed -i 's/^\/\/`define MULTIPLE_CYCLES/`define MULTIPLE_CYCLES/' ../../../rtl/fpau_top.sv
elif [[ $CORE == "steel" ]]
then
  echo "Building $TEST for the STEEL core."
  sudo make clean
  sudo make $TEST
  sudo ./elf2hex.sh $TEST
  sudo cp $TEST.hex ../../../riscv_cores/riscv-steel-core/hello_world/program.mem
  sudo /opt/riscv/bin/riscv32-unknown-elf-objdump -d -S $CWD/$TEST > $CWD/generated_asm_$TEST.txt
  sed -i 's/^`define MULTIPLE_CYCLES/\/\/`define MULTIPLE_CYCLES/' ../../../rtl/fpau_top.sv
else
  echo "Invalid core option. Call with -h for help"
fi
else
  echo "Invalid test option. Call with -h for help"
fi
