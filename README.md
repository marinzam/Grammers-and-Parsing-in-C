# Grammers-and-Parsing-in-C
Project for Formal Systems Course
Implement a recursive-descent parser for the grammar of arithmetic expressions seen in class (FOCS Figure 11.2 or an appropriate variant).
• Your parser should read successive expressions from standard input, con- struct a parse tree for the expression if it is well-formed, and then print that parse tree to standard output (see description below).
• If the input is not well-formed, your parser should print an appropriate mes- sage and resume processing at the next line of input (i.e., skip to the next newline).
• You may assume that expressions are on a single line of input if you want, or deal with multi-line input.
• Your program should exit on end-of-file on standard input.
Implement a table-driven parser for the grammar of arithmetic expressions. Read expressions from standard input, try to parse the input, print the parse tree or an error message. Most of the infrastructure of the program will be the same part 1. Design it right and much of your code can be shared between the programs.
3. Turn your parser into a calculator by evaluating the parse tree produced by your parser. That is, write code to traverse a parse tree computing the value of sub- expressions as appropriate and printing the final result. You may use either ver- sion of your parser—they should produce equivalent parse trees. The result is a command-line calculator like the UNIX program bc.
