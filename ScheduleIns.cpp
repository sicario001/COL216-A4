#include "ScheduleIns.h"
// currDRAMRequest = [type, register, mem_address, end_time_cycles, alt_reg]
// type = 0      :       lw
// type = 1      :       sw
// request = [type, register, mem_address, alt_reg]

int ScheduleIns::getRowInd(int address){
	return address/1024;
}
int ScheduleIns::getColInd(int address){
	return (address%1024)/4;
}

int ScheduleIns::getEndCycle(vector<int>& req){
	int row = getRowInd(req[2]);
    int end_time = dram.getCycle();
    if (req[0]==1){
	    if (row!=dram.row_buffer_ind){
			if (dram.row_buffer_ind!=-1){
	        	dram.copy_row_buffer_to_row(dram.row_buffer_ind, end_time);
	        	end_time+= dram.row_access_delay;
	    	}
	        dram.copy_row_to_row_buffer(row, end_time);
	        end_time+= dram.row_access_delay;
	        dram.copy_register_val_to_buffer(req[1], getColInd(req[2]), end_time);
	        end_time+=dram.col_access_delay;

	    }
	    else{
	        dram.copy_register_val_to_buffer(req[1], getColInd(req[2]), end_time);
	        end_time += dram.col_access_delay;
	    }
	}
    else{
        if (row!=dram.row_buffer_ind){
        	if (dram.row_buffer_ind!=-1){
            	dram.copy_row_buffer_to_row(dram.row_buffer_ind, end_time);
            	end_time+= dram.row_access_delay;
        	}
            dram.copy_row_to_row_buffer(row, end_time);
            end_time+=dram.row_access_delay;
            dram.copy_buffer_val_to_register(req[1], getColInd(req[2]), end_time);
            end_time+=dram.col_access_delay;
        }
        else{
            dram.copy_buffer_val_to_register(req[1], getColInd(req[2]), end_time);
            end_time += dram.col_access_delay;
        }
    }
    return end_time-1;
}
void ScheduleIns::updateCurrDRAMRequest(){
	if (dram.getCycle() == currDRAMRequest[3]+1 || currDRAMRequest[0]==-1){
		currDRAMRequest = nextDRAMRequest();
		// cout<<currDRAMRequest[0]<<" "<<currDRAMRequest[1]<<" "<<currDRAMRequest[2]<<" "<<currDRAMRequest[3]<<" "<<currDRAMRequest[4]<<"\n";
	}
}

vector<int> ScheduleIns::nextDRAMRequest(){
	
	removeDependencies();
	removeNonDependentRegs();
	

	int row = getRowInd(currDRAMRequest[2]);

	if (currDRAMRequest[0]!=-1 && !dram_requests[row].empty()){
		vector<int> nextReq = dram_requests[row].front();
		dram_requests[row].pop_front();

		return {nextReq[0], nextReq[1], nextReq[2], getEndCycle(nextReq), nextReq[3]};
	}
	else{
		if (!reg_read.empty()){
			for (int i=0; i<1024; i++){
				if (!dram_requests[i].empty()){
					for(auto req_it : dram_requests[i]){
						if (checkDependencies(0, reg_read.front(), req_it)){
							reg_read.pop();
							auto req = dram_requests[i].front();
							dram_requests[i].pop_front();
							return {req[0], req[1], req[2], getEndCycle(req), req[3]};
						}
					}
				}
			}

		}
		if (!reg_write.empty()){
			for (int i=0; i<1024; i++){
				if (!dram_requests[i].empty()){
					for(auto req_it : dram_requests[i]){
						if (checkDependencies(1, reg_write.front(), req_it)){
							reg_write.pop();
							auto req = dram_requests[i].front();
							dram_requests[i].pop_front();
							return {req[0], req[1], req[2], getEndCycle(req), req[3]};
						}
					}
				}
			}

		}
		if (!emptyDRAMRequests()){
			for (int i=0; i<1024; i++){
				if (!dram_requests[i].empty()){
					vector<int> nextReq = dram_requests[i].front();
					dram_requests[i].pop_front();
					return {nextReq[0], nextReq[1], nextReq[2], getEndCycle(nextReq), nextReq[3]};

				}
			}
		}
		else{
			return {-1, -1, -1, -1, -1};
		}
	}
	return {-1, -1, -1, -1, -1};
}

void ScheduleIns::processCurrDRAMRequest(){
	dram.setCycle(currDRAMRequest[3]+1);
	updateCurrDRAMRequest();
}

