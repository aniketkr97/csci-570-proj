#!/bin/sh

g++ basic.cpp -o basic

mkdir BASIC_FOLDER
mv basic BASIC_FOLDER
cd BASIC_FOLDER


./basic ../in1.txt out1.txt
./basic ../in2.txt out2.txt
./basic ../in3.txt out3.txt
./basic ../in4.txt out4.txt
./basic ../in5.txt out5.txt
./basic ../in6.txt out6.txt
./basic ../in7.txt out7.txt
./basic ../in8.txt out8.txt
./basic ../in9.txt out9.txt
./basic ../in10.txt out10.txt
./basic ../in11.txt out11.txt
./basic ../in12.txt out12.txt
./basic ../in13.txt out13.txt
./basic ../in14.txt out14.txt
./basic ../in15.txt out15.txt
