#ifndef PROCESS_H
#define PROCESS_H

#include<iostream>
#include<iomanip>
#include<string>

#include "ScheduleIns.h"

using namespace std;

extern unordered_map<string, int> reg_str_to_ind;
extern vector<string> reg_ind_to_str;
extern vector<int> memory_vec;
extern vector<int> register_vec;
extern vector<vector<int>> instruction_processed;
extern vector<int> times_instruction_processed;
extern int currentLineIndex;
extern Dram dram;



int get_address(int register_ind, int Addr);

void printState();

void add(int ri_index, int rj_index, int rk_index);
void addi(int ri_index,int rj_index, int val);
void sub(int ri_index, int rj_index, int rk_index);
void mul(int ri_index, int rj_index, int rk_index);
void beq(int ri_index, int rj_index, int register_ind, int Addr);
void bne(int ri_index, int rj_index, int register_ind, int Addr);
void slt(int ri_index, int rj_index, int rk_index);
void j(int register_ind, int Addr);
void sw(int r_index, int register_ind, int Addr);
void lw(int r_index, int register_ind, int Addr);

void process_ins();

#endif
