#!/bin/bash

echo cleanup
rm color

echo building
g++ coloring.cpp graph.cpp -o color

echo RUN: certain
./color certain

echo RUN: random 15
./color random 15

