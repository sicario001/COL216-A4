/*
Instructions are add, sub, mul, beq, bne, slt, j, lw, sw and addi
0 i j k     add $ri,$rj,$rk         : $ri = $rj + $rk
1 i j k     sub $ri,$rj,$rk         : $ri = $rj - $rk
2 i j k     mul $ri,$rj,$rk         : store the result of first 32 bits of multiplication of $rj * $rk in $ri
3 i j addr  beq $ri,$rj,addr        : branch to address addr if $ri == $rj
4 i j addr  bne $ri,$rj,addr        : branch to address addr if $ri != $rj
5 i j k     slt $ri,$rj,$rk         : $ri =1 if $rj < $rk
6 i         j addr                  : unconditional jump to address addr
7 i add_mem lw $ri,address_memory   : load $ri with value stored at address address_memory
8 i add_mem sw $ri,address_memory   : store value of $ri in the memory at address address_memory
9 i j val   addi $ri,$rj,val        : $ri = $rj + val

total of 2^20 bytes available
*/
#include "Parse.h"
#include "Process.h"
#include "Tokenize.h"
#include "ScheduleIns.h"

unordered_map<string, int> reg_str_to_ind;
vector<string> reg_ind_to_str;

// added for DRAM
Dram dram;
map<string, int> id_address;

// initialize registers
vector<int> register_vec (32, 0);
vector<vector<int>> instruction_processed;
vector<int> times_instruction_processed;
int currentLineIndex = 0;

void add_reg_str(int num, string pref,int start=0){
    stringstream ss;
    for(int i=start;i<num+start;i++){
        ss.str("");
        ss << "$" << pref << i;
        reg_ind_to_str.push_back(ss.str());
    }
}

void initialize_reg_map(){
    reg_ind_to_str.push_back("$zero");
    add_reg_str(2,"v");
    add_reg_str(4,"a");
    add_reg_str(8,"t");
    add_reg_str(8,"s");
    add_reg_str(2,"t",8);
    add_reg_str(2,"k");
    reg_ind_to_str.push_back("$gp");
    reg_ind_to_str.push_back("$fp");
    reg_ind_to_str.push_back("$sp");
    reg_ind_to_str.push_back("$ra");
    for(int i = 0;i <reg_ind_to_str.size();i++){
        reg_str_to_ind[reg_ind_to_str[i]]=i;
    }
}

int main(int argc, char** argv){
    if (argc>2){
        cout<<"Invalid command! Too many arguments.\n";
        return 0;
    }
    else if(argc<2){
        cout<<"Invalid command! Command is of the form ./mips <filename>\n";
        return 0;
    }
    else{
        string filename = argv[1];
        int row_access,col_access;
        cout<<"\nEnter the row_access_delay for DRAM : ";
        cin>>row_access;
        if (row_access<=0){
        	cout<<"row_access_delay must be a positive integer\n";
        	return 0;
        }
        cout<<"\nEnter the col_access_delay for DRAM : ";
        cin>>col_access;
        
        if (col_access<=0){
        	cout<<"col_access_delay must be a positive integer\n";
        	return 0;
        }
        cout<<"\nEvery cycle description :\n\n";
        ifstream fin (filename);
        vector<vector<string>> instruction_vector_tokens;
        if (fin.is_open()){
            try{
                instruction_vector_tokens = tokenize(fin);
            }
            catch(string exp){
                cout<<exp;
                return 0;
            }
            initialize_reg_map();

            int flag;
            try {
                flag = check_instructions(instruction_vector_tokens);
            }

            catch(out_of_range& e){
                cout<<"Integer out of range!\n";
                return 0;
            }

            if (flag==0){
                cout<<"Invalid Instructions!\n";
                return 0;
            }

            try{
                dram = Dram(row_access, col_access);
                process_ins();
            }
            catch(const string exp){
                cout<<exp<<"\n";
                return 0;
            }
            
            cout<<"\nRan in "<<dec<<dram.getFinalCycle()<<" clock cycles"<<endl;
            cout<<"Number of times each instruction was exectuted:\n";
            for (int i = 0; i < times_instruction_processed.size(); i++)
            {
                cout<<setw(2)<<i+1<<" - "<<setw(5)<<instruction_vector_tokens[i][0]<<" : ";
                cout<<setw(3)<<times_instruction_processed[i]<<"\n";
            }
            dram.print_mem_state();
            return 0;
        }
        else{
            cout<<"Invalid instruction file name\n";
            return 0;
        }
    }
}
