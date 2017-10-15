#ifndef tokenize_h
#define tokenize_h
#include <bits/stdc++.h>

using namespace std;

void error_print(int token_no); // Function to print postion of error in input program while compiling
string getVal(int token_no); // Function return value of token
string get_token(string name); //  Function that tokenizes the C like program
int getRow(int token_no);// Function return line no of token
int getCol(int token_no);// Function return col no of token

#endif

