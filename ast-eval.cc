#include "ast.hh"
#include "local-environment.hh"
#include "symbol-table.hh"

void Ast::print_value(Local_Environment & eval_env, ostream & file_buffer) {

}

Eval_Result & Ast::get_value_of_evaluation(Local_Environment & eval_env) {

}

void Ast::set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result) {

}

Eval_Result& Assignment_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer) {
    string var_name = lhs->get_symbol_entry().get_variable_name();
    this->print(file_buffer);
    file_buffer << "\n" << VAR_SPACE << var_name << " : ";

    Eval_Result* rhs_eval;
    rhs_eval = &rhs->evaluate(eval_env, file_buffer);
    lhs->set_value_of_evaluation(eval_env, *rhs_eval);

    if(eval_env.does_variable_exist(var_name))
    {
        if(rhs_eval->get_result_enum() == int_result) {
            file_buffer << eval_env.get_variable_value(var_name)->get_int_value();
        }
        else if (rhs_eval->get_result_enum() == double_result) {
            file_buffer << eval_env.get_variable_value(var_name)->get_double_value();
        }
        file_buffer << "\n\n";
        return *rhs_eval;
    }
    else if(interpreter_global_table.does_variable_exist(var_name))
    {
        if(rhs_eval->get_result_enum() == int_result) {
            file_buffer << interpreter_global_table.get_variable_value(var_name)->get_int_value();
        }
        else if (rhs_eval->get_result_enum() == double_result) {
            file_buffer << interpreter_global_table.get_variable_value(var_name)->get_double_value();
        }
        file_buffer << "\n\n";
        return *rhs_eval;
    }
    else 
    {
        cout << "cs316: Error: Variable should be declared before its use" << endl;
        exit(1);
    }
    
}


Eval_Result& Name_Ast::get_value_of_evaluation(Local_Environment & eval_env) {
    string var_name = this->get_symbol_entry().get_variable_name();
    if(eval_env.does_variable_exist(var_name))
    {
        if(eval_env.is_variable_defined(var_name)) {
            Eval_Result& name_eval = *eval_env.get_variable_value(var_name);
            return name_eval;
        }
        else {
            cout << "cs316: Error: Variable should be defined before its use" << endl;
            exit(1);
        }
    }
    else if (interpreter_global_table.does_variable_exist(var_name))
    {
        if(interpreter_global_table.is_variable_defined(var_name)) {
            return *interpreter_global_table.get_variable_value(var_name);
        }
        else {
            cout << "cs316: Error: Variable should be defined before its use" << endl;
            exit(1);
        }        
    }
    else {
        cout << "cs316: Error: Variable should be declared before its use" << endl;
        exit(1);
    }
}

void Name_Ast::set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result) {
    string var_name = this->get_symbol_entry().get_variable_name();
    if(eval_env.does_variable_exist(var_name))
    {
        eval_env.put_variable_value(result, var_name);
        Eval_Result* temp = eval_env.get_variable_value(var_name);
        temp->set_variable_status(true);
        temp->set_result_enum(result.get_result_enum());    
    }
    else if(interpreter_global_table.does_variable_exist(var_name))
    {
        interpreter_global_table.put_variable_value(result, var_name);
        Eval_Result* temp = interpreter_global_table.get_variable_value(var_name);
        temp->set_variable_status(true);
        temp->set_result_enum(result.get_result_enum()); 
    }
    else {
        cout << "cs316: Error: Variable should be declared before its use" << endl;
        exit(1);
    }
}

Eval_Result & Name_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer) {
    return this->get_value_of_evaluation(eval_env);
}


void Name_Ast::print_value(Local_Environment & eval_env, ostream & file_buffer) {
    
}

template <>
Eval_Result & Number_Ast<int>::evaluate(Local_Environment & eval_env, ostream & file_buffer){
    Eval_Result *num_eval = new Eval_Result_Value_Int();
    num_eval->set_value(this->constant);
    num_eval->set_result_enum(int_result);
    return *num_eval;
}

template <>
Eval_Result & Number_Ast<double>::evaluate(Local_Environment & eval_env, ostream & file_buffer){
    Eval_Result *num_eval = new Eval_Result_Value_Double();
    num_eval->set_value(this->constant);
    num_eval->set_result_enum(double_result);
    return *num_eval;
}