void ScheduleIns::pushDRAMRequest(vector<int>request){
	updateCurrDRAMRequest();
	int row = getRowInd(request[2]);
	dram_requests[row].push_back(request);
	updateDependencies(request);
}
void ScheduleIns::updateDependencies(vector<int>& req){
	if (req[0]==0){
		isBusyRegWrite[req[1]] = true;
		if (req[3]!=-1){
			isBusyRegStore[req[3]]++;
		}
	}
	else if (req[0]==1){
		isBusyRegStore[req[1]] = true;
		RegValChanged[req[1]] = false;
		if (req[3]!=-1){
			isBusyRegStore[req[3]]++;
		}
	}
}
void ScheduleIns::removeDependencies(){
	if (currDRAMRequest[0] ==0){
		isBusyRegWrite[currDRAMRequest[1]] = false;
		if (currDRAMRequest[4]!=-1){
			isBusyRegStore[currDRAMRequest[4]]--;
			if (isBusyRegStore[currDRAMRequest[4]]==0){
				RegValChanged[currDRAMRequest[4]] = false;
			}
		}
	}
	if (currDRAMRequest[0] ==1){
		isBusyRegStore[currDRAMRequest[1]]--;
		if (isBusyRegStore[currDRAMRequest[1]]==0){
			RegValChanged[currDRAMRequest[1]] = false;
		}
		if (currDRAMRequest[4]!=-1){
			isBusyRegStore[currDRAMRequest[4]]--;
			if (isBusyRegStore[currDRAMRequest[4]]==0){
				RegValChanged[currDRAMRequest[4]] = false;
			}
		}
	}
}

bool ScheduleIns::checkDependencies(int type, int reg, vector<int>& req){
	// type = 0 : reg_read
	// type = 1 : reg_write
	if (type==0){
		if (req[0]==0){
			if (req[3]==reg){
				return true;
			}
		}
		else{
			if (req[1]==reg || req[3]==reg){
				return true;
			}
		}
	}
	else{
		if (req[0]==0){
			if (req[1]==reg){
				return true;
			}
		}
	}
	return false;
}

void ScheduleIns::removeNonDependentRegs(){
	while (!reg_read.empty()){
		if (isBusyRegStore[reg_read.front()]==0 || !RegValChanged[reg_read.front()]){
			reg_read.pop();
		}
		else{
			break;
		}
	}
	while (!reg_write.empty()){
		if (!isBusyRegWrite[reg_write.front()]){
			reg_write.pop();
		}
		else{
			break;
		}
	}
}

bool ScheduleIns::emptyDRAMRequests(){
	for (auto x: dram_requests){
		if (!x.empty()){
			return false;
		}
	}
	return true;
}
bool ScheduleIns::isSafeIns(vector<int>&dep_reg_read, vector<int>&dep_reg_write, vector<int>& reg_to_store){
	for (int x:dep_reg_read){
		if (x!=-1 && isBusyRegWrite[x]==true){
			return false;
		}
	}
	for (int x:dep_reg_write){
		if (x!=-1 && isBusyRegWrite[x]==true){
			return false;
		}
	}
	for (int x:reg_to_store){
		if (x!=-1 && isBusyRegStore[x] && RegValChanged[x]==true){
			return false;
		}
	}
	return true;

}

void ScheduleIns::cycleUpdate(vector<int> dep_reg_read, vector<int> dep_reg_write, vector<int> reg_to_store){
	dram.incCycle();
	queue<int>().swap(reg_read);
	queue<int>().swap(reg_write);
	// check this
	
	if (!isSafeIns(dep_reg_read, dep_reg_write, reg_to_store)){
		for (auto x: reg_to_store){
			if (x!=-1){
				reg_read.push(x);
			}
		}
		for (auto x: dep_reg_write){
			if (x!=-1){
				reg_write.push(x);
			}
		}

		for (auto x: dep_reg_read){
			if (x!=-1){
				reg_write.push(x);
			}
		}
	}
	updateCurrDRAMRequest();
	while (!isSafeIns(dep_reg_read, dep_reg_write, reg_to_store)){
		processCurrDRAMRequest();
	}

	for (auto x: dep_reg_write){
		if (x!=-1 && isBusyRegStore[x]){
			RegValChanged[x] = true;
		}
	}


}
void ScheduleIns::processRemReq(){
	if (!emptyDRAMRequests()){
		if (currDRAMRequest[0]==-1){
			dram.incCycle();
		}
		updateCurrDRAMRequest();
		while (currDRAMRequest[0]!=-1){
			processCurrDRAMRequest();
		}
		dram.decCycle();
	}
}
