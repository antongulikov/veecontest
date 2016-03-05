#!/bin/bash

g++ -std=c++11 -o sol tmp3.cpp
echo "first compile"
g++ -std=c++11 -o gen gen.cpp
echo "second compile"

./gen $1 > in
echo "gen done"
./sol < in > out
echo "sol done"
cat in out > ret
echo "gogogog"
python3 tur.py < ret