Eval_Result& Plus_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer) {
    Eval_Result& lhs_eval = lhs->evaluate(eval_env, file_buffer);
    Eval_Result& rhs_eval = rhs->evaluate(eval_env, file_buffer);
    if(lhs_eval.get_result_enum() == int_result) {
        Eval_Result_Value_Int* plus_eval = new Eval_Result_Value_Int();
        plus_eval->set_value(lhs_eval.get_int_value() + rhs_eval.get_int_value());
        plus_eval->set_result_enum(int_result);
        return *plus_eval;
    }
    else if(lhs_eval.get_result_enum() == double_result) {
        Eval_Result_Value_Double* plus_eval = new Eval_Result_Value_Double();
        plus_eval->set_value(lhs_eval.get_double_value() + rhs_eval.get_double_value());
        plus_eval->set_result_enum(double_result);
        return *plus_eval;
    }
}

Eval_Result& Minus_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer) {
    Eval_Result& lhs_eval = lhs->evaluate(eval_env, file_buffer);
    Eval_Result& rhs_eval = rhs->evaluate(eval_env, file_buffer);
    if(lhs_eval.get_result_enum() == int_result) {
        Eval_Result_Value_Int* minus_eval = new Eval_Result_Value_Int();
        minus_eval->set_value(lhs_eval.get_int_value() - rhs_eval.get_int_value());
        minus_eval->set_result_enum(int_result);
        return *minus_eval;
    }
    else if(lhs_eval.get_result_enum() == double_result) {
        Eval_Result_Value_Double* minus_eval = new Eval_Result_Value_Double();
        minus_eval->set_value(lhs_eval.get_double_value() - rhs_eval.get_double_value());
        minus_eval->set_result_enum(double_result);
        return *minus_eval;
    }
}

Eval_Result& Divide_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer) {
    Eval_Result& lhs_eval = lhs->evaluate(eval_env, file_buffer);
    Eval_Result& rhs_eval = rhs->evaluate(eval_env, file_buffer);
    if(lhs_eval.get_result_enum() == int_result) {
        Eval_Result_Value_Int* divide_eval = new Eval_Result_Value_Int();
        if(rhs_eval.get_int_value() != 0) {
            divide_eval->set_value(lhs_eval.get_int_value() / rhs_eval.get_int_value());
        }
        else {
            cout << "\ncs316 Error: Divide by 0\n" << endl;
            exit(1);
        }
        divide_eval->set_result_enum(int_result);
        return *divide_eval;
    }
    else if(lhs_eval.get_result_enum() == double_result) {
        Eval_Result_Value_Double* divide_eval = new Eval_Result_Value_Double();
        if(rhs_eval.get_double_value() != 0.0) {
            divide_eval->set_value(lhs_eval.get_double_value() / rhs_eval.get_double_value());
        }
        else {
            cout << "\ncs316: Error: Divide by 0\n" << endl;
            exit(1);
        }
        divide_eval->set_result_enum(double_result);
        return *divide_eval;
    }
}

Eval_Result& Mult_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer) {
    Eval_Result& lhs_eval = lhs->evaluate(eval_env, file_buffer);
    Eval_Result& rhs_eval = rhs->evaluate(eval_env, file_buffer);
    if(lhs_eval.get_result_enum() == int_result) {
        Eval_Result_Value_Int* mult_eval = new Eval_Result_Value_Int();
        mult_eval->set_value(lhs_eval.get_int_value() * rhs_eval.get_int_value());
        mult_eval->set_result_enum(int_result);
        return *mult_eval;
    }
    else if(lhs_eval.get_result_enum() == double_result) {
        Eval_Result_Value_Double* mult_eval = new Eval_Result_Value_Double();
        mult_eval->set_value(lhs_eval.get_double_value() * rhs_eval.get_double_value());
        mult_eval->set_result_enum(double_result);
        return *mult_eval;
    }
}

Eval_Result& UMinus_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer) {
    Eval_Result& lhs_eval = lhs->evaluate(eval_env, file_buffer);
    if(lhs_eval.get_result_enum() == int_result) {
        Eval_Result_Value_Int* uminus_eval = new Eval_Result_Value_Int();
        uminus_eval->set_value(-1*lhs_eval.get_int_value());
        uminus_eval->set_result_enum(int_result);
        return *uminus_eval;
    }
    else if(lhs_eval.get_result_enum() == double_result) {
        Eval_Result_Value_Double* uminus_eval = new Eval_Result_Value_Double();
        uminus_eval->set_value(-1.0*lhs_eval.get_double_value());
        uminus_eval->set_result_enum(double_result);
        return *uminus_eval;
    }
}

