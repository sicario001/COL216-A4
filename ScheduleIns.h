#ifndef SCHEDULEINS_H
#define SCHEDULEINS_H

#include<iostream>
#include<vector>
#include<queue>
#include<deque>
#include<unordered_map>
#include<map>
#include<string>
#include<algorithm>

using namespace std;
extern unordered_map<string, int> reg_str_to_ind;
extern vector<string> reg_ind_to_str;
extern vector<int> memory_vec;
extern vector<int> row_buffer;
extern vector<int> register_vec;
extern map<int, int> memory_changes;
extern int num_row_buffer_updates;
extern int num_row_activation;
extern int num_row_writeback;

class ScheduleIns{

private:
	vector<deque<vector<int>>> dram_requests;
	vector<bool> isBusyRegRead;
	vector<bool> isBusyRegWrite;
	queue<int> reg_read;
	queue<int> reg_write;
	int cycle;
	int row_access_delay;
	int col_access_delay;
	int row_buffer_ind ;
	vector<int> currDRAMRequest;
	bool emptyDRAMRequests();
	bool isSafeIns(vector<int>&dep_reg_read, vector<int>&dep_reg_write);
	void updateCurrDRAMRequest();
	void processCurrDRAMRequest();
	vector<int> nextDRAMRequest();
	void updateDependencies(vector<int>& req);
	void removeDependencies();
	int getEndCycle(vector<int>& req);
	int getRowInd(int addr);
	int getColInd(int addr);
	void copy_row_buffer_to_row(int row_val, int start_time);
	void copy_row_to_row_buffer(int row_val, int start_time);
	void copy_register_val_to_buffer(int r_index, int col_val, int start_time);
	void copy_buffer_val_to_register(int r_index, int col_val, int start_time);
	
	
public:
	ScheduleIns(){}
	ScheduleIns(int row_access, int col_access){
		dram_requests = vector<deque<vector<int>>>(1024, deque<vector<int>>());
		isBusyRegRead = vector<bool>(32, false);
		isBusyRegWrite = vector<bool>(32, false);
		cycle = 0;
		row_access_delay = row_access;
		col_access_delay = col_access;
		
		currDRAMRequest = {-1, -1, -1, -1, -1};
		row_buffer_ind = -1;
	}
	
	void pushDRAMRequest(vector<int> request);
	int getCycle();
	void cycleUpdate(vector<int>&dep_reg_read, vector<int>&dep_reg_write);
	void processRemReq();
};

#endif
