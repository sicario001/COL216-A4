#ifndef PARSE_H
#define PARSE_H

#include<iostream>
#include <iomanip>
#include<fstream>
#include<string>
#include<algorithm>
#include<vector>
#include<sstream>
#include<unordered_map>
#include<map>
using namespace std;

extern unordered_map<string, int> reg_str_to_ind;
extern map<string, int> id_address;
extern vector<int> memory_vec;
extern vector<int> register_vec;
extern vector<vector<int>> instruction_processed;
extern vector<int> times_instruction_processed;
extern int currentLineIndex;
extern int cycles;

bool check_all_digits(string num_str);
bool is_correct_instruction_address(int ins_addr);
bool is_correct_memory_address(int mem_addr);
bool is_valid_immediate_val(string num_str);

pair<int, int> address_as_register_offset(string reg_off);

int check_ins_add(vector<string> &inst);
int check_ins_sub(vector<string> &inst);
int check_ins_mul(vector<string> &inst);
int check_ins_beq(vector<string> &inst);
int check_ins_bne(vector<string> &inst);
int check_ins_j(vector<string> &inst);
int check_ins_addi(vector<string> &inst);
int check_ins_slt(vector<string> &inst);
int check_ins_lw(vector<string> &inst);
int check_ins_sw(vector<string> &inst);

int check_instructions(vector<vector<string>> &instruction_vector_tokens);
#endif
