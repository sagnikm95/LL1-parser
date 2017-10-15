#ifndef leftrecur_h
#define leftrecur_h
#include <bits/stdc++.h>

using namespace std;
extern map<string,vector<string> > prod,finalProd; //data structure for storing production rules
extern vector<string> nonTerminal;  // data structure for maintaining list of non Terminals used
void eliminate_lr(string name); // Function to eliminate LR from given grammar
void parse(string s); // Function to parse a file containing the grammar and store it in the form of a hash map
#endif