Eval_Result& Conditional_Expression_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer) {
    Eval_Result& cond_eval = cond->evaluate(eval_env, file_buffer);
    if(cond_eval.get_int_value() == 1) {
        Eval_Result& lhs_eval = lhs->evaluate(eval_env, file_buffer);
        return lhs_eval;
    }
    else if (cond_eval.get_int_value() == 0) {
        Eval_Result& rhs_eval = rhs->evaluate(eval_env, file_buffer);
        return rhs_eval;
    }
}

Eval_Result& Relational_Expr_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer) {
    Eval_Result& lhs_eval = lhs_condition->evaluate(eval_env, file_buffer);
    Eval_Result& rhs_eval = rhs_condition->evaluate(eval_env, file_buffer);
    Eval_Result_Value_Int* rel_eval = new Eval_Result_Value_Int();
    double lhs_val, rhs_val;
    if(lhs_eval.get_result_enum() == int_result) {
        lhs_val = lhs_eval.get_int_value();
        rhs_val = rhs_eval.get_int_value();
    }
    else if(lhs_condition->get_data_type() == double_data_type) {
        lhs_val = lhs_eval.get_double_value();
        rhs_val = rhs_eval.get_double_value();
    }
    int res;
    switch (rel_op)
    {
        case less_equalto:
            res = (lhs_val <= rhs_val)?1:0;
            break;
        case less_than:
            res = (lhs_val < rhs_val)?1:0;
            break;
        case greater_than:
            res = (lhs_val > rhs_val)?1:0;
            break;
        case greater_equalto:
            res = (lhs_val >= rhs_val)?1:0;
            break;
        case equalto:
            res = (lhs_val == rhs_val)?1:0;
            break;
        case not_equalto:
            res = (lhs_val != rhs_val)?1:0;
            break;
        default:
            break;
    }
    rel_eval->set_value(res);
    rel_eval->set_result_enum(int_result);
    return *rel_eval;
}

Eval_Result& Logical_Expr_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer) {
    Eval_Result& rhs_eval = rhs_op->evaluate(eval_env, file_buffer);
    Eval_Result_Value_Int* logical_eval = new Eval_Result_Value_Int();
    logical_eval->set_result_enum(int_result);
    if(bool_op == _logical_not) {
        if(rhs_eval.get_int_value() == 1) {
            logical_eval->set_value(0);
        }
        else {
            logical_eval->set_value(1);
        }
    }
    else {
        Eval_Result& lhs_eval = lhs_op->evaluate(eval_env, file_buffer);
        if(bool_op == _logical_and) {
            if(lhs_eval.get_int_value() && rhs_eval.get_int_value()) {
                logical_eval->set_value(1);
            }
            else {
                logical_eval->set_value(0);
            }
        }
        else if(bool_op == _logical_or) {
            if(lhs_eval.get_int_value() || rhs_eval.get_int_value()) {
                logical_eval->set_value(1);
            }
            else {
                logical_eval->set_value(0);
            }
        }
    }
    return *logical_eval;
}

Eval_Result& Selection_Statement_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer) {
    Eval_Result& cond_eval = cond->evaluate(eval_env, file_buffer);
    if(cond_eval.get_int_value() == 1) {
        Eval_Result& then_eval = then_part->evaluate(eval_env, file_buffer);
        return then_eval;
    }
    else {
        if(else_part != NULL) {
            Eval_Result& else_eval = else_part->evaluate(eval_env, file_buffer);
            return else_eval;
        }
    }
}

Eval_Result& Iteration_Statement_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer) 
{
    Eval_Result* iteration_eval;
    if(is_do_form)
    {
        do 
        {
            iteration_eval = &(body->evaluate(eval_env, file_buffer));
        }
        while ( cond->evaluate(eval_env, file_buffer).get_int_value() == 1 );
    }
    else
    {
        while ( cond->evaluate(eval_env, file_buffer).get_int_value() == 1 )
        {
            iteration_eval = &(body->evaluate(eval_env, file_buffer));
        }
    }
    return (*iteration_eval);
}

Eval_Result & Sequence_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
    list<Ast *>::iterator itr;
    Eval_Result* sequence_eval;
    for (itr = statement_list.begin() ; itr != statement_list.end() ; itr++)
    {
        sequence_eval = &((*itr)->evaluate(eval_env,file_buffer));
    }
    return (*sequence_eval);
}

Eval_Result & Return_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer) {

}

Eval_Result & Call_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
    
}
