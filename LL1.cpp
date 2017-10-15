#include <bits/stdc++.h>
#include "firfollow.h"
#include "left_recur.h"
#include "parsetable.h"
#include "parser.h"
#include "tokenize.h"
using namespace std;

int main(){
	string to_parse =get_token("input3.txt");
	cout<<"String to Match : \n "<<to_parse<<endl<<endl<<endl;;
	eliminate_lr("my_c_gram.txt");

	firfol_compute();

	generate_pt();
	
	parse_input(to_parse);
	return 0;
}
