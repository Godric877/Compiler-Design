#include "ast.hh"
#include "symbol-table.hh"

template class Number_Ast<double>;
template class Number_Ast<int>;

int Ast::labelCounter;

Ast::Ast() {

}

Ast::~Ast() {

}

Data_Type Ast::get_data_type() {
    return this->node_data_type;
}

void Ast::set_data_type(Data_Type dt) {
    this->node_data_type = dt;
}

bool Ast::is_value_zero() {
    
}

bool Ast::check_ast() {
    
}

Symbol_Table_Entry & Ast::get_symbol_entry() {

}



Assignment_Ast::Assignment_Ast(Ast * temp_lhs, Ast * temp_rhs, int line) {
    this->lhs = temp_lhs;
    this->rhs = temp_rhs;
    this->lineno = line;
    this->ast_num_child = binary_arity;
    this->node_data_type = temp_lhs->get_data_type();
}

// Assignment_Ast::~Assignment_Ast() {
//     delete this->lhs;
//     delete this->rhs;
// }

bool Assignment_Ast::check_ast() {

    if (this->lhs->get_data_type() != this->rhs->get_data_type()) {
        // cout << "LHS Data Type" << this->lhs->get_data_type() << endl; 
        // cout << "RHS Data Type" << this->rhs->get_data_type() << endl;
        cout << "cs316: Error : Line: " << lineno << ": Assignment statement data type not compatible" << endl;
        exit(1);
    }
    return (this->lhs->get_data_type() == this->rhs->get_data_type());
}

void Assignment_Ast::print(ostream & file_buffer) {
    file_buffer << "\n         Asgn:\n";
    file_buffer << "            LHS (";
    this->lhs->print(file_buffer);
    file_buffer << ")\n";
    file_buffer << "            RHS (";
    this->rhs->print(file_buffer);
    file_buffer << ")";
}

Name_Ast::Name_Ast(string & name, Symbol_Table_Entry & var_entry, int line) {
    this->variable_symbol_entry = &var_entry;
    this->lineno = line;
    this->ast_num_child = zero_arity;
    this->node_data_type = var_entry.get_data_type();
}

Name_Ast::~Name_Ast() {
}

Data_Type Name_Ast::get_data_type() {
    return this->node_data_type;
}

Symbol_Table_Entry & Name_Ast::get_symbol_entry() {
    return *(this->variable_symbol_entry);
}

void Name_Ast::set_data_type(Data_Type dt) {
    this->variable_symbol_entry->set_data_type(dt);
    this->node_data_type = dt;
}

void Name_Ast::print(ostream & file_buffer) {
    file_buffer << "Name : " << this->variable_symbol_entry->get_variable_name();
}

template<class T>
Number_Ast<T>::Number_Ast(T number, Data_Type constant_data_type, int line) {
    this->constant = number;
    this->lineno = line;
    this->node_data_type = constant_data_type;
    this->ast_num_child = zero_arity;
}

template<class T>
Number_Ast<T>::~Number_Ast() {

}

template<class T>
Data_Type Number_Ast<T>::get_data_type() {
    return this->node_data_type;
}

template<class T>
void Number_Ast<T>::set_data_type(Data_Type dt) {
    this->node_data_type = dt;
}

template<class T>
bool Number_Ast<T>::is_value_zero() {
    
}

template<class T>
void Number_Ast<T>::print(ostream & file_buffer) {
    file_buffer << "Num : " << this->constant;
}


Data_Type Arithmetic_Expr_Ast::get_data_type() {
    return this->node_data_type;
}

void Arithmetic_Expr_Ast::set_data_type(Data_Type dt) {
    this->node_data_type = dt;
}

bool Arithmetic_Expr_Ast::check_ast() {
    if (this->lhs->get_data_type() != this->rhs->get_data_type()) {
        cout << "cs316: Error : Line: " << lineno << ": Arithmetic statement data type not compatible" << endl;
        exit(1);
    }

    return this->lhs->get_data_type() == this->rhs->get_data_type();
}



Plus_Ast::Plus_Ast(Ast * l, Ast * r, int line) {
    this->lhs = l;
    this->rhs = r;
    this->lineno = line;
    this->ast_num_child = binary_arity;
    this->node_data_type = l->get_data_type();
}


