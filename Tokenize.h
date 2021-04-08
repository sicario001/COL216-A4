#ifndef TOKENIZE_H
#define TOKENIZE_H

#include<iostream>
#include<iomanip>
#include<fstream>
#include<string>
#include<algorithm>
#include<vector>
#include<sstream>
#include<unordered_map>
#include<map>
#include<set>
using namespace std;

extern unordered_map<string, int> reg_str_to_ind;
extern map<string, int> id_address;
extern vector<int> memory_vec;
extern vector<int> register_vec;
extern vector<vector<int>> instruction_processed;
extern vector<int> times_instruction_processed;
extern int currentLineIndex;
extern int cycles;

int check_comma(string s);
int count_comma(string x);
bool check_first_comma(string x);
vector<vector<string>> tokenize(istream& fin);

#endif
