#!/bin/sh

g++ efficient.cpp -o efficient 

mkdir EFFICIENT_FOLDER
mv efficient EFFICIENT_FOLDER
cd EFFICIENT_FOLDER


./efficient ../in1.txt out1.txt
./efficient ../in2.txt out2.txt
./efficient ../in3.txt out3.txt
./efficient ../in4.txt out4.txt
./efficient ../in5.txt out5.txt
./efficient ../in6.txt out6.txt
./efficient ../in7.txt out7.txt
./efficient ../in8.txt out8.txt
./efficient ../in9.txt out9.txt
./efficient ../in10.txt out10.txt
./efficient ../in11.txt out11.txt
./efficient ../in12.txt out12.txt
./efficient ../in13.txt out13.txt
./efficient ../in14.txt out14.txt
./efficient ../in15.txt out15.txt