void Plus_Ast::print(ostream & file_buffer) {
    file_buffer << "\n            Arith: PLUS\n";
    file_buffer << "               LHS (";
    lhs->print(file_buffer);
    file_buffer << ")";
    file_buffer << "\n               RHS (";
    rhs->print(file_buffer);
    file_buffer << ")";
}

Minus_Ast::Minus_Ast(Ast * l, Ast * r, int line) {
    this->lhs = l;
    this->rhs = r;
    this->lineno = line;
    this->ast_num_child = binary_arity;
    this->node_data_type = l->get_data_type();
}


void Minus_Ast::print(ostream & file_buffer) {
    file_buffer << "\n            Arith: MINUS\n";
    file_buffer << "               LHS (";
    lhs->print(file_buffer);
    file_buffer << ")";
    file_buffer << "\n               RHS (";
    rhs->print(file_buffer);
    file_buffer << ")";
}

Divide_Ast::Divide_Ast(Ast * l, Ast * r, int line) {
    this->lhs = l;
    this->rhs = r;
    this->lineno = line;
    this->ast_num_child = binary_arity;
    this->node_data_type = l->get_data_type();
}


void Divide_Ast::print(ostream & file_buffer) {
    file_buffer << "\n            Arith: DIV\n";
    file_buffer << "               LHS (";
    lhs->print(file_buffer);
    file_buffer << ")";
    file_buffer << "\n               RHS (";
    rhs->print(file_buffer);
    file_buffer << ")";
}

Mult_Ast::Mult_Ast(Ast * l, Ast * r, int line) {
    this->lhs = l;
    this->rhs = r;
    this->lineno = line;
    this->ast_num_child = binary_arity;
    this->node_data_type = l->get_data_type();
}

void Mult_Ast::print(ostream & file_buffer) {
    file_buffer << "\n            Arith: MULT\n";
    file_buffer << "               LHS (";
    lhs->print(file_buffer);
    file_buffer << ")";
    file_buffer << "\n               RHS (";
    rhs->print(file_buffer);
    file_buffer << ")";
}

UMinus_Ast::UMinus_Ast(Ast * l, Ast * r, int line) {
    this->lhs = l;
    this->rhs = r;
    this->lineno = line;
    this->ast_num_child = unary_arity;
    this->node_data_type = l->get_data_type();
}

void UMinus_Ast::print(ostream & file_buffer) {
    file_buffer << "\n            Arith: UMINUS\n";
    file_buffer << "               LHS (";
    lhs->print(file_buffer);
    file_buffer << ")";
}

Return_Ast::Return_Ast(Ast * ret_val, string name, int line) {
    this->return_value = ret_val;
    this->proc_name = name;
    this->lineno = line;
}

Return_Ast::~Return_Ast() {}

void Return_Ast::print(ostream & file_buffer) {
    file_buffer << "\n" << AST_NODE_SPACE << "RETURN \n";
    if(this->return_value != NULL)
        this->return_value->print(file_buffer);
}

Data_Type Return_Ast::get_data_type() { 
    return program_object.get_procedure_prototype(this->proc_name)->get_return_type();
}

Call_Ast::Call_Ast(string name, int line){
    this->procedure_name = name;
    this->lineno = line;
    this->return_value_reg = new Register_Descriptor(none, "none", int_num, fn_result);
    this->return_value_reg->reset_used_for_fn_return();
}

Call_Ast::~Call_Ast(){}

Data_Type Call_Ast::get_data_type(){
    return program_object.get_procedure_prototype(this->procedure_name)->get_return_type();
}

void Call_Ast::check_actual_formal_param(Symbol_Table & formal_param_list){

    if (formal_param_list.get_table().size() != this->actual_param_list.size()) {
        cout << "cs316: Error : Line: " << lineno << " : Actual and formal parameter count do not match\n";
        exit(0);
    }
    int idx = 1;
    list<Ast *>::iterator itr;
    for(itr = actual_param_list.begin(); itr != actual_param_list.end(); itr++) {
        string var_name = formal_param_list.get_variable_in_formal_list(idx);
        Symbol_Table_Entry & temp = formal_param_list.get_symbol_table_entry(var_name);
        if (temp.get_data_type() != (*itr)->get_data_type()) {
            cout << "cs316: Error : Line: " << lineno << " : Actual and formal parameter Data types do not match\n";
            exit(0);
        }
        idx++;
    }
}

