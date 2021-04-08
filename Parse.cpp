#include "Parse.h"

bool check_all_digits(string num_str){
    for (int i=0; i<num_str.size(); i++){
        if (num_str[i]<'0' || num_str[i]>'9'){
            return false;
        }
    }
    return true;
}
bool is_correct_instruction_address(int ins_addr){
    return true;
}
bool is_correct_memory_address(int mem_addr){
    return true;
}

bool is_valid_immediate_val(string num_str){
    if (num_str[0]!='+' && num_str[0]!='-'){
        return check_all_digits(num_str);
    }
    if (num_str.size()==1){
        return false;
    }
    return (check_all_digits(num_str.substr(1, num_str.size()-1)));

}

pair<int, int> address_as_register_offset(string reg_off){
    int offset_val;
    int l_paren_ind=-1;
    for (int i=0; i<reg_off.size(); i++){
        if (reg_off[i]=='('){
            l_paren_ind = i;
            break;
        }
    }
    if (l_paren_ind==-1)
        return {-1, -1};
    bool correct_offset=false;
    string offset_str = reg_off.substr(0, l_paren_ind);
    if (l_paren_ind==0 || is_valid_immediate_val(offset_str)){
        correct_offset = true;
        if (l_paren_ind==0)
            offset_val = 0;
        else
            offset_val = stoi(offset_str);
    }

    if (correct_offset==false || reg_off[reg_off.size()-1]!=')' || l_paren_ind == (reg_off.size()-2))
        return {-1, -1};
    
    string reg_val = reg_off.substr(l_paren_ind+1, reg_off.size()-l_paren_ind-2);
    if (reg_str_to_ind.find(reg_val)==reg_str_to_ind.end())
        return {-1, -1};
    int reg_num = reg_str_to_ind[reg_val];
    return {reg_num, offset_val};

}

