#include "Dram.h"

void Dram::print_mem_state(){
    cout<<"\nMemory content at the end of execution :\n";
    for (auto x: memory_changes){
        int mem_add = x.first;
        int val_mem_add = x.second;
        cout<<mem_add<<"-"<<mem_add+3<<" : "<<val_mem_add<<"\n";
    }
    cout<<"\nNumber of row buffer updates : "<<num_row_buffer_updates<<"\n";
    cout<<"\nNumber of row activations : "<<num_row_activation<<"\n";
    cout<<"\nNumber of row writebacks : "<<num_row_writeback<<"\n";
}
int Dram::getCycle(){
	return cycle;
}
void  Dram::incCycle(){
	cycle++;
}
void  Dram::decCycle(){
	cycle--;
}
void Dram::setCycle(int c){
	cycle = c;
}

void Dram::copy_row_buffer_to_row(int row_val, int start_time){
    for (int i=(256*row_val); i<(256*(row_val+1)); i++){
        memory_vec[i] = row_buffer[i-(256*row_val)];
    	
    }
    num_row_writeback++;
    cout<<"Cycle "<<start_time<<"-"<<start_time+row_access_delay-1<<" : "<<"DRAM : Writeback row "<<row_val<<"\n";
}
void Dram::copy_row_to_row_buffer(int row_val, int start_time){
	for (int i=(256*row_val); i<(256*(row_val+1)); i++){
    	row_buffer[i-(256*row_val)] = memory_vec[i];
    }
    row_buffer_ind = row_val;
    num_row_buffer_updates++;
    num_row_activation++;
    cout<<"Cycle "<<start_time<<"-"<<start_time+row_access_delay-1<<" : "<<"DRAM : Activate row "<<row_val<<"\n";
}
void Dram::copy_register_val_to_buffer(int r_index, int col_val, int start_time){
	
    row_buffer[col_val] = register_vec[r_index];
    num_row_buffer_updates++;
	cout<<"Cycle "<<start_time<<"-"<<start_time+col_access_delay-1<<" : "<<"DRAM : Column access, column "<<col_val*4<<"-"<<(col_val+1)*4-1<<" of row buffer updated to "<<register_vec[r_index]<<"\n";
	int memoryAddress = (row_buffer_ind*1024+(4*col_val));
	memory_changes[memoryAddress] = register_vec[r_index];
}
void Dram::copy_buffer_val_to_register(int r_index, int col_val, int start_time){
	register_vec[r_index] = row_buffer[col_val];

	cout<<"Cycle "<<start_time<<"-"<<start_time+col_access_delay-1<<" : "<<"DRAM : Column access, register "<<reg_ind_to_str[r_index]<<" updated to "<<register_vec[r_index]<<"\n";
}
