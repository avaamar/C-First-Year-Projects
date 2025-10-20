# Grid Word Finder

This program searches for target words in a two-dimensional character grid, both horizontally and vertically.  
It constructs a solution grid that marks found words and records words that were not located. The code includes automated test cases to verify correct functionality.

## Features
- Searches for words across rows and columns  
- Uses efficient substring comparison and early-exit conditions  
- Modular decomposition with helper functions for row and column operations  
- Demonstrates use of `std::vector` and `std::string`  

## Learning Outcomes
- Developing structured, test-driven C++ programs  
- Improving code readability and modularity  
- Understanding nested loops and memory-safe indexing  

## How to Run
Compile and execute using any C++ compiler:
```bash
g++ main.cpp -o gridfinder
./gridfinder
