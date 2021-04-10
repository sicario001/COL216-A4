#ifndef DRAM_H
#define DRAM_H

#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>

using namespace std;

extern vector<int> register_vec;
extern unordered_map<string, int> reg_str_to_ind;
extern vector<string> reg_ind_to_str;

class Dram{
	private:
		int cycle;
		int actual_cycle;
		vector<int> memory_vec;
		map<int, int> memory_changes;
		vector<int> row_buffer;
		int num_row_buffer_updates;
		int num_row_activation;
		int num_row_writeback;
	public:
		Dram(){}
		Dram(int row_access, int col_access){
			cycle = 0;
			actual_cycle = 0;
			memory_vec = vector<int>(1<<18,0);
			row_buffer = vector<int>(256,0);
			num_row_buffer_updates = 0;
			num_row_activation = 0;
			num_row_writeback = 0;
			row_access_delay = row_access;
			col_access_delay = col_access;
			row_buffer_ind = -1;
		}
		int row_access_delay;
		int col_access_delay;
		int row_buffer_ind;
        void print_mem_state();
		int getCycle();
		int getFinalCycle();
		void incCycle();
		void decCycle();
		void setCycle(int c);
		void copy_row_buffer_to_row(int row_val, int start_time);
		void copy_row_to_row_buffer(int row_val, int start_time);
		void copy_register_val_to_buffer(int r_index, int col_val, int start_time);
		void copy_buffer_val_to_register(int r_index, int col_val, int start_time);
};
#endif