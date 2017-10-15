#ifndef parseTable_h
#define parseTable_h
#include <bits/stdc++.h>

using namespace std;
extern map<pair<string,string> ,string> parse_table; //  ds to maintain parse table entries
void generate_pt(); // Function to generate parse table using first follow sets

#endif
