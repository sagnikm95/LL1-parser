#include<bits/stdc++.h>
using namespace std;
#define MAX_LINE_SIZE 1000
#define SPECIAL_CHAR_LEN 14



int cur_row,token_id;

// Each token in the parsed C like program will have following info associated with it
struct tok_n{
	string val; //token value
	string type; // token  type
	int row_no; // line no on which it appears
	int col_no; // column no on which it appears
};

map<int,tok_n> tok; // ds to maintain token id and associated attributes

typedef map<int,tok_n> :: const_iterator m_it;

// Function to print the position at which error occurred while compiling
void error_print(int token_no){
	cout<<"\tError occured at Line No "<<tok[token_no].row_no <<" Column No "<<tok[token_no].col_no <<" : "<<tok[token_no].val<<endl;
}

// Helper function that returns value of token
string getVal(int token_no){
	return tok[token_no].val;
}

// Helper function that returns line no of token
int getRow(int token_no){
	return tok[token_no].row_no;
}

// Helper function that returns column no of token
int getCol(int token_no){
	return tok[token_no].col_no;
}



//Helper Function to check for comments
 int isComment(char* line){
	if(line[0]=='/' && line[1]=='/')return 1;
	else return 0;
}


//Function to check for special symbols
int isSpecial(char c){

	int i;	
	char spl[]={'+','-','*','/','<','>','!','=',';',',','(',')','{','}'};
	for(i=0;i<SPECIAL_CHAR_LEN;i++){
		if(c==spl[i]){
			if(i<4) return 1; //arithmetic operator
			if(i>3 && i<7) return 2;//relational operator
			if(i==7) return 3;//asiignment operator
			return 4;} //Punctuation

	}
	return 0;
}		
	
//Function to check if token is a keyword
int isKeyWord(char* word){
	string t(word);
	
	if( (strcmp(word,"if")==0) || (strcmp(word,"else")==0) || (strcmp(word,"for")==0) || (strcmp(word,"int")==0) || (strcmp(word,"float")==0) || (strcmp(word,"void")==0) || (strcmp(word,"return")==0) ||
	 (strcmp(word ,"break")==0 ) || (strcmp(word,"continue")==0 ) || (strcmp(word,"read")==0) || (strcmp(word , "print")==0) || strcmp( word,"main")==0) {

	return 1;}
	else return 0;
}


//Function to classify token as keyword/ID/NUM	
void classify(char* word,int col,ofstream &fout){
	char* it;int flag=1;
	
	if(isComment(word)) return;
	if( isKeyWord(word) )
		{ 
			fout<<word<<" ";
			tok_n p;
			p.val=string(word);
			p.type="keyword";
			p.col_no=col;
			p.row_no=cur_row;
			tok[token_id]=p;
	
					token_id++;}
        else{
		if(word[0]=='0' && (word[1]=='x' || word[1]=='X') )
			it=word+2; //Check for octal and hexadecimal
		else it=word;
		for(;*it!='\0';it++){
			if((*it>='0' && *it <= '9') || *it=='.') continue;
			else{
				fout<<"id ";
				tok_n p;
			p.val=string(word);
			p.type="identifier";
			p.col_no=col;
			p.row_no=cur_row;
			tok[token_id]=p;
					token_id++;
				flag=0;break;}
		}
		if(flag) { 
			fout<<"constant ";
			tok_n p;
			p.val=string(word);
			p.type="constant";
			p.col_no=col;
			p.row_no=cur_row;
			tok[token_id]=p;
				token_id++;
            }
}
}


/* Function to break C like program into tokens for future parsing */
void tokenizer(string name){
char line[MAX_LINE_SIZE];
ofstream fout("token_file.txt");
char* ch, *start, *it;
int len,i;
cur_row=0;
//Taking file input from console
ifstream input(name.c_str());
string line2;
while(getline(input,line2)){
	if(line2.length()==0){
	cur_row++;
	continue;}
	strcpy(line,line2.c_str());
	char* token=strtok(line," \t\n");
	if(isComment(token)){ cur_row++;continue;}
	//Delimiting initially by space/tab/newline	
	while(token!=NULL){
		
		len=0;
		start=token;
		int cont=1;
		//Further delimiting by special symbol and classifying token
                for(ch = token ; *ch!='\0'; ch++){
			if(isSpecial(*ch) || *(ch+1)=='\0'){
				if(*ch=='/' && *(ch+1)=='/'){
					cont=0;break;}
				if(*(ch+1)=='\0' && !isSpecial(*ch))len++;
				char prev[len+1];
			
				for(i=0;i<len;i++){
					prev[i]=start[i];
				
					
					}	
				prev[i]='\0';
				if(len)			
				classify(prev,start-line,fout);
				len=0;
                if(isSpecial(*ch)==1) {
					fout<<*ch<<" ";
					string value(1,*ch);
					tok_n p;
				p.val=value;
				p.type="arith-op";
				p.col_no=ch-line;
				p.row_no=cur_row;
				tok[token_id]=p;
					token_id++;
				}
				
				else if(isSpecial(*ch)==2 || isSpecial(*ch)==3) {
					string type,value;
					if(isSpecial(*(ch+1))==3){
						fout<<*ch<<*(ch+1)<<" ";
						
						value+=*ch;value+=*(ch+1);
						ch++;
						type="rel-op";
						
					}
					else
						{
							fout<<*ch<<" ";
							value+=*ch;
							if(*ch=='=')type="assign-op";
							else type="relop";
						}
						tok_n p;
				p.val=value;
				p.type=type;
				p.col_no=ch-line;
				p.row_no=cur_row;
				tok[token_id]=p;
					token_id++;
				}
				
	
				else if(isSpecial(*ch)==4) {
					fout<<*ch<<" ";
					string value(1,*ch);
					tok_n p;
				p.val=value;
				p.type="punc-op";
				p.col_no=ch-line;
				p.row_no=cur_row;
				tok[token_id]=p;
					token_id++; }
				start=ch+1;    	
			
			}
			else len++;
		}
		if(!cont)break;
		token = strtok(NULL, " \t\n");
	}
cur_row++;
}
fout<<"$ ";
fout.close();

//The result of tokenisationis stored in file "symbol_table.txt"
ofstream f("symbol_table.txt");
for(m_it it = tok.begin();it!=tok.end();it++){
	tok_n p=it->second;
	f<<it->first<<"\t"<<p.val<<"\t\t"<<p.type<<"\t\t\tRow : "<<p.row_no<<"\t\tCol : "<<p.col_no<<endl;
}
f.close();

}
			 

/*Function called in main() that reads the C like program , tokenizes it and 
	returns a single string which is a collection of token for future parsing */
	
string get_token(string name){
	string to_parse;
	tokenizer(name);
	string line;
	ifstream input("token_file.txt");
	while(getline(input,line)){
		to_parse+=line;
	}
	
	return line;
}