int check_ins_add(vector<string> &inst){
    if (inst.size()!=4){
        return 0;
    }
    vector<int> tmp_ins(4);
    tmp_ins[0] = 0;
    if (reg_str_to_ind.find(inst[1])==reg_str_to_ind.end()){
        return 0;
    }
    tmp_ins[1] = reg_str_to_ind[inst[1]];
    if (reg_str_to_ind.find(inst[2])==reg_str_to_ind.end()){
        return 0;
    }
    tmp_ins[2] = reg_str_to_ind[inst[2]];
    if (reg_str_to_ind.find(inst[3])==reg_str_to_ind.end()){
        return 0;
    }
    tmp_ins[3] = reg_str_to_ind[inst[3]];
    instruction_processed.push_back(tmp_ins);
    return 1;

}
int check_ins_sub(vector<string> &inst){
    if (inst.size()!=4){
        return 0;
    }
    vector<int> tmp_ins(4);
    tmp_ins[0] = 1;
    if (reg_str_to_ind.find(inst[1])==reg_str_to_ind.end()){
        return 0;
    }
    tmp_ins[1] = reg_str_to_ind[inst[1]];
    if (reg_str_to_ind.find(inst[2])==reg_str_to_ind.end()){
        return 0;
    }
    tmp_ins[2] = reg_str_to_ind[inst[2]];
    if (reg_str_to_ind.find(inst[3])==reg_str_to_ind.end()){
        return 0;
    }
    tmp_ins[3] = reg_str_to_ind[inst[3]];
    instruction_processed.push_back(tmp_ins);
    return 1;
}
int check_ins_mul(vector<string> &inst){
    if (inst.size()!=4){
        return 0;
    }
    vector<int> tmp_ins(4);
    tmp_ins[0] = 2;
    if (reg_str_to_ind.find(inst[1])==reg_str_to_ind.end()){
        return 0;
    }
    tmp_ins[1] = reg_str_to_ind[inst[1]];
    if (reg_str_to_ind.find(inst[2])==reg_str_to_ind.end()){
        return 0;
    }
    tmp_ins[2] = reg_str_to_ind[inst[2]];
    if (reg_str_to_ind.find(inst[3])==reg_str_to_ind.end()){
        return 0;
    }
    tmp_ins[3] = reg_str_to_ind[inst[3]];
    instruction_processed.push_back(tmp_ins);
    return 1;
}
int check_ins_beq(vector<string> &inst){
    if (inst.size()!=4){
        return 0;
    }
    vector<int> tmp_ins(5);
    tmp_ins[0] = 3;
    if (reg_str_to_ind.find(inst[1])==reg_str_to_ind.end()){
        return 0;
    }
    tmp_ins[1] = reg_str_to_ind[inst[1]];
    if (reg_str_to_ind.find(inst[2])==reg_str_to_ind.end()){
        return 0;
    }
    tmp_ins[2] = reg_str_to_ind[inst[2]];
    if (check_all_digits(inst[3])==false){
        pair<int, int> addr_val = address_as_register_offset(inst[3]);
        if (addr_val.first!=-1){
            tmp_ins[3] = addr_val.first;
            tmp_ins[4] = addr_val.second;
            instruction_processed.push_back(tmp_ins);
            return 1;
        }
        if (id_address.find(inst[3])!=id_address.end()){
        	tmp_ins[3] = -1;
        	tmp_ins[4] = id_address[inst[3]]*4;
        	instruction_processed.push_back(tmp_ins);
        	return 1;
        }
        return 0;
    }
    int num_val = stoi(inst[3]);
    if (is_correct_instruction_address(num_val)==false)
        return 0;
    //tmp_ins[3] = num_val;
    tmp_ins[3] = -1;
    tmp_ins[4] = num_val;
    instruction_processed.push_back(tmp_ins);
    return 1;
}
int check_ins_bne(vector<string> &inst){
    if (inst.size()!=4){
        return 0;
    }
    vector<int> tmp_ins(5);
    tmp_ins[0] = 4;
    if (reg_str_to_ind.find(inst[1])==reg_str_to_ind.end()){
        return 0;
    }
    tmp_ins[1] = reg_str_to_ind[inst[1]];
    if (reg_str_to_ind.find(inst[2])==reg_str_to_ind.end()){
        return 0;
    }
    tmp_ins[2] = reg_str_to_ind[inst[2]];
    if (check_all_digits(inst[3])==false){
        pair<int, int> addr_val = address_as_register_offset(inst[3]);
        if (addr_val.first!=-1){
            tmp_ins[3] = addr_val.first;
            tmp_ins[4] = addr_val.second;
            instruction_processed.push_back(tmp_ins);
            return 1;
        }
        if (id_address.find(inst[3])!=id_address.end()){
        	tmp_ins[3] = -1;
        	tmp_ins[4] = id_address[inst[3]]*4;
        	instruction_processed.push_back(tmp_ins);
        	return 1;
        }
        return 0;
    }
    int num_val = stoi(inst[3]);
    if (is_correct_instruction_address(num_val)==false)
        return 0;
    // tmp_ins[3] = num_val;
    tmp_ins[3] = -1;
    tmp_ins[4] = num_val;
    instruction_processed.push_back(tmp_ins);
    return 1;
}
int check_ins_j(vector<string> &inst){
    if (inst.size()!=2){
        return 0;
    }
    vector<int> tmp_ins(3);
    tmp_ins[0] = 6;
    if (check_all_digits(inst[1])==false){
        pair<int, int> addr_val = address_as_register_offset(inst[1]);
        if (addr_val.first!=-1){
            tmp_ins[1] = addr_val.first;
            tmp_ins[2] = addr_val.second;
            instruction_processed.push_back(tmp_ins);
            return 1;
        }
        if (id_address.find(inst[1])!=id_address.end()){
        	tmp_ins[1] = -1;
        	tmp_ins[2] = id_address[inst[1]]*4;
        	instruction_processed.push_back(tmp_ins);
        	return 1;
        }
        return 0;
    }
    int num_val = stoi(inst[1]);
    if (is_correct_instruction_address(num_val)==false){
        return 0;
    }
    // tmp_ins[1] = num_val;
    tmp_ins[1] = -1;
    tmp_ins[2] = num_val;
    instruction_processed.push_back(tmp_ins);
    return 1;

}
int check_ins_addi(vector<string> &inst){
    if (inst.size()!=4){
        return 0;
    }
    vector<int> tmp_ins(4);
    tmp_ins[0] = 9;
    if (reg_str_to_ind.find(inst[1])==reg_str_to_ind.end()){
        return 0;
    }
    tmp_ins[1] = reg_str_to_ind[inst[1]];
    if (reg_str_to_ind.find(inst[2])==reg_str_to_ind.end()){
        return 0;
    }
    tmp_ins[2] = reg_str_to_ind[inst[2]];
    if (is_valid_immediate_val(inst[3])==false){
        return 0;
    }
    tmp_ins[3] = stoi(inst[3]);
    instruction_processed.push_back(tmp_ins);
    return 1;

}
int check_ins_slt(vector<string> &inst){
    if (inst.size()!=4){
        return 0;
    }
    vector<int> tmp_ins(4);
    tmp_ins[0] = 5;
    if (reg_str_to_ind.find(inst[1])==reg_str_to_ind.end()){
        return 0;
    }
    tmp_ins[1] = reg_str_to_ind[inst[1]];
    if (reg_str_to_ind.find(inst[2])==reg_str_to_ind.end()){
        return 0;
    }
    tmp_ins[2] = reg_str_to_ind[inst[2]];
    if (reg_str_to_ind.find(inst[3])==reg_str_to_ind.end()){
        return 0;
    }
    tmp_ins[3] = reg_str_to_ind[inst[3]];
    instruction_processed.push_back(tmp_ins);
    return 1;
}
int check_ins_lw(vector<string> &inst){
    if (inst.size()!=3){
        return 0;
    }
    vector<int> tmp_ins(4);
    tmp_ins[0] = 7;
    if (reg_str_to_ind.find(inst[1])==reg_str_to_ind.end()){
        return 0;
    }
    tmp_ins[1] = reg_str_to_ind[inst[1]];
    if (check_all_digits(inst[2])==false){
        pair<int, int> addr_val = address_as_register_offset(inst[2]);
        if (addr_val.first!=-1){
            tmp_ins[2] = addr_val.first;
            tmp_ins[3] = addr_val.second;
            instruction_processed.push_back(tmp_ins);
            return 1;
        }
        return 0;
    }
    int num_val = stoi(inst[2]);
    if (is_correct_memory_address(num_val)==false){
        return 0;
    }
    // tmp_ins[2] = num_val;
    tmp_ins[2] = -1;
    tmp_ins[3] = num_val;
    instruction_processed.push_back(tmp_ins);
    return 1;
}
int check_ins_sw(vector<string> &inst){
    if (inst.size()!=3){
        return 0;
    }
    vector<int> tmp_ins(4);
    tmp_ins[0] = 8;
    if (reg_str_to_ind.find(inst[1])==reg_str_to_ind.end()){
        return 0;
    }
    tmp_ins[1] = reg_str_to_ind[inst[1]];
    if (check_all_digits(inst[2])==false){
        pair<int, int> addr_val = address_as_register_offset(inst[2]);
        if (addr_val.first!=-1){
            tmp_ins[2] = addr_val.first;
            tmp_ins[3] = addr_val.second;
            instruction_processed.push_back(tmp_ins);
            return 1;
        }
        return 0;
    }
    int num_val = stoi(inst[2]);
    if (is_correct_memory_address(num_val)==false){
        return 0;
    }
    // tmp_ins[2] = num_val;
    tmp_ins[2] = -1;
    tmp_ins[3] = num_val;
    instruction_processed.push_back(tmp_ins);
    return 1;
}

