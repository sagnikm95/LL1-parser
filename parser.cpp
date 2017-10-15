#include <bits/stdc++.h>
#include "firfollow.h"
#include "left_recur.h"
#include "parsetable.h"
#include "tokenize.h"
using namespace std;

// Contains relevant attributes and information of each token
struct info{
	string type; // data type of id
	int token_id; // token id -> acts as a direct link to the token
	string token_type; // token type as in function or identifier
	int scope; // Scope of usage
	string usage; // Type of usage -> declaration or reference
	int row; // Line No it apears in
	int col; // Col No it appears in
};

typedef map<pair<string,int> ,string> :: iterator it;
typedef map<pair<string,int> ,vector<info> > :: iterator m_it;

/* A list of each occurence of an identifier with associated attributes */
map<pair<string,int> , vector<info > > gl_tab; 
map<pair<string , int> , string > sym;  // ds to maintain all variables accessible in the current scope ->  


// Helper Function to find the scope in which a variable was declared
int findScope(string var){
	int declared_scope=-1;

	for(it i=sym.begin();i!=sym.end();i++){
		if(i->first.first==var)
			declared_scope=max(declared_scope,i->first.second);}
				
	return declared_scope;
}


// Helper Function to find what data type the variable 'var' is
string findType(string var){
	int cur=-1;
	string type;
	for(it i=sym.begin();i!=sym.end();i++){
		if(i->first.first==var)
			if(cur<i->first.second){
				cur=i->first.second;
				type=i->second;
			}
	}
	return type;			
	
}


// Function to delete all entries of varibales delared in scope no 'level' at the end of the scope
void update_table(int level,ofstream& fout2){
	
	for(it i=sym.begin();i!=sym.end();i++){
	
		if(i->first.second==level){
				sym.erase(i);}
	}
}


// Helper Function to print  Stack Contents during ll1 parsing 
void print(stack<string> &s,ofstream &fout){
	stack<string> temp;
	string ans;
	while(!s.empty()){
		string t=s.top();
		s.pop();
		temp.push(t);
	}
	while(!temp.empty()){
		string t=temp.top();
		ans+=t+" ";
		temp.pop();
		s.push(t);
	}
	
	fout<<ans<<endl<<endl;;
}



//FUnction called from main() to compile[parse] The tokenised C like program
void parse_input(string input){
	stack<string> s;
	s.push("$");
	s.push(START);
	bool accepted=true;
	
	ofstream fout("parse_stack_contents.txt");
	ofstream fout2("Symbol_table_2.txt");
	ofstream fout3("Rules_Applied.txt");
	bool been_declared=false;
	int cur_level=0;

	vector<string> token=split(input);
	int cur=0 ,size=token.size();
	int cur_ind=0;
	
	string data_type;
	print(s,fout);
	fout3<<"\t"<<cur<<"\t";
	fout3<<endl;
	
	
	/*For each non-terminal S ,We look at the next token a  
		And chose the production shown at parse_table[S][a]
		No entry would imply an error situation */
		
	while(!s.empty()){
		string s_top=s.top();
		
		if(s_top=="init_declarator")  // indicates variable declaration
			been_declared=true;
		if(s_top=="=" || token[cur]==";")
			been_declared=false;
		else if(s_top=="{") //start of scope
			cur_level++;
		else if(s_top=="}"){  // end of scope
	
			update_table(cur_level,fout2);
			cur_level--;
		
		}
		else if(s_top=="type"){
			data_type=getVal(cur);
		}
		else if(s_top=="id"){
			pair<string,int> t=make_pair(getVal(cur),cur_level);
			
			// if variable has not been declared yet in current scope  and we are trying to declare it -> VALID
			if(sym.count(t)!=1 && been_declared )
			{
				info p;
				p.type=data_type;
				p.row=getRow(cur);
				p.col=getCol(cur);
				p.token_type="identifier";
				p.usage="declaration";
				p.scope=cur_level;
				p.token_id=cur;
				gl_tab[t].push_back(p);
				sym[t]=data_type;
				
			
				}
			
			// if variable has  been declared  in current scope  and we are trying to declare it again  -> ERROR
			else if(sym.count(t)==1 && been_declared){
				accepted=false;
				cout<<"\n\n VARIABLE Redclared in this scope !! \n";
				error_print(cur);
				cout<<"\n\n";}
			
			
			/* Else trying to reference the variable */
			else{
			
			bool found=false;
			for(it i=sym.begin();i!=sym.end();i++){
				if(i->first.first==getVal(cur) && i->first.second <=cur_level){
					found=true;break;
				
				}
			}
			
			// Trying to reference a variable that has not been declared -> ERROR
			if(!found){
				accepted=false;
				cout<<"\n\n VARIABLE undeclared in this scope !! \n";
				error_print(cur);
				cout<<"\n\n";}
			
			
			// Valid Reference 	
			else{
				
				info p;
				p.type=findType(t.first);
				p.row=getRow(cur);
				p.col=getCol(cur);
				p.token_type="identifier";
				p.usage="reference";
				p.scope=cur_level;
				p.token_id=cur;
				int declared_scope=findScope(t.first);
				gl_tab[make_pair(t.first,declared_scope)].push_back(p);
				
				
			
			}	
		}
	}
		
		
		// LL1 parsing using input buffer , stack and parsing table
		
		if(s_top != token[cur]){
			if(parse_table.count(make_pair(s_top,token[cur]))==0){
				accepted=false;			
				break;}
			string cur_rule = parse_table[make_pair(s_top,token[cur])];
			
			vector<string> temp =split(cur_rule);
			s.pop();
			
			for(int i = temp.size()-1;i>=0;i--)
				if(temp[i]!="eps"){
					s.push(temp[i]);}
					print(s,fout);
					fout3<<"\t"<<cur<<"\t";
			fout3<<s_top<<" - > "<<cur_rule<<endl;
		}
		else{
		
			s.pop();
				
				cur++;
				print(s,fout);
				fout3<<"\t"<<cur<<"\t";
			fout3<<"match\n";
		}
	}
	
	fout.close();
	
	/* Printing Symbol Table Entries to File */
	for(m_it i =gl_tab.begin();i!=gl_tab.end();i++){
		fout2<<"[ Variable Name : "<<i->first.first<< " ; Declared_Scope : "<<i->first.second<< " ]  -> \n\n";
		fout2<<"Token Id\tToken_type\tScope_Used_In\tType\tUsage_type\tLine_no\tCol_No\n\n";
		vector<info> tmp=i->second;
		
		for(int j=0;j<tmp.size();j++)
			fout2<<tmp[j].token_id<<"\t\t"<<tmp[j].token_type<<"\t\t"<<tmp[j].scope<<"\t"<<tmp[j].type<<"\t"<<tmp[j].usage<<"\t"<<tmp[j].row<<"\t"<<tmp[j].col<<endl;
			fout2<<"\n\n";
		
	}
	fout2.close();
	
	fout3.close();
	if(accepted)cout<<"\n\nGiven Program has been accepted by parser\n";
	else
		{
			if(cur!=token.size()){
			cout<<"\n\nInvalid Statement\n";
			error_print(cur);}
			cout<<"\n\nGiven Program has NOT been accepted by parser\n";}
			
}
