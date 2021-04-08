#include "Tokenize.h"

int check_comma(string s){
    string tmp_string = "";
    for (int i=0; i<s.size(); i++){
        if (s[i]!=' ' && s[i]!= '\t'){
            tmp_string.push_back(s[i]);
        }
    }
    if (tmp_string[0]==',' || tmp_string[(tmp_string.size())-1]==','){
        return 0;
    }
    for (int i=0; i<(tmp_string.size())-1; i++){
        if (tmp_string[i]==',' && tmp_string[i+1]==','){
            return 0;
        }
    }
    return 1;
}
int count_comma(string x){
    int cnt = 0;
    for (int i=0; i<x.size(); i++){
        if (x[i]==',')
            cnt++;
    }
    return cnt;
}
bool check_first_comma(string x){
    int token_visit = 0;
    int ws_visit = 0;
    int comma_visit = 0;
    for (int i=0; i<x.size(); i++){
        if (x[i]!=' '&& x[i]!=','){
            if (token_visit==ws_visit)
                token_visit++;
        }
        else if (x[i]==' '){
            if (token_visit>ws_visit)
                ws_visit++;
        }
        else{
            if (token_visit<2){
                return false;
            }
            return true;
        }
    }
    return true;
}
string strip_ws(string s){
	int start, end;
	for (int i=0; i<(int)s.size(); i++){
		if (s[i]!=' ' && s[i]!='\t'){
			start = i;
			break;
		}
	}
	for (int i=(int)s.size()-1; i>=0; i--){
		if (s[i]!=' ' && s[i]!= '\t'){
			end = i;
			break;
		}
	}
	return s.substr(start, end-start+1);
}
bool check_id(string s){
	set <char> allowed_first_char;
	for (char x = 'a'; x<='z'; x++){
		allowed_first_char.insert(x);
	}
	for (char x = 'A'; x<='Z'; x++){
		allowed_first_char.insert(x);
	}
	allowed_first_char.insert('_');
	set <char> allowed_char;
	for (auto x: allowed_first_char){
		allowed_char.insert(x);
	}
	for (char x = '0'; x<='9'; x++){
		allowed_char.insert(x);
	}
	for (int i=0; i<(int)s.size(); i++){
		if (i==0){
			if (allowed_first_char.find(s[i])==allowed_first_char.end()){
				return false;
			}
		}
		else{
			if (allowed_char.find(s[i])==allowed_char.end()){
				return false;
			}
		}
	}
	return true;
}
string check_label(string s){

	string s_ws_removed = strip_ws(s);
	
	if (s_ws_removed[(int)s_ws_removed.size()-1]!=':'){
		return "0";
	}
	string id = strip_ws(s_ws_removed.substr(0, (int)s_ws_removed.size()-1));
	if (check_id(id)){
		return id;
	}
	return "0";
}
bool check_all_ws(string s){
	for (auto x:s){
		if (x!=' ' && x!= '\t'){
			return false;
		}
	}
	return true;
}
vector<vector<string>> tokenize(istream& fin){
    string read_line;
    vector<string> instruction_vector;
    vector<vector<string>> instruction_vector_tokens(0, vector<string>(0));
    while (getline(fin, read_line)){
        // cout<<read_line<<"\n";
        if (!check_all_ws(read_line)){
        	string id = check_label(read_line);
        	if (id!="0"){
        		if (id_address.find(id)!=id_address.end()){
        			stringstream oss;
            		oss<<"Multiple labels with same id \""<<id<<"\"\n";
            		throw oss.str();
        		}
        		id_address[id] = (int)instruction_vector.size();
        		continue;
        	}
            instruction_vector.push_back(read_line);
        }
    }
    for (int i=0; i<instruction_vector.size(); i++){
        string x = instruction_vector[i];
        replace(x.begin(), x.end(), '\t', ' ');
        int flag = check_comma(x);
        if (flag==0){
            stringstream oss;
            oss<<"Invalid syntax at line "<<i+1<<"\n";
            throw oss.str();
        }
        int cnt_comma = count_comma(x);
        bool flag1 = check_first_comma(x);
        if (flag1==false){
            stringstream oss;
            oss<<"Invalid syntax at line "<<i+1<<"\n";
            throw oss.str();
        }
        replace(x.begin(), x.end(), ',', ' ');
        string token;
        stringstream ss(x);
        instruction_vector_tokens.push_back(vector<string>(0));
        while(getline(ss, token, ' ')){
            if (token!="")
                instruction_vector_tokens[i].push_back(token);
        }
        if (cnt_comma!=(instruction_vector_tokens[i].size()-2)){
            stringstream oss;
            oss<<"Invalid syntax at line "<<i+1<<"\n";
            throw oss.str();
        }
    }

    // for (auto x: instruction_vector_tokens){
    //     for (auto y: x){
    //         cout<<y<<"-";
    //     }
    //     cout<<"\n";
    // }
    return instruction_vector_tokens;
}