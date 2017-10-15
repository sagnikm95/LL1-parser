#include <bits/stdc++.h>
#include "left_recur.h"
#include "firfollow.h"
using namespace std;
map<pair<string,string> ,string> parse_table; // ds for parse table for the ll[1] grammar
typedef set<string>::const_iterator s_it ;
typedef map<pair<string,string > ,string > :: const_iterator m_it;

/*Function to generate Parse table using First and Follow Sets 
	For every production  A -> alpha   in the grammar:

	1. If  alpha can derive a string starting with a (i.e., for all a in FIRST( alpha ) ,
		parse_table [A] [a] = A -> alpha
		 
	2. If  alpha can derive the empty string, eps , then, 
	   for all b that can follow a string derived from A (i.e., for all b in FOLLOW (A) ,
         parse_table [A] [b] = A -> alpha */
         
         
void generate_pt(){
				

	
	for(int i=0;i<nonTerminal.size();i++){
				string nt=nonTerminal[i];
				for(int j=0;j<prod[nt].size();j++){
					vector<string> token=split(prod[nt][j]);	
					int k;bool cont;
					for(k=0;k<token.size();k++ ){
						set<string> temp=first[token[k]];
						cont=false;
						for(s_it it=temp.begin();it!=temp.end();it++){
							if(*it!="eps"){
								parse_table[make_pair(nt,*it)]=prod[nt][j];
							
							}
							else cont=true;}
						if(!cont)break;
					}
					if(k==token.size() && cont == true){
							set<string> temp=follow[nt];
							for(s_it it=temp.begin();it!=temp.end();it++){
								parse_table[make_pair(nt,*it)]=prod[nt][j];
								
				}
		   }
		   
	}

//printing parse Table entries into file parse_table.txt
	ofstream fout("parse_table.txt");
	
		
	int cnt=1;
	for(m_it it =parse_table.begin();it!=parse_table.end();it++){
		pair<string,string> p=it->first;
		fout<<cnt<<"   parse_table [ "<<p.first<<" ] [ "<<p.second<<" ] = "<<it->second<<endl;
		cnt++;
		
	}

	fout.close();
}}
