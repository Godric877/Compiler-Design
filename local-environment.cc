#include "local-environment.hh"
#include "symbol-table.hh"


int Eval_Result::get_int_value() {

}

void Eval_Result::set_value(int value) {

}

double Eval_Result::get_double_value() {

}

void Eval_Result::set_value(double value) {

}


bool Eval_Result::is_variable_defined() {

}

void Eval_Result::set_variable_status(bool def) {

}

void Eval_Result::set_result_enum(Result_Enum) {

}

Result_Enum Eval_Result::get_result_enum() {

}

void Eval_Result_Value::set_value(int number) {

}

void Eval_Result_Value::set_value(double number) {

}

int Eval_Result_Value::get_int_value() {

}

double Eval_Result_Value::get_double_value() {

}

bool Eval_Result_Value::is_variable_defined() {

}

void Eval_Result_Value::set_variable_status(bool def) {

}

void Eval_Result_Value::set_result_enum(Result_Enum res) {

}

Result_Enum Eval_Result_Value::get_result_enum() {

}

Eval_Result_Value_Int::Eval_Result_Value_Int() {
    defined = false;
}

Eval_Result_Value_Int::~Eval_Result_Value_Int() {

}

void Eval_Result_Value_Int::set_value(int number) {
    value = number;
}

void Eval_Result_Value_Int::set_value(double number) {

}

int Eval_Result_Value_Int::get_int_value() {
    return value;
}

void Eval_Result_Value_Int::set_variable_status(bool def) {
    defined = def;
}

bool Eval_Result_Value_Int::is_variable_defined() {
    return defined;
}

void Eval_Result_Value_Int::set_result_enum(Result_Enum res) {
    result_type = res;
}

Result_Enum Eval_Result_Value_Int::get_result_enum() {
    return result_type;
}

Eval_Result_Value_Double::Eval_Result_Value_Double() {
    defined = false;
}

Eval_Result_Value_Double::~Eval_Result_Value_Double() {

}

void Eval_Result_Value_Double::set_value(double number) {
    value = number;
}

void Eval_Result_Value_Double::set_value(int number) {
    
}

double Eval_Result_Value_Double::get_double_value() {
    return value;
}

void Eval_Result_Value_Double::set_variable_status(bool def) {
    defined = def;
}

bool Eval_Result_Value_Double::is_variable_defined() {
    return defined;
}

void Eval_Result_Value_Double::set_result_enum(Result_Enum res) {
    result_type = res;
}

Result_Enum Eval_Result_Value_Double::get_result_enum() {
    return result_type;
}

Local_Environment::Local_Environment() {

}

Local_Environment::~Local_Environment() {

}

void Local_Environment::print(ostream & file_buffer) {
    map<string, Eval_Result *>::iterator itr;
    for(itr = variable_table.begin(); itr != variable_table.end(); itr++) {
        file_buffer << "         " + itr->first + " : ";
        if((itr->second)->get_result_enum() == int_result) {
            if(itr->second->is_variable_defined()) {
                file_buffer << itr->second->get_int_value();    
            }
            else {
                file_buffer << "undefined";
            }
            
        }
        else if ((itr->second)->get_result_enum() ==  double_result){
            if(itr->second->is_variable_defined()) {
                file_buffer << itr->second->get_double_value();
            }
            else {
                file_buffer << "undefined";
            }
            
            
        }
        file_buffer << endl;
    }
}

bool Local_Environment::is_variable_defined(string name) {
    return variable_table[name]->is_variable_defined();
}

Eval_Result* Local_Environment::get_variable_value(string name) {
    return variable_table[name];
}

void Local_Environment::put_variable_value(Eval_Result & value, string name) {
    variable_table[name] = &value;
}

bool Local_Environment::does_variable_exist(string name) {
    return variable_table.find(name) != variable_table.end();
}

void Symbol_Table::create(Local_Environment & local_global_variables_table)
{
    list<Symbol_Table_Entry*>::iterator itr;
    for (itr = variable_table.begin(); itr != variable_table.end(); itr++)
    {
        string var_name;
        var_name = (*itr)->get_variable_name();
        if( (*itr)->get_data_type() == int_data_type ) 
        {
            Eval_Result_Value_Int* init_eval = new Eval_Result_Value_Int();
            if(this->scope == global)
            {
                init_eval->set_value(0);
                init_eval->set_variable_status(true);
            }
            else if(this->scope == local)
            {
                init_eval->set_variable_status(false);
            }
            init_eval->set_result_enum(int_result);
            local_global_variables_table.put_variable_value(*init_eval,var_name);
        }
        else if( (*itr)->get_data_type() == double_data_type )
        {
            Eval_Result_Value_Double* init_eval = new Eval_Result_Value_Double();
            if(this->scope == global)
            {
                init_eval->set_value(0.0);
                init_eval->set_variable_status(true);
            }
            else if(this->scope == local)
            {
                init_eval->set_variable_status(false);
            }
            init_eval->set_result_enum(double_result);
            local_global_variables_table.put_variable_value(*init_eval,var_name);
        }
    }
}
