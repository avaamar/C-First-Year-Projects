# Decision Tree Classifier

A general-purpose decision tree implementation using a multi-child linked structure.  
It can construct a tree from tabular input data, query attribute paths, and report node and leaf statistics.

## Features
- Custom `TreeNode` and `EdgeNode` classes for multi-branch tree design  
- Recursive algorithms for insertion, traversal, and query operations  
- Avoids STL algorithms to focus on manual pointer manipulation  

## Learning Outcomes
- Understanding hierarchical data representation  
- Implementing recursive structures in C++  
- Practising memory management and data abstraction  

## How to Run
Compile and execute using:
```bash
g++ main.cpp -o decisiontree
./decisiontree
