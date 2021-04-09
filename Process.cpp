#include "Process.h"


ScheduleIns ins_scheduler;

int get_address(int register_ind, int Addr){
    if (register_ind==-1){
        return Addr;
    }
    else{
        return (Addr+ register_vec[register_ind]);
    }
}

void printState(){
    for (int i = 0; i < reg_ind_to_str.size(); i++)
    {
        cout<<left<<setw(10)<<dec<<reg_ind_to_str[i]<<right<<setw(20)<<register_vec[i]<<endl;
    }
}


void add(int ri_index, int rj_index, int rk_index){
    // cycle update
    vector<int> dep_reg_read = {rj_index, rk_index};
    vector<int> dep_reg_write = {ri_index};
    // cout<<dram.getCycle()<<"\n";
    ins_scheduler.cycleUpdate(dep_reg_read, dep_reg_write, {});
    // operation
    register_vec[ri_index] = register_vec[rj_index] + register_vec[rk_index];

    // print result
    cout<<"Cycle "<<dram.getCycle()<<" : "<<"add instruction, "<<reg_ind_to_str[ri_index]<<" = "<<register_vec[ri_index]<<"\n";
}

void addi(int ri_index,int rj_index, int val){
    vector<int> dep_reg_read = {rj_index};
    vector<int> dep_reg_write = {ri_index};
    ins_scheduler.cycleUpdate(dep_reg_read, dep_reg_write, {});

    register_vec[ri_index] = register_vec[rj_index] +val;
    // print result
    cout<<"Cycle "<<dram.getCycle()<<" : "<<"addi instruction, "<<reg_ind_to_str[ri_index]<<" = "<<register_vec[ri_index]<<"\n";
}

void sub(int ri_index, int rj_index, int rk_index){
    vector<int> dep_reg_read = {rj_index, rk_index};
    vector<int> dep_reg_write = {ri_index};
    ins_scheduler.cycleUpdate(dep_reg_read, dep_reg_write, {});

    register_vec[ri_index] = register_vec[rj_index] - register_vec[rk_index];
    // print result
    cout<<"Cycle "<<dram.getCycle()<<" : "<<"sub instruction, "<<reg_ind_to_str[ri_index]<<" = "<<register_vec[ri_index]<<"\n";
}

void mul(int ri_index, int rj_index, int rk_index){
    
    vector<int> dep_reg_read = {rj_index, rk_index};
    vector<int> dep_reg_write = {ri_index};
    ins_scheduler.cycleUpdate(dep_reg_read, dep_reg_write, {});

    register_vec[ri_index] = register_vec[rj_index] * register_vec[rk_index];
    // print result
    cout<<"Cycle "<<dram.getCycle()<<" : "<<"mul instruction, "<<reg_ind_to_str[ri_index]<<" = "<<register_vec[ri_index]<<"\n";
}

void beq(int ri_index, int rj_index, int register_ind, int Addr){

    vector<int> dep_reg_read  = {ri_index, rj_index, register_ind};
    vector<int> dep_reg_write(0);
    ins_scheduler.cycleUpdate(dep_reg_read, dep_reg_write, {});

    int branchLineAddr = get_address(register_ind, Addr);
    if(branchLineAddr%4==0 && (branchLineAddr/4)<=instruction_processed.size()){
    	
        if(register_vec[ri_index]==register_vec[rj_index]){
            currentLineIndex = branchLineAddr/4;
            if (currentLineIndex== instruction_processed.size()){
	    		cout<<"Cycle "<<dram.getCycle()<<" : "<<"bne instruction, jump to end of program\n";
	    		return;
    		}
            cout<<"Cycle "<<dram.getCycle()<<" : "<<"beq instruction, jump to instruction address "<<branchLineAddr<<"\n";
        }
        else{
            cout<<"Cycle "<<dram.getCycle()<<" : "<<"beq instruction, no jump\n";
        }
    }
    else{
        std::ostringstream oss;
        oss<< "Error: Invalid Instruction Address,"<<branchLineAddr<<" at instruction "<<currentLineIndex<<"\n";
        throw oss.str();     
    }

    //print result
}

void bne(int ri_index, int rj_index, int register_ind, int Addr){

    vector<int> dep_reg_read  = {ri_index, rj_index, register_ind};
    vector<int> dep_reg_write(0);

    ins_scheduler.cycleUpdate(dep_reg_read, dep_reg_write, {});

    int branchLineAddr = get_address(register_ind, Addr);

    if(branchLineAddr%4==0 && (branchLineAddr/4)<=instruction_processed.size()){
        if(register_vec[ri_index]!=register_vec[rj_index]){
            currentLineIndex = branchLineAddr/4;
            if (currentLineIndex== instruction_processed.size()){
	    		cout<<"Cycle "<<dram.getCycle()<<" : "<<"bne instruction, jump to end of program\n";
	    		return;
    		}
            cout<<"Cycle "<<dram.getCycle()<<" : "<<"bne instruction, jump to instruction address "<<branchLineAddr<<"\n";
        }
        else{
            cout<<"Cycle "<<dram.getCycle()<<" : "<<"bne instruction, no jump\n";
        }
    }
    else{
        std::ostringstream oss;
        oss<< "Error: Invalid Instruction Address,"<<branchLineAddr<<" at instruction "<<currentLineIndex<<"\n";
        throw oss.str();     
    }
    
}

