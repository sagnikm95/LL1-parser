#include <bits/stdc++.h>
#include "left_recur.h"
using namespace std;
# define MAXLEN 100 
typedef set<string>::const_iterator s_it ;
typedef map< string , set<string> > ::const_iterator m_it ;
typedef map<string , bool> :: const_iterator m_it2;

set<string> terminals; // ds to maintain list of terminals
map<string,set<string> > first;  // ds to maintain first set of each Terminal and non Terminal 
map<string,set<string> > follow; // ds to maintain follow set of each non Terminal
map<string,bool> visited; 
string START; // Start Symbol for grammar



/* Function to tokenize a string s taking space as a delimiter ;
		Function return a vector of string containing each token */
		
vector<string> split(string &s){
	
 	vector<string> result;
 	istringstream iss(s);
	for(string s; iss >> s; )
    	result.push_back(s);
    	return result;
}


/* Helper Function to see if a string is a Terminal */
bool isTerminal(string exp){
	return !(prod.count(exp)!=0);
}

/* Helper Function to see if a string is epsilon */
bool isEps( set<string> s){
	return (s.find("eps")!=s.end());
}

/* Helper Function to perform Union of sets ; 
		set S = S U { str }
		flag=1 for first ; 0 for follow */
		
void setUnion(string str,set<string> s,bool flag){

	if(flag){
	for(s_it it=s.begin();it!=s.end();it++)
		first[str].insert(*it);
	}
	else{
		for(s_it it=s.begin();it!=s.end();it++)
		follow[str].insert(*it);
	}
}	


 /* Function to generate first values of each Terminal */
void getFirstTerminal(){
	for(int i=0;i<nonTerminal.size();i++){
		string nt= nonTerminal[i];
		for(int j=0;j<prod[nt].size();j++){
			vector<string> token=split(prod[nt][j]);
			for(int k=0;k<token.size();k++){
				if(isTerminal(token[k])){
					if(token[k]!="eps")
					terminals.insert(token[k]);
					first[token[k]].insert(token[k]);}}}}
}



 /* Function to compute First Set of each Non Terminal "exp" */
 
set<string> getFirst(string exp){
	
	//memoize for efficiency
	if(first.find(exp)!=first.end())
		return first[exp];
		
	// First of a terminal is the terminal itself	
	if (isTerminal(exp)){
		set<string> s;
		s.insert(exp);
		return s;
	}	
	
	
	for(int i=0;i<prod[exp].size();i++){
		
		vector<string> token = split(prod[exp][i]);
		
		/*  -> If X is a non-terminal, and X -> Y1 Y2 … Yk is a production, 
			and eps is in all of FIRST(Y1), …, FIRST(Yk), then add eps to FIRST(X).
			-> If X is a non-terminal, and X -> Y1 Y2 … Yk is a production, 
			   then add a to FIRST(X) if for some i, a is in FIRST(Yi), and eps is in all of FIRST(Y1), …, FIRST(Yi-1).	*/
			   
		for(int i = 0;i < token.size(); i++){
			if(token[i]==exp)	
					break;
			set<string> temp = getFirst(token[i]);
			setUnion(exp,temp,1);
			if(!isEps(temp))
				break;
			if(i!=token.size()-1)
				first[exp].erase("eps");
		}
	
	}
	return first[exp];	
}


/* Function to compute Follow set of non terminal exp ; A DFS type approach has been used */

set<string> getFollow(string exp){
	
	// to avoid cycle we keep a visited array to tract non terminals seen till now
	visited[exp]=true;
	
	if(follow.find(exp)!=follow.end() )
		return follow[exp];
	
	// $ is in the follow of START symbol
	if(exp==START)
		follow[exp].insert("$");
	
	/* -> If $ is the input end-marker, and S is the start symbol, $ belongs tp FOLLOW(S).
		-> If there is a production, A -> aBß, then (FIRST(ß) – eps) is a subset of  FOLLOW(B).
		-> If there is a production, A -> aB, or a production A -> aBß, where  eps is in  FIRST(ß), then FOLLOW(B)= FOLLOW(B) U FOLLOW (A) */
			
	set<string> s;	
	for(int i=0;i<nonTerminal.size();i++){
		string nt= nonTerminal[i];

		for(int j=0;j<prod[nt].size();j++){
			vector<string> token=split(prod[nt][j]);
		
			for(int k=0;k<token.size();){
			
				if(token[k]==exp){
							
					k++;
					
					while(k!=token.size()){
						set<string> t=first[token[k]];
						
						setUnion(exp,t,0);
						k++;
						if(isEps(t))
							follow[exp].erase("eps");	
						else
							break;
						
					}
					if(k==token.size() && token[k-1]==exp && !visited[nt])
						setUnion(exp, getFollow(nt),0);
					if(k==token.size() && isEps(first[token[k-1]]) && nt!=exp &&!visited[nt])
						setUnion(exp, getFollow(nt),0);	
				}
				else k++;
			}
		}
	}
	return s;;	
}


// Function gets called in main() to compute first and follow set for the given grammar
void firfol_compute(){
		prod.clear();
		parse("newGram.txt");
		
		START=nonTerminal[0];
		for(int i= 0;i<nonTerminal.size();i++){
			string nt=nonTerminal[i];
			setUnion(nt,getFirst(nt),1);
		}
		
		getFirstTerminal();
		
		// First and Follow sets are written tp " First_Follow_sets.txt" 
		ofstream fout("First_Follow_sets.txt");
   		for(m_it i= first.begin();i!=first.end();i++){
	   		set<string> temp=i->second;
	   		fout<<"FIRST  "<<i->first<< "  -> ";
	   		for(s_it it=temp.begin();it!=temp.end();it++)
	   			fout<<*it<<" ";
	   			fout<<endl;
	   		
		   }
			
			fout<<"\n\n_____________________________________________________\n\n";
			
	  	for(int j=0;j<nonTerminal.size();j++){
	   		string nt=nonTerminal[j];
	   	
	   		for(int k=0;k<nonTerminal.size();k++)
	   			visited[nonTerminal[k]]=false;
	   			visited[nt]=true;
	   		setUnion(nt,getFollow(nt),0);
	   }
	   
	   	for(int j=0;j<nonTerminal.size();j++){
	   		set<string> temp=follow[nonTerminal[j]];
	   		fout<<"FOLLOW  "<<nonTerminal[j]<< "  -> ";
	   		for(s_it it=temp.begin();it!=temp.end();it++)
	   			fout<<*it<<" ";
	   			fout<<endl;
		   }
		   fout.close();	

}
