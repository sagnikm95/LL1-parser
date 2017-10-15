#ifndef firfollow_h
#define firfollow_h
#include <bits/stdc++.h>

using namespace std;
extern map<string,vector<string> > prod; //ds storing production rules
extern vector<string> nonTerminal; //ds storing list of non Terminals
extern map<string,set<string> > first; // ds to maintain first set of each Terminal and non Terminal 
extern map<string,set<string> > follow;// ds to maintain follow set of each non Terminal
extern string START; // start symbol of grammar
extern set<string> terminals; //// ds to maintain list of terminals

// Function to compute First Follow sets
void firfol_compute();

//Function to tokenize a string
vector<string> split(string &str);
#endif
