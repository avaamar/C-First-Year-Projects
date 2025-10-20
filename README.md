# C++ First-Year Projects

This repository gathers a selection of my first-year C++ programming projects completed as part of the Electrical and Electronic Engineering course at Imperial College London.  
Each project demonstrates different aspects of problem-solving, algorithmic thinking, and object-oriented design — from grid search and testing to data structures and embedded systems.

---

## 1) Grid Word Finder (Assignment 1)

A C++ program that searches for target words in a two-dimensional character grid, both horizontally and vertically.  
The program builds a “solution” grid indicating found words and compiles a list of words not found. It includes automated test cases to verify correctness, including overlapping and missing words.

**Main features:**
- Efficient word search using substring scanning and row/column extraction  
- Modular structure with separate functions for row and column handling  
- Demonstrates use of `std::vector`, `std::string`, and const references  

**Code review improvements:**
- Early exit on first character mismatch for better performance  
- Decomposition into smaller helper functions for readability  
- Proper use of `const` and improved index safety  

*Folder:* `/GridWordFinder`

---

## 2) Wordle Helper

A console-based assistant for solving Wordle-style puzzles.  
The program filters possible words based on known positions, required letters, and excluded characters. It demonstrates structured use of the C++ Standard Library and clear modular design for user interaction.

**Main features:**
- String and file handling for dictionary-based search  
- Letter constraint filtering logic  
- Clean, readable structure for maintainability  

*Folder:* `/WordleHelper`

---

## 3) Decision Tree Classifier

A general-purpose decision tree implementation built using a multi-child linked structure.  
The project supports tree construction from tabular input data, querying based on attribute values, and reporting node and leaf counts. It was designed with a focus on data abstraction and memory management.

**Main features:**
- Custom `TreeNode` and `EdgeNode` class hierarchy  
- Recursive tree traversal for search and analysis  
- Emphasis on pointer-based design and object-oriented programming  

*Folder:* `/DecisionTree`

---

## Skills Demonstrated
- C++ programming fundamentals (functions, classes, pointers, const-correctness)  
- Algorithm design and implementation  
- Modular and object-oriented design principles  
- Development of testing and validation routines  
- Basic embedded systems programming and integration