void Call_Ast::print(ostream & file_buffer){
    file_buffer << "\n" << AST_NODE_SPACE << "FN CALL: " << this->procedure_name << "(";
    list<Ast *>::iterator itr;
    for(itr = actual_param_list.begin(); itr != actual_param_list.end(); itr++) {
        file_buffer << "\n" << AST_SUB_NODE_SPACE << "(";
        (*itr)->print(file_buffer);
        file_buffer << ")";
    }
    file_buffer << ")";
}

void Call_Ast::set_actual_param_list(list<Ast *> & param_list){
    this->actual_param_list = param_list;
}

void Call_Ast::set_register(Register_Descriptor * reg) {
    this->return_value_reg = reg;
}

Conditional_Expression_Ast::Conditional_Expression_Ast(Ast* cond, Ast* l, Ast* r, int line)
{
    this->lhs = l;
    this->rhs = r;
    this->lineno = line;
    this->ast_num_child = ternary_arity;
    this->node_data_type = l->get_data_type();
    this->cond = cond;
    labelCounter++;
}

Conditional_Expression_Ast::~Conditional_Expression_Ast()
{

}

void Conditional_Expression_Ast::print(ostream & file_buffer)
{
    file_buffer << "\n         Cond:\n";
    file_buffer << "            IF_ELSE";
    cond->print(file_buffer);
    file_buffer << "\n            LHS (";
    lhs->print(file_buffer);
    file_buffer << ")";
    file_buffer << "\n            RHS (";
    rhs->print(file_buffer);
    file_buffer << ")";
}

Relational_Expr_Ast::Relational_Expr_Ast(Ast * lhs, Relational_Op rop, Ast * rhs, int line)
{
    this->lhs_condition = lhs;
    this->rhs_condition = rhs;
    this->rel_op = rop;
    this->lineno = line;
    this->ast_num_child = binary_arity;
    this->node_data_type = lhs->get_data_type();
}

Relational_Expr_Ast::~Relational_Expr_Ast()
{

}

Data_Type Relational_Expr_Ast::get_data_type() {
    return this->node_data_type;
}

void Relational_Expr_Ast::set_data_type(Data_Type dt) {
    this->node_data_type = dt;
}

bool Relational_Expr_Ast::check_ast() {
    if (this->lhs_condition->get_data_type() != this->rhs_condition->get_data_type()) {
        cout << "cs316: Error : Line: " << lineno << ": Relational statement data type not compatible" << endl;
        exit(1);
    }

    return this->lhs_condition->get_data_type() == this->rhs_condition->get_data_type();
}

void Relational_Expr_Ast::print(ostream & file_buffer)
{
    string s = "\n            Condition: ";
    if (rel_op == less_equalto)
    {
        s += "LE";
    }
    else if(rel_op == less_than)
    {
        s += "LT";
    }
    else if(rel_op == greater_than)
    {
        s += "GT";
    }
    else if(rel_op == greater_equalto)
    {
        s += "GE";
    }
    else if(rel_op == equalto)
    {
        s += "EQ";
    }
    else if(rel_op == not_equalto)
    {
        s += "NE";
    }
    file_buffer << s;
    file_buffer << "\n               LHS (";
    lhs_condition->print(file_buffer);
    file_buffer << ")";
    file_buffer << "\n               RHS (";
    rhs_condition->print(file_buffer);
    file_buffer << ")";
}

Logical_Expr_Ast::Logical_Expr_Ast(Ast * lhs, Logical_Op bop, Ast * rhs, int line)
{
    this->lhs_op = lhs;
    this->rhs_op = rhs;
    this->bool_op = bop;
    this->node_data_type = rhs->get_data_type();
    if(bop == _logical_not)
    {
        this->ast_num_child = unary_arity;
    }
    else
    {
        this->ast_num_child = binary_arity;
    }
    this->lineno = line;
}

Logical_Expr_Ast::~Logical_Expr_Ast()
{

}

Data_Type Logical_Expr_Ast::get_data_type() {
    return this->node_data_type;
}

void Logical_Expr_Ast::set_data_type(Data_Type dt) {
    this->node_data_type = dt;
}

bool Logical_Expr_Ast::check_ast() {
    if (this->lhs_op->get_data_type() != this->rhs_op->get_data_type()) {
        cout << "cs316: Error : Line: " << lineno << ": Logical statement data type not compatible" << endl;
        exit(1);
    }

    return this->lhs_op->get_data_type() == this->rhs_op->get_data_type();
}

