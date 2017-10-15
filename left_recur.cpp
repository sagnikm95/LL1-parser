#include <bits/stdc++.h>
using namespace std;
# define MAXLEN 1000

map<string,vector<string> > prod,finalProd; //data structure for storing production rules
vector<string> nonTerminal; // data structure for maintaining list of non Terminals used

//Function to check if a non Terminal s has occured in any previous production rule
bool notFound(string s){
	for(int i=0;i<nonTerminal.size();i++)
		if(nonTerminal[i]==s)return false;
	return true;
}


//Function to parse file containing production rules for grammar
void parse(string name){
	ifstream input(name.c_str());
	string line;
	while(getline(input,line)){
		int i=0,j=0;
		/* Extracting left hand side of rule */
		char lhs[MAXLEN];
		while(line[i]==' ')i++;
	    while(line[i]!=' ' && line[i]!= '-') lhs[j++]=line[i++];
		lhs[j]='\0';
		string nt(lhs);
		if(notFound(lhs))
			nonTerminal.push_back(nt);
			
		while(line[i]!='>')i++;
		i++;
		while(line[i]==' ')i++;
		
		/* Extracting right hand side of rule */
		char rhs[MAXLEN];
		while(i<line.length()){
			j=0;
			// Rules corresponding to same non Terminal might be ' | ' separated
			while(line[i]!='|' && i<line.length())
				rhs[j++]=line[i++];
			rhs[j]='\0';
			while(line[i] == ' ' || line[i] == '|') i++;
			string t(rhs);
			prod[nt].push_back(t);
		}		
	}
}



// Function to eliminate Left Recusrion from Production Rules
void lr(){
	for(int i=0;i< nonTerminal.size();i++){
		string nt=nonTerminal[i];
		string new_nt=nt+"'";
		
		/*Remove non immediate Recursion */
		for(int k=0;k<i;k++){
			string prev=nonTerminal[k];
			
			for(int j=0;j<prod[nt].size();j++){
				if(prev == prod[nt][j].substr(0,prev.length()) && prod[nt][j][prev.length()]==' '){
				
					for(int l = 0; l < finalProd[prev].size(); l++)
						prod[nt].push_back(finalProd[prev][l] + prod[nt][j].substr(prev.length(), prod[nt][j].length() - prev.length()));
					prod[nt].erase(prod[nt].begin() + j);
				}
			}
		
		}
		
		
		/*Removing Immediate Left Recusrion 
			A -> A alpha | beta after lr removal becomes
				A -> beta A'
				A' -> alpha A' | eps
		*/
		
		bool leftRec=false;
		int restart=-1;
		for(int k=0;k<prod[nt].size();k++){
			if(k==restart)continue;
			string rhs=prod[nt][k];
			
			if(nt==rhs.substr(0,nt.length()) && rhs[nt.length()]==' '){

				if(leftRec==false){
					leftRec=true;
					restart=k;
					k=-1;					
					nonTerminal.push_back(new_nt);
					finalProd[new_nt].clear();
					finalProd[nt].clear();
					finalProd[new_nt].push_back("eps");}
				
				finalProd[new_nt].push_back(rhs.substr(nt.length(),rhs.length()-nt.length())+" "+new_nt);
			}

			else if (leftRec) 
			{		if(rhs!="eps")
					finalProd[nt].push_back(rhs+" "+new_nt);
					else
					finalProd[nt].push_back(new_nt);
			
			}
			else finalProd[nt].push_back(rhs);
		}

	
	}
}



//  Function that gets called in main() to remove left recursion from the grammar in file "name" 
void eliminate_lr(string name){
	
	parse(name);
	lr();
	/* New grammar after left recursion removal is written in "newGram.txt" */
	ofstream fout("newGram.txt");
	for(int i=0;i<nonTerminal.size();i++){
		int j;
		string nt=nonTerminal[i];
		fout<<nt<<" -> ";
		for(j=0;j<finalProd[nt].size()-1;j++)
			fout<<finalProd[nt][j]<<"|";
		fout<<finalProd[nt][j]<<endl;
	}
	fout.close();

}