int check_instructions(vector<vector<string>> &instruction_vector_tokens){
    for (int i=0; i<instruction_vector_tokens.size(); i++){
        string ins_type = instruction_vector_tokens[i][0];
        if (ins_type == "add"){
            if (check_ins_add(instruction_vector_tokens[i])==0)
                return 0;
        }
        else if (ins_type == "sub"){
            if (check_ins_sub(instruction_vector_tokens[i])==0)
                return 0;
        }
        else if(ins_type == "mul"){
            if (check_ins_mul(instruction_vector_tokens[i])==0)
                return 0;
        }
        else if(ins_type == "beq"){
            if (check_ins_beq(instruction_vector_tokens[i])==0)
                return 0;
        
        }
        else if(ins_type == "bne"){
            if (check_ins_bne(instruction_vector_tokens[i])==0)
                return 0;
        
        }
        else if(ins_type == "j"){
            if (check_ins_j(instruction_vector_tokens[i])==0)
                return 0;
        }
        
        else if(ins_type == "slt"){
            if (check_ins_slt(instruction_vector_tokens[i])==0)
                return 0;
        
        }
        else if(ins_type == "lw"){
            if (check_ins_lw(instruction_vector_tokens[i])==0)
                return 0;
        
        }
        else if(ins_type == "sw"){
            if (check_ins_sw(instruction_vector_tokens[i])==0)
                return 0;
        }
        else if(ins_type == "addi"){
            if (check_ins_addi(instruction_vector_tokens[i])==0)
                return 0;
        }

        else{
            return 0;
        }
    }
    return 1;
}