void Logical_Expr_Ast::print(ostream & file_buffer)
{
    string s = "\n            Condition: ";
    if (bool_op == _logical_not)
    {
        s += "NOT";
    }
    else if(bool_op == _logical_and)
    {
        s += "AND";
    }
    else if(bool_op == _logical_or)
    {
        s += "OR";
    }
    file_buffer << s;
    if(lhs_op != NULL)
    {
        file_buffer << "\n               LHS (";
        lhs_op->print(file_buffer);
        file_buffer << ")";
    }
    file_buffer << "\n               RHS (";
    rhs_op->print(file_buffer);
    file_buffer << ")";
}

Selection_Statement_Ast::Selection_Statement_Ast(Ast * cond,Ast* then_part, Ast* else_part, int line)
{
    this->cond = cond;
    this->then_part = then_part;
    this->else_part = else_part;
    this->lineno = line;
    this->node_data_type = int_data_type;
    this->ast_num_child = ternary_arity;
}

Selection_Statement_Ast::~Selection_Statement_Ast()
{

}

Data_Type Selection_Statement_Ast::get_data_type() {
    return this->node_data_type;
}

void Selection_Statement_Ast::set_data_type(Data_Type dt) {
    this->node_data_type = dt;
}

bool Selection_Statement_Ast::check_ast() {
    
}

void Selection_Statement_Ast::print(ostream & file_buffer)
{
    file_buffer << "\n         IF : ";
    file_buffer << "\n         CONDITION (";
    cond->print(file_buffer);
    file_buffer << ")";
    file_buffer << "\n         THEN (";
    then_part->print(file_buffer);
    file_buffer << ")";
    if(else_part != NULL) {
        file_buffer << "\n         ELSE (";
        else_part->print(file_buffer);
        file_buffer << ")";    
    }
    
}

Iteration_Statement_Ast::Iteration_Statement_Ast(Ast * cond, Ast* body, int line, bool do_form)
{
    this->cond = cond;
    this->body = body;
    this->is_do_form = do_form;
    this->lineno = line;
    this->node_data_type = int_data_type;
    this->ast_num_child = binary_arity;
}

Iteration_Statement_Ast::~Iteration_Statement_Ast()
{

}

Data_Type Iteration_Statement_Ast::get_data_type() {
    return this->node_data_type;
}

void Iteration_Statement_Ast::set_data_type(Data_Type dt) {
    this->node_data_type = dt;
}

bool Iteration_Statement_Ast::check_ast() {
    
}

void Iteration_Statement_Ast::print(ostream & file_buffer)
{
    if(is_do_form)
    {
        file_buffer << "\n         DO (";
        body->print(file_buffer);
        file_buffer << ")";
        file_buffer << "\n         WHILE CONDITION (";
        cond->print(file_buffer);
        file_buffer << ")";
    }
    else
    {
        file_buffer << "\n         WHILE : ";
        file_buffer << "\n         CONDITION (";
        cond->print(file_buffer);
        file_buffer << ")";
        file_buffer << "\n         BODY (";
        body->print(file_buffer);
        file_buffer << ")";       
    }
}

Sequence_Ast::Sequence_Ast(int line)
{
    this->lineno = line;
    this->ast_num_child = zero_arity;
    labelCounter++;
}

Sequence_Ast::~Sequence_Ast()
{

}

void Sequence_Ast::ast_push_back(Ast * ast)
{
    statement_list.push_back(ast);
}

void Sequence_Ast::print(ostream & file_buffer)
{
    list<Ast*>::iterator itr;
    for (itr = statement_list.begin() ; itr != statement_list.end() ; itr++)
    {
        file_buffer << '\n' << AST_NODE_SPACE;
        (*itr)->print(file_buffer);
    }
}

void Sequence_Ast::print_assembly(ostream & file_buffer)
{

}

void Sequence_Ast::print_icode(ostream & file_buffer)
{

}

Print_Ast::Print_Ast(Ast *v, int line)
{
    this->var = v;
    this->lineno = line;
}

 Print_Ast::~Print_Ast(){}

 void Print_Ast::print(ostream & file_buffer)
 {
    file_buffer << "\n" << AST_NODE_SPACE << "Print :\n";
    file_buffer <<  AST_SUB_NODE_SPACE << "(";
    this->var->print(file_buffer);
    file_buffer << ")";
 }

