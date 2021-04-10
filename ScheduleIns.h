#ifndef SCHEDULEINS_H
#define SCHEDULEINS_H

#include<vector>
#include<queue>
#include<deque>

#include "Dram.h"

using namespace std;


extern Dram dram;

class ScheduleIns{

private:
	vector<deque<vector<int>>> dram_requests;
	vector<int> isBusyRegStore;
	vector<bool> isBusyRegWrite;
	vector<bool> RegValChanged;
	queue<int> reg_read;
	queue<int> reg_write;
	vector<int> currDRAMRequest;
	bool emptyDRAMRequests();
	bool isSafeIns(vector<int>&dep_reg_read, vector<int>&dep_reg_write, vector<int>& reg_to_store);
	void updateCurrDRAMRequest();
	void processCurrDRAMRequest();
	vector<int> nextDRAMRequest();
	void updateDependencies(vector<int>& req);
	void removeDependencies();
	bool checkDependencies(int type, int reg, vector<int>& req);
	void removeNonDependentRegs();
	int getEndCycle(vector<int>& req);
	int getRowInd(int addr);
	int getColInd(int addr);
	
public:
	ScheduleIns(){
		dram_requests = vector<deque<vector<int>>>(1024, deque<vector<int>>());
		isBusyRegStore = vector<int>(32, 0);
		isBusyRegWrite = vector<bool>(32, false);
		RegValChanged = vector<bool>(32, false);
		currDRAMRequest = {-1, -1, -1, -1, -1};
	}
	
	void pushDRAMRequest(vector<int> request);
	void cycleUpdate(vector<int> dep_reg_read, vector<int> dep_reg_write, vector<int> reg_to_store);
	void processRemReq();
};

#endif