void slt(int ri_index, int rj_index, int rk_index){

    vector<int> dep_reg_read = {rj_index, rk_index};
    vector<int> dep_reg_write = {ri_index};
    ins_scheduler.cycleUpdate(dep_reg_read, dep_reg_write, {});

    register_vec[ri_index] = register_vec[rj_index] < register_vec[rk_index];
    
    cout<<"Cycle "<<dram.getCycle()<<" : "<<"slt instruction. "<<reg_ind_to_str[ri_index]<<" = "<<register_vec[ri_index]<<"\n";
}

void j(int register_ind, int Addr){

    vector<int> dep_reg_read = {register_ind};
    vector<int> dep_reg_write(0);
    ins_scheduler.cycleUpdate(dep_reg_read, dep_reg_write, {});


    int branchLineAddr = get_address(register_ind, Addr);
    if(branchLineAddr%4==0 && (branchLineAddr/4)<=instruction_processed.size()){
        currentLineIndex = branchLineAddr/4;
    }
    else{
        std::ostringstream oss;
        oss<< "Error: Invalid Instruction Address,"<<branchLineAddr<<" at instruction "<<currentLineIndex<<"\n";
        throw oss.str();     
    }
    if (currentLineIndex == instruction_processed.size()){
    	cout<<"Cycle "<<dram.getCycle()<<" : "<<"jump to end of program\n";
    	return;
    }
    cout<<"Cycle "<<dram.getCycle()<<" : "<<"jump to instruction address "<<branchLineAddr<<"\n";
}

void sw(int r_index, int register_ind, int Addr){
    
    vector<int> dep_reg_read = {r_index, register_ind};
    vector<int> dep_reg_write(0);
    ins_scheduler.cycleUpdate(dep_reg_read, dep_reg_write, dep_reg_read);

	cout<<"Cycle "<<dram.getCycle()<<" : DRAM request issued for sw instruction"<<"\n";

    int memoryAddress = get_address(register_ind, Addr);

    if((memoryAddress%4==0)&&(memoryAddress/4>=instruction_processed.size()) && (memoryAddress<1048576)){
        ins_scheduler.pushDRAMRequest({1, r_index, memoryAddress, register_ind});
        
    }
    else{
        std::ostringstream oss;
        oss<< "Error: Invalid Memory Address,"<<memoryAddress<<" at sw instruction "<<currentLineIndex<<"\n";
        throw oss.str();
    }
}

void lw(int r_index, int register_ind, int Addr){

    vector<int> dep_reg_read = {register_ind};
    vector<int> dep_reg_write = {r_index};
    ins_scheduler.cycleUpdate(dep_reg_read, dep_reg_write, dep_reg_read);

    cout<<"Cycle "<<dram.getCycle()<<" : DRAM request issued for lw instruction"<<"\n";
    int memoryAddress = get_address(register_ind, Addr);

    if((memoryAddress%4==0)&&(memoryAddress/4>=instruction_processed.size()) && (memoryAddress<1048576)){
        ins_scheduler.pushDRAMRequest({0, r_index, memoryAddress, register_ind});
    }
    else{
        std::ostringstream oss;
        oss<< "Error: Invalid Memory Address,"<<memoryAddress<<" at lw instruction "<<currentLineIndex<<"\n";
        throw oss.str();
    }
}

void process_ins(){
    ins_scheduler = ScheduleIns();
    times_instruction_processed = vector<int>(instruction_processed.size(),0);
    while (currentLineIndex<instruction_processed.size())
    {

        vector<int> currentInstruction = instruction_processed[currentLineIndex];
        if ((currentInstruction[0]!=6)&&(currentInstruction[0]!=3) &&(currentInstruction[0]!=4) &&(currentInstruction[0]!=8) &&(currentInstruction[1]==0))
        {
            std::ostringstream oss;
            oss<< "Error: Invalid Operation, at instruction "<<currentLineIndex+1<<"\n";
            throw oss.str();
        }
    
        times_instruction_processed[currentLineIndex++]++;

        switch (currentInstruction[0])
        {
            case 0: add(currentInstruction[1], currentInstruction[2], currentInstruction[3]);break;
            case 1: sub(currentInstruction[1], currentInstruction[2], currentInstruction[3]);break; 
            case 2: mul(currentInstruction[1], currentInstruction[2], currentInstruction[3]);break; 
            case 3: beq(currentInstruction[1], currentInstruction[2], currentInstruction[3], currentInstruction[4]);break; 
            case 4: bne(currentInstruction[1], currentInstruction[2], currentInstruction[3], currentInstruction[4]);break; 
            case 5: slt(currentInstruction[1], currentInstruction[2], currentInstruction[3]);break; 
            case 6: j(currentInstruction[1], currentInstruction[2]);break; 
            case 7: lw(currentInstruction[1], currentInstruction[2], currentInstruction[3]);break; 
            case 8: sw(currentInstruction[1], currentInstruction[2], currentInstruction[3]);break; 
            case 9: addi(currentInstruction[1], currentInstruction[2],currentInstruction[3]);break; 
            default:
                break;
        }

        // printState();
    }
    ins_scheduler.processRemReq();
    cout<<"\nFinal register states :\n";
    printState();
}