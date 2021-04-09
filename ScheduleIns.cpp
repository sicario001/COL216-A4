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

void ScheduleIns::copy_row_buffer_to_row(int row_val, int start_time){
    for (int i=(256*row_val); i<(256*(row_val+1)); i++){
        memory_vec[i] = row_buffer[i-(256*row_val)];
    	
    }
    num_row_writeback++;
    cout<<"Cycle "<<start_time<<"-"<<start_time+row_access_delay-1<<" : "<<"DRAM : Writeback row "<<row_val<<"\n";
}
void ScheduleIns::copy_row_to_row_buffer(int row_val, int start_time){
	for (int i=(256*row_val); i<(256*(row_val+1)); i++){
    	row_buffer[i-(256*row_val)] = memory_vec[i];
    }
    row_buffer_ind = row_val;
    num_row_buffer_updates++;
    num_row_activation++;
    cout<<"Cycle "<<start_time<<"-"<<start_time+row_access_delay-1<<" : "<<"DRAM : Activate row "<<row_val<<"\n";
}
void ScheduleIns::copy_register_val_to_buffer(int r_index, int col_val, int start_time){
	
    row_buffer[col_val] = register_vec[r_index];
    num_row_buffer_updates++;
	cout<<"Cycle "<<start_time<<"-"<<start_time+col_access_delay-1<<" : "<<"DRAM : Column access, column "<<col_val*4<<"-"<<(col_val+1)*4-1<<" of row buffer updated to "<<register_vec[r_index]<<"\n";
	int memoryAddress = (row_buffer_ind*1024+(4*col_val));
	memory_changes[memoryAddress] = register_vec[r_index];
}
void ScheduleIns::copy_buffer_val_to_register(int r_index, int col_val, int start_time){
	register_vec[r_index] = row_buffer[col_val];

	cout<<"Cycle "<<start_time<<"-"<<start_time+col_access_delay-1<<" : "<<"DRAM : Column access, register "<<reg_ind_to_str[r_index]<<" updated to "<<register_vec[r_index]<<"\n";
}

int ScheduleIns::getEndCycle(vector<int>& req){
	int row = getRowInd(req[2]);
    int end_time = cycle;
    if (req[0]==1){
	    if (row!=row_buffer_ind){
			if (row_buffer_ind!=-1){
	        	copy_row_buffer_to_row(row_buffer_ind, end_time);
	        	end_time+= row_access_delay;
	    	}
	        copy_row_to_row_buffer(row, end_time);
	        end_time+= row_access_delay;
	        copy_register_val_to_buffer(req[1], getColInd(req[2]), end_time);
	        end_time+=col_access_delay;

	    }
	    else{
	        copy_register_val_to_buffer(req[1], getColInd(req[2]), end_time);
	        end_time += col_access_delay;
	    }
	}
    else{
        if (row!=row_buffer_ind){
        	if (row_buffer_ind!=-1){
            	copy_row_buffer_to_row(row_buffer_ind, end_time);
            	end_time+= row_access_delay;
        	}
            copy_row_to_row_buffer(row, end_time);
            end_time+=row_access_delay;
            copy_buffer_val_to_register(req[1], getColInd(req[2]), end_time);
            end_time+=col_access_delay;
        }
        else{
            copy_buffer_val_to_register(req[1], getColInd(req[2]), end_time);
            end_time += col_access_delay;
        }
    }
    return end_time-1;
}
void ScheduleIns::updateCurrDRAMRequest(){
	if (cycle == currDRAMRequest[3]+1 || currDRAMRequest[0]==-1){
		currDRAMRequest = nextDRAMRequest();
		// cout<<currDRAMRequest[0]<<" "<<currDRAMRequest[1]<<" "<<currDRAMRequest[2]<<" "<<currDRAMRequest[3]<<" "<<currDRAMRequest[4]<<"\n";
	}
}

vector<int> ScheduleIns::nextDRAMRequest(){
	
	removeDependencies();

	while (!reg_read.empty()){
		if (!isBusyRegStore[reg_read.front()]){
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
						if (req_it[0]==0 && (req_it[1]==reg_read.front())){
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
						if (req_it[0]==0 && ((req_it[1]==reg_write.front())||(req_it[3]==reg_write.front()))){
							reg_write.pop();
							auto req = dram_requests[i].front();
							dram_requests[i].pop_front();
							return {req[0], req[1], req[2], getEndCycle(req), req[3]};
						}
						else if (req_it[0]==1 && ((req_it[1]==reg_write.front())||(req_it[3]==reg_write.front()))){
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
	cycle = currDRAMRequest[3]+1;
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
			isBusyRegStore[req[3]] = true;
		}
	}
	else if (req[0]==1){
		isBusyRegStore[req[1]] = true;
		if (req[3]!=-1){
			isBusyRegStore[req[3]] = true;
		}
	}
}
void ScheduleIns::removeDependencies(){
	if (currDRAMRequest[0] ==0){
		isBusyRegWrite[currDRAMRequest[1]] = false;
		if (currDRAMRequest[4]!=-1){
			isBusyRegStore[currDRAMRequest[4]] = false;
		}
	}
	if (currDRAMRequest[0] ==1){
		isBusyRegStore[currDRAMRequest[1]] = false;
		if (currDRAMRequest[4]!=-1){
			isBusyRegStore[currDRAMRequest[4]] = false;
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
		if (x!=-1 && isBusyRegStore[x]==true){
			return false;
		}
	}
	return true;

}

void ScheduleIns::cycleUpdate(vector<int> dep_reg_read, vector<int> dep_reg_write, vector<int> reg_to_store){
	cycle++;
	queue<int>().swap(reg_read);
	queue<int>().swap(reg_write);
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
	}
	updateCurrDRAMRequest();
	while (!isSafeIns(dep_reg_read, dep_reg_write, reg_to_store)){
		processCurrDRAMRequest();
	}


}
void ScheduleIns::processRemReq(){
	if (!emptyDRAMRequests()){
		if (currDRAMRequest[0]==-1){
			cycle++;
		}
		updateCurrDRAMRequest();
		while (currDRAMRequest[0]!=-1){
			processCurrDRAMRequest();
		}
		cycle--;
	}
}

int ScheduleIns::getCycle(){
	return cycle;
}
