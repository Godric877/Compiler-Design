#include "ast.hh"
#include "reg-alloc.hh"
#include "icode.hh"
#include <map>

using namespace std;

Code_For_Ast & Ast::compile() {}
Code_For_Ast & Ast::compile_and_optimize_ast(Lra_Outcome & lra) {}
Code_For_Ast & Ast::create_store_stmt(Register_Descriptor * store_register) {}

Code_For_Ast & Assignment_Ast::compile() {
	Code_For_Ast & rhs_compile = rhs->compile();
	Register_Addr_Opd *rhs_reg = new Register_Addr_Opd(rhs_compile.get_reg());
	Mem_Addr_Opd *mem_addr = new Mem_Addr_Opd(lhs->get_symbol_entry());
	Code_For_Ast *assignment_compile = new Code_For_Ast;
	*assignment_compile = rhs_compile;
	if(rhs_compile.get_reg()->get_use_category() == int_reg)
	{
		Move_IC_Stmt *store_stmt = new Move_IC_Stmt(store, rhs_reg, mem_addr);
		assignment_compile->append_ics(*store_stmt);
	}
	else if(rhs_compile.get_reg()->get_use_category() == float_reg)
	{
		Move_IC_Stmt *store_stmt = new Move_IC_Stmt(store_d, rhs_reg, mem_addr);
		assignment_compile->append_ics(*store_stmt);
	}
	rhs_compile.get_reg()->reset_register_occupied();
	return *assignment_compile;
}

Code_For_Ast & Assignment_Ast::compile_and_optimize_ast(Lra_Outcome & lra) {}

Code_For_Ast & Name_Ast::compile() {
	Mem_Addr_Opd *mem_addr = new Mem_Addr_Opd(this->get_symbol_entry());
	Code_For_Ast *name_compile = new Code_For_Ast;
	if(this->get_data_type() == int_data_type) {
		Register_Addr_Opd *name_reg = new Register_Addr_Opd(machine_desc_object.get_new_register<int_reg>());
		Move_IC_Stmt *load_stmt = new Move_IC_Stmt(load, mem_addr, name_reg);
		name_compile->append_ics(*load_stmt);
		name_compile->set_reg(name_reg->get_reg());
	}
	else if (this->get_data_type() == double_data_type) {
		Register_Addr_Opd *name_reg = new Register_Addr_Opd(machine_desc_object.get_new_register<float_reg>());
		Move_IC_Stmt *load_stmt = new Move_IC_Stmt(load_d, mem_addr, name_reg);
		name_compile->append_ics(*load_stmt);
		name_compile->set_reg(name_reg->get_reg());
	}
	return *name_compile;
}

Code_For_Ast & Name_Ast::compile_and_optimize_ast(Lra_Outcome & lra) {}
Code_For_Ast & Name_Ast::create_store_stmt(Register_Descriptor * store_register) {}

template <>
Code_For_Ast & Number_Ast<int>::compile() {
	Code_For_Ast *num_compile = new Code_For_Ast;
	Const_Opd<int> *num_opd = new Const_Opd<int>(this->constant);
	Register_Descriptor* r = machine_desc_object.get_new_register<int_reg>();
	Register_Addr_Opd *num_reg = new Register_Addr_Opd(r);
	Move_IC_Stmt *iload = new Move_IC_Stmt(imm_load, num_opd, num_reg);
	num_compile->append_ics(*iload);
	num_compile->set_reg(num_reg->get_reg());
	return *num_compile;
}

template<>
Code_For_Ast & Number_Ast<int>::compile_and_optimize_ast(Lra_Outcome & lra) {}

template <>
Code_For_Ast & Number_Ast<double>::compile() {
	Const_Opd<double> *num_opd = new Const_Opd<double>(this->constant);
	Code_For_Ast *num_compile = new Code_For_Ast;
	Register_Addr_Opd *num_reg = new Register_Addr_Opd(machine_desc_object.get_new_register<float_reg>());
	Move_IC_Stmt *iload = new Move_IC_Stmt(imm_load_d, num_opd, num_reg);
	num_compile->append_ics(*iload);
	num_compile->set_reg(num_reg->get_reg());
	return *num_compile;
}

template<>
Code_For_Ast & Number_Ast<double>::compile_and_optimize_ast(Lra_Outcome & lra) {}

Code_For_Ast & Arithmetic_Expr_Ast::compile() {}
Code_For_Ast & Arithmetic_Expr_Ast::compile_and_optimize_ast(Lra_Outcome & lra) {}

Code_For_Ast & Plus_Ast::compile() {
	Code_For_Ast *plus_compile = new Code_For_Ast;
	Code_For_Ast & lhs_compile = lhs->compile();
	Code_For_Ast & rhs_compile = rhs->compile();

	list<Icode_Stmt *> lhs_list = lhs_compile.get_icode_list();
	list<Icode_Stmt *> rhs_list = rhs_compile.get_icode_list();
	list<Icode_Stmt *>::iterator itr;

	for(itr = lhs_list.begin(); itr != lhs_list.end(); itr++) {
		plus_compile->append_ics(**itr);
	}
	for(itr = rhs_list.begin(); itr != rhs_list.end(); itr++) {
		plus_compile->append_ics(**itr);
	}

	Register_Addr_Opd *lhs_reg = new Register_Addr_Opd(lhs_compile.get_reg());
	Register_Addr_Opd *rhs_reg = new Register_Addr_Opd(rhs_compile.get_reg()); 
	if(lhs->get_data_type() == int_data_type) {
		Register_Addr_Opd *plus_reg = new Register_Addr_Opd(machine_desc_object.get_new_register<int_reg>());
		Compute_IC_Stmt *plus = new Compute_IC_Stmt(add, lhs_reg, rhs_reg, plus_reg);
		plus_compile->append_ics(*plus);
		plus_compile->set_reg(plus_reg->get_reg());
	}
	else if (lhs->get_data_type() == double_data_type) {
		Register_Addr_Opd *plus_reg = new Register_Addr_Opd(machine_desc_object.get_new_register<float_reg>());
		Compute_IC_Stmt *plus = new Compute_IC_Stmt(add_d, lhs_reg, rhs_reg, plus_reg);
		plus_compile->append_ics(*plus);
		plus_compile->set_reg(plus_reg->get_reg());		
	}
	lhs_compile.get_reg()->reset_register_occupied();
	rhs_compile.get_reg()->reset_register_occupied();
	return *plus_compile;
}

Code_For_Ast & Plus_Ast::compile_and_optimize_ast(Lra_Outcome & lra) {}

Code_For_Ast & Minus_Ast::compile() {
	Code_For_Ast *minus_compile = new Code_For_Ast;
	Code_For_Ast & lhs_compile = lhs->compile();
	Code_For_Ast & rhs_compile = rhs->compile();

	list<Icode_Stmt *> lhs_list = lhs_compile.get_icode_list();
	list<Icode_Stmt *> rhs_list = rhs_compile.get_icode_list();
	list<Icode_Stmt *>::iterator itr;

	for(itr = lhs_list.begin(); itr != lhs_list.end(); itr++) {
		minus_compile->append_ics(**itr);
	}
	for(itr = rhs_list.begin(); itr != rhs_list.end(); itr++) {
		minus_compile->append_ics(**itr);
	}

	Register_Addr_Opd *lhs_reg = new Register_Addr_Opd(lhs_compile.get_reg());
	Register_Addr_Opd *rhs_reg = new Register_Addr_Opd(rhs_compile.get_reg()); 
	if(lhs->get_data_type() == int_data_type) {
		Register_Addr_Opd *minus_reg = new Register_Addr_Opd(machine_desc_object.get_new_register<int_reg>());
		Compute_IC_Stmt *minus = new Compute_IC_Stmt(sub, lhs_reg, rhs_reg, minus_reg);
		minus_compile->append_ics(*minus);
		minus_compile->set_reg(minus_reg->get_reg());
	}
	else if (lhs->get_data_type() == double_data_type) {
		Register_Addr_Opd *minus_reg = new Register_Addr_Opd(machine_desc_object.get_new_register<float_reg>());
		Compute_IC_Stmt *minus = new Compute_IC_Stmt(sub_d, lhs_reg, rhs_reg, minus_reg);
		minus_compile->append_ics(*minus);
		minus_compile->set_reg(minus_reg->get_reg());		
	}
	lhs_compile.get_reg()->reset_register_occupied();
	rhs_compile.get_reg()->reset_register_occupied();
	return *minus_compile;
}

Code_For_Ast & Minus_Ast::compile_and_optimize_ast(Lra_Outcome & lra) {}

Code_For_Ast & Divide_Ast::compile() {
	Code_For_Ast *res_compile = new Code_For_Ast;
	Code_For_Ast & lhs_compile = lhs->compile();
	Code_For_Ast & rhs_compile = rhs->compile();

	list<Icode_Stmt *> lhs_list = lhs_compile.get_icode_list();
	list<Icode_Stmt *> rhs_list = rhs_compile.get_icode_list();
	list<Icode_Stmt *>::iterator itr;

	for(itr = lhs_list.begin(); itr != lhs_list.end(); itr++) {
		res_compile->append_ics(**itr);
	}
	for(itr = rhs_list.begin(); itr != rhs_list.end(); itr++) {
		res_compile->append_ics(**itr);
	}

	Register_Addr_Opd *lhs_reg = new Register_Addr_Opd(lhs_compile.get_reg());
	Register_Addr_Opd *rhs_reg = new Register_Addr_Opd(rhs_compile.get_reg()); 
	if(lhs->get_data_type() == int_data_type) {
		Register_Addr_Opd *res_reg = new Register_Addr_Opd(machine_desc_object.get_new_register<int_reg>());
		Compute_IC_Stmt *res = new Compute_IC_Stmt(divd, lhs_reg, rhs_reg, res_reg);
		res_compile->append_ics(*res);
		res_compile->set_reg(res_reg->get_reg());
	}
	else if (lhs->get_data_type() == double_data_type) {
		Register_Addr_Opd *res_reg = new Register_Addr_Opd(machine_desc_object.get_new_register<float_reg>());
		Compute_IC_Stmt *res = new Compute_IC_Stmt(div_d, lhs_reg, rhs_reg, res_reg);
		res_compile->append_ics(*res);
		res_compile->set_reg(res_reg->get_reg());		
	}
	lhs_compile.get_reg()->reset_register_occupied();
	rhs_compile.get_reg()->reset_register_occupied();
	return *res_compile;
}

Code_For_Ast & Divide_Ast::compile_and_optimize_ast(Lra_Outcome & lra) {}

Code_For_Ast & Mult_Ast::compile() {
	Code_For_Ast *res_compile = new Code_For_Ast;
	Code_For_Ast & lhs_compile = lhs->compile();
	Code_For_Ast & rhs_compile = rhs->compile();

	list<Icode_Stmt *> lhs_list = lhs_compile.get_icode_list();
	list<Icode_Stmt *> rhs_list = rhs_compile.get_icode_list();
	list<Icode_Stmt *>::iterator itr;

	for(itr = lhs_list.begin(); itr != lhs_list.end(); itr++) {
		res_compile->append_ics(**itr);
	}
	for(itr = rhs_list.begin(); itr != rhs_list.end(); itr++) {
		res_compile->append_ics(**itr);
	}

	Register_Addr_Opd *lhs_reg = new Register_Addr_Opd(lhs_compile.get_reg());
	Register_Addr_Opd *rhs_reg = new Register_Addr_Opd(rhs_compile.get_reg()); 
	if(lhs->get_data_type() == int_data_type) {
		Register_Addr_Opd *res_reg = new Register_Addr_Opd(machine_desc_object.get_new_register<int_reg>());
		Compute_IC_Stmt *res = new Compute_IC_Stmt(mult, lhs_reg, rhs_reg, res_reg);
		res_compile->append_ics(*res);
		res_compile->set_reg(res_reg->get_reg());
	}
	else if (lhs->get_data_type() == double_data_type) {
		Register_Addr_Opd *res_reg = new Register_Addr_Opd(machine_desc_object.get_new_register<float_reg>());
		Compute_IC_Stmt *res = new Compute_IC_Stmt(mult_d, lhs_reg, rhs_reg, res_reg);
		res_compile->append_ics(*res);
		res_compile->set_reg(res_reg->get_reg());		
	}
	lhs_compile.get_reg()->reset_register_occupied();
	rhs_compile.get_reg()->reset_register_occupied();
	return *res_compile;
}

Code_For_Ast & Mult_Ast::compile_and_optimize_ast(Lra_Outcome & lra) {}

Code_For_Ast & UMinus_Ast::compile() {
	Code_For_Ast *res_compile = new Code_For_Ast;
	Code_For_Ast & lhs_compile = lhs->compile();

	list<Icode_Stmt *> lhs_list = lhs_compile.get_icode_list();
	list<Icode_Stmt *>::iterator itr;

	for(itr = lhs_list.begin(); itr != lhs_list.end(); itr++) {
		res_compile->append_ics(**itr);
	}

	Register_Addr_Opd *lhs_reg = new Register_Addr_Opd(lhs_compile.get_reg());

	if(lhs->get_data_type() == int_data_type) {
		Register_Addr_Opd *res_reg = new Register_Addr_Opd(machine_desc_object.get_new_register<int_reg>());
		Move_IC_Stmt *res = new Move_IC_Stmt(uminus, lhs_reg, res_reg);
		res_compile->append_ics(*res);
		res_compile->set_reg(res_reg->get_reg());
	}
	else if (lhs->get_data_type() == double_data_type) {
		Register_Addr_Opd *res_reg = new Register_Addr_Opd(machine_desc_object.get_new_register<float_reg>());
		Move_IC_Stmt *res = new Move_IC_Stmt(uminus_d, lhs_reg, res_reg);
		res_compile->append_ics(*res);
		res_compile->set_reg(res_reg->get_reg());		
	}
	lhs_compile.get_reg()->reset_register_occupied();
	return *res_compile;
}

Code_For_Ast & UMinus_Ast::compile_and_optimize_ast(Lra_Outcome & lra) {}

Code_For_Ast & Conditional_Expression_Ast::compile() {
	Code_For_Ast *res_compile = new Code_For_Ast;

	Code_For_Ast & cond_compile = cond->compile();
	list<Icode_Stmt *>::iterator itr;
	list<Icode_Stmt *> cond_list = cond_compile.get_icode_list();
	for(itr = cond_list.begin(); itr != cond_list.end(); itr++) {
		res_compile->append_ics(**itr);
	}

	Register_Addr_Opd* cond_reg;
	string label1 = this->get_new_label();
	if(cond_compile.get_reg() == NULL)
	{
		Label_IC_Stmt *branch;	
		itr = --cond_list.end();
		Tgt_Op temp_op = ((**itr).get_op()).get_op();
		if (temp_op == seq_d || temp_op == slt_d || temp_op == sle_d) {
			branch = new Label_IC_Stmt(bc1f, label1);
		}
		else if (temp_op == sne_d || temp_op == sge_d || temp_op == sgt_d) {
			branch = new Label_IC_Stmt(bc1t, label1);
		}
		res_compile->append_ics(*branch);
	}
	else
	{
		Control_Flow_IC_Stmt *branch;
		cond_reg = new Register_Addr_Opd(cond_compile.get_reg());
		branch = new Control_Flow_IC_Stmt(beq, cond_reg, NULL, label1, 0); 
		res_compile->append_ics(*branch);
	}

	Code_For_Ast & lhs_compile = lhs->compile();
	Code_For_Ast & rhs_compile = rhs->compile();

	list<Icode_Stmt *> lhs_list = lhs_compile.get_icode_list();
	for(itr = lhs_list.begin(); itr != lhs_list.end(); itr++) {
		res_compile->append_ics(**itr);
	}

	Register_Addr_Opd *lhs_reg = new Register_Addr_Opd(lhs_compile.get_reg());
	Register_Addr_Opd *zero_reg = new Register_Addr_Opd(machine_desc_object.spim_register_table[zero]);
	Register_Addr_Opd *res_reg;
	Compute_IC_Stmt *or_stmt;

	if(lhs->get_data_type() == int_data_type) {
		res_reg = new Register_Addr_Opd(machine_desc_object.get_new_register<int_reg>());
	}
	else if (lhs->get_data_type() == double_data_type) {
		res_reg = new Register_Addr_Opd(machine_desc_object.get_new_register<float_reg>());
	}
	or_stmt = new Compute_IC_Stmt(or_t, lhs_reg, zero_reg, res_reg);
	res_compile->append_ics(*or_stmt);
	
	string label2 = this->get_new_label();
	Label_IC_Stmt *jump = new Label_IC_Stmt(j,label2);
	res_compile->append_ics(*jump);

	Label_IC_Stmt *label_stmt = new Label_IC_Stmt(label, label1);
	res_compile->append_ics(*label_stmt);

	list<Icode_Stmt *> rhs_list = rhs_compile.get_icode_list();
	for(itr = rhs_list.begin(); itr != rhs_list.end(); itr++) {
		res_compile->append_ics(**itr);
	}

	Register_Addr_Opd *rhs_reg = new Register_Addr_Opd(rhs_compile.get_reg());

	or_stmt = new Compute_IC_Stmt(or_t, rhs_reg, zero_reg, res_reg);
	res_compile->append_ics(*or_stmt);

	Label_IC_Stmt *label_stmt2 = new Label_IC_Stmt(label, label2);
	res_compile->append_ics(*label_stmt2);

	res_compile->set_reg(res_reg->get_reg());
	if(cond_compile.get_reg() != NULL)
		cond_compile.get_reg()->reset_register_occupied();
	lhs_compile.get_reg()->reset_register_occupied();
	rhs_compile.get_reg()->reset_register_occupied();
	return *res_compile;
}

Code_For_Ast & Relational_Expr_Ast::compile() {
	Code_For_Ast *res_compile = new Code_For_Ast;
	Code_For_Ast & lhs_compile = lhs_condition->compile();
	Code_For_Ast & rhs_compile = rhs_condition->compile();

	list<Icode_Stmt *> lhs_list = lhs_compile.get_icode_list();
	list<Icode_Stmt *> rhs_list = rhs_compile.get_icode_list();
	list<Icode_Stmt *>::iterator itr;

	for(itr = lhs_list.begin(); itr != lhs_list.end(); itr++) {
		res_compile->append_ics(**itr);
	}
	for(itr = rhs_list.begin(); itr != rhs_list.end(); itr++) {
		res_compile->append_ics(**itr);
	}

	map<Relational_Op,Tgt_Op> m1,m2;
	m1[less_equalto] = sle;
	m1[less_than] = slt;
	m1[greater_than] = sgt;
	m1[greater_equalto] = sge;
	m1[equalto] = seq;
	m1[not_equalto] = sne;

	m2[less_equalto] = sle_d;
	m2[less_than] = slt_d;
	m2[greater_than] = sgt_d;
	m2[greater_equalto] = sge_d;
	m2[equalto] = seq_d;
	m2[not_equalto] = sne_d;

	Register_Addr_Opd* lhs_reg = new Register_Addr_Opd(lhs_compile.get_reg());
	Register_Addr_Opd* rhs_reg = new Register_Addr_Opd(rhs_compile.get_reg()); 
	if(lhs_condition->get_data_type() == int_data_type) {
		Register_Addr_Opd* res_reg = new Register_Addr_Opd(machine_desc_object.get_new_register<int_reg>());
		Compute_IC_Stmt *res = new Compute_IC_Stmt(m1[this->rel_op], lhs_reg, rhs_reg, res_reg);
		res_compile->append_ics(*res);
		res_compile->set_reg(res_reg->get_reg());
	}
	else if (rhs_condition->get_data_type() == double_data_type) {
		Move_IC_Stmt *res = new Move_IC_Stmt(m2[this->rel_op], lhs_reg, rhs_reg);
		res_compile->append_ics(*res);
	}
	lhs_compile.get_reg()->reset_register_occupied();
	rhs_compile.get_reg()->reset_register_occupied();
	return *res_compile;
}

Code_For_Ast & Logical_Expr_Ast::compile() {
	Code_For_Ast *res_compile = new Code_For_Ast;
	Code_For_Ast *lhs_compile = new Code_For_Ast;
	Code_For_Ast *rhs_compile = new Code_For_Ast;
	if (this->bool_op == _logical_not) {
		if(rhs_op->get_data_type() == double_data_type){
			cout << "CS316 Error : Floating point operands not supported by Logical Expressions\n";
			exit(1);
		}
		Number_Ast<int> temp(1,int_data_type,-1);
		*lhs_compile = temp.compile();
		*rhs_compile = rhs_op->compile();
	}
	else {
		if(rhs_op->get_data_type() == double_data_type || lhs_op->get_data_type() == double_data_type){
			cout << "CS316 Error : Floating point operands not supported by Logical Expressions\n";
			exit(1);
		}
		*lhs_compile = lhs_op->compile();
		*rhs_compile = rhs_op->compile();
	}

	list<Icode_Stmt *> lhs_list = lhs_compile->get_icode_list();
	list<Icode_Stmt *> rhs_list = rhs_compile->get_icode_list();
	list<Icode_Stmt *>::iterator itr;

	for(itr = lhs_list.begin(); itr != lhs_list.end(); itr++) {
		res_compile->append_ics(**itr);
	}
	for(itr = rhs_list.begin(); itr != rhs_list.end(); itr++) {
		res_compile->append_ics(**itr);
	}

	map<Logical_Op,Tgt_Op> m;
	m[_logical_not] = not_t;
	m[_logical_or] = or_t;
	m[_logical_and] = and_t;

	Register_Addr_Opd* lhs_reg = new Register_Addr_Opd(lhs_compile->get_reg());
	Register_Addr_Opd* rhs_reg = new Register_Addr_Opd(rhs_compile->get_reg()); 
	Register_Addr_Opd* res_reg = new Register_Addr_Opd(machine_desc_object.get_new_register<int_reg>());
	if (bool_op == _logical_not) {
		Compute_IC_Stmt *res = new Compute_IC_Stmt(m[this->bool_op], rhs_reg, lhs_reg, res_reg);
		res_compile->append_ics(*res);
	}
	else {
		Compute_IC_Stmt *res = new Compute_IC_Stmt(m[this->bool_op], lhs_reg, rhs_reg, res_reg);
		res_compile->append_ics(*res);
	}
	res_compile->set_reg(res_reg->get_reg());
	lhs_compile->get_reg()->reset_register_occupied();
	rhs_compile->get_reg()->reset_register_occupied();
	return *res_compile;
}

Code_For_Ast & Selection_Statement_Ast::compile() {
	Code_For_Ast *res_compile = new Code_For_Ast;
	Code_For_Ast & cond_compile = cond->compile();
	list<Icode_Stmt *>::iterator itr;
	list<Icode_Stmt *> cond_list = cond_compile.get_icode_list();
	for(itr = cond_list.begin(); itr != cond_list.end(); itr++) {
		res_compile->append_ics(**itr);
	}

	Code_For_Ast & lhs_compile = then_part->compile();

	string label1 = this->get_new_label();
	Register_Addr_Opd* cond_reg;


	if(cond_compile.get_reg() == NULL)
	{
		Label_IC_Stmt *branch;	
		itr = --cond_list.end();
		Tgt_Op temp_op = ((**itr).get_op()).get_op();
		if (temp_op == seq_d || temp_op == slt_d || temp_op == sle_d) {
			branch = new Label_IC_Stmt(bc1f, label1);
		}
		else if (temp_op == sne_d || temp_op == sge_d || temp_op == sgt_d) {
			branch = new Label_IC_Stmt(bc1t, label1);
		}
		res_compile->append_ics(*branch);
	}
	else
	{
		Control_Flow_IC_Stmt *branch;
		cond_reg = new Register_Addr_Opd(cond_compile.get_reg());
		branch = new Control_Flow_IC_Stmt(beq, cond_reg, NULL, label1, 0); 
		res_compile->append_ics(*branch);
	}

	list<Icode_Stmt *> lhs_list = lhs_compile.get_icode_list();
	for(itr = lhs_list.begin(); itr != lhs_list.end(); itr++) {
		res_compile->append_ics(**itr);
	}

	if(else_part != NULL) {
		string label2 = this->get_new_label();
		Label_IC_Stmt *jump = new Label_IC_Stmt(j,label2);
		res_compile->append_ics(*jump);

		Label_IC_Stmt *label_stmt = new Label_IC_Stmt(label, label1);
		res_compile->append_ics(*label_stmt);	

		Code_For_Ast & rhs_compile = else_part->compile();
		list<Icode_Stmt *> rhs_list = rhs_compile.get_icode_list();
		for(itr = rhs_list.begin(); itr != rhs_list.end(); itr++) {
			res_compile->append_ics(**itr);
		}

		Label_IC_Stmt *label_stmt2 = new Label_IC_Stmt(label, label2);
		res_compile->append_ics(*label_stmt2);

		if(rhs_compile.get_reg() != NULL)
		{
			rhs_compile.get_reg()->reset_register_occupied();
		}
	}
	else{
		Label_IC_Stmt *label_stmt = new Label_IC_Stmt(label, label1);
		res_compile->append_ics(*label_stmt);	
	}	

	if(cond_compile.get_reg() != NULL)
		cond_compile.get_reg()->reset_register_occupied();

	if(lhs_compile.get_reg() != NULL)
		lhs_compile.get_reg()->reset_register_occupied();

	return *res_compile;
}

Code_For_Ast & Iteration_Statement_Ast::compile() {

	Code_For_Ast *res_compile = new Code_For_Ast;

	string label1,label2;
	label1 = this->get_new_label();
	label2 = this->get_new_label();

	if(!(this->is_do_form))
	{
		Label_IC_Stmt *jump = new Label_IC_Stmt(j,label2);
		res_compile->append_ics(*jump);
	}

	Label_IC_Stmt *label_stmt = new Label_IC_Stmt(label, label1);
	res_compile->append_ics(*label_stmt);

	Code_For_Ast & rhs_compile = cond->compile();
	Code_For_Ast & lhs_compile = body->compile();

	list<Icode_Stmt *> lhs_list = lhs_compile.get_icode_list();
	list<Icode_Stmt *>::iterator itr;
	for(itr = lhs_list.begin(); itr != lhs_list.end(); itr++) {
		res_compile->append_ics(**itr);
	}

	Label_IC_Stmt *label_stmt1 = new Label_IC_Stmt(label, label2);
	res_compile->append_ics(*label_stmt1);

	list<Icode_Stmt *> rhs_list = rhs_compile.get_icode_list();
	for(itr = rhs_list.begin(); itr != rhs_list.end(); itr++) {
		res_compile->append_ics(**itr);
	}

	Register_Addr_Opd* cond_reg;
	if(rhs_compile.get_reg() == NULL)
	{
		Label_IC_Stmt *branch;	
		itr = --rhs_list.end();
		Tgt_Op temp_op = ((**itr).get_op()).get_op();
		if (temp_op == seq_d || temp_op == slt_d || temp_op == sle_d) {
			branch = new Label_IC_Stmt(bc1t, label1);
		}
		else if (temp_op == sne_d || temp_op == sge_d || temp_op == sgt_d) {
			branch = new Label_IC_Stmt(bc1f, label1);
		}
		res_compile->append_ics(*branch);

	}
	else
	{
		Control_Flow_IC_Stmt *branch;
		cond_reg = new Register_Addr_Opd(rhs_compile.get_reg());
		branch = new Control_Flow_IC_Stmt(bne, cond_reg, NULL, label1, 0); 
		res_compile->append_ics(*branch);
	}

	if(lhs_compile.get_reg() != NULL)
		lhs_compile.get_reg()->reset_register_occupied();
	if(rhs_compile.get_reg() != NULL)
		rhs_compile.get_reg()->reset_register_occupied();

	return *res_compile;
}

Code_For_Ast & Sequence_Ast::compile() {
	list<Ast *>::iterator itr;
	Code_For_Ast *res_compile = new Code_For_Ast;
	for(itr = this->statement_list.begin(); itr != this->statement_list.end(); itr++) {
		Code_For_Ast & statement_compile =  (*itr)->compile();
		list<Icode_Stmt *> statement_list = statement_compile.get_icode_list();
		list<Icode_Stmt *>::iterator itr1;
		for(itr1 = statement_list.begin(); itr1 != statement_list.end(); itr1++) {
			res_compile->append_ics(**itr1);
			this->sa_icode_list.push_back(*itr1);
		}
		if(statement_compile.get_reg() != NULL)
			statement_compile.get_reg()->reset_register_occupied();
	}
	return *res_compile;
}

Code_For_Ast & Print_Ast::compile() {
 	
 	Code_For_Ast *res_compile = new Code_For_Ast;

 	Const_Opd<int> *constant;
 	Register_Descriptor* r1 = machine_desc_object.spim_register_table[v0];
	Register_Addr_Opd *v0 = new Register_Addr_Opd(r1);
 	if(this->var->get_data_type() == int_data_type){
 		constant = new Const_Opd<int>(1);
	}
	else if(this->var->get_data_type() == double_data_type){
		constant = new Const_Opd<int>(3);
	}
	Move_IC_Stmt *load1 = new Move_IC_Stmt(imm_load, constant, v0);
	res_compile->append_ics(*load1);

	Mem_Addr_Opd *mem_addr = new Mem_Addr_Opd(this->var->get_symbol_entry());
	// Register_Descriptor* r2 = machine_desc_object.get_new_register<argument>();
	Register_Descriptor* r2;
	Register_Addr_Opd *argument;
	Move_IC_Stmt *load2;
	if(this->var->get_data_type() == int_data_type){
		r2 = machine_desc_object.spim_register_table[a0];
		argument = new Register_Addr_Opd(r2);
		load2 = new Move_IC_Stmt(load, mem_addr, argument);
	}
	else if(this->var->get_data_type() == double_data_type){
		r2 = machine_desc_object.spim_register_table[f12];
		argument = new Register_Addr_Opd(r2);
		load2 = new Move_IC_Stmt(load_d, mem_addr, argument);
	}

	res_compile->append_ics(*load2);

	Print_IC_Stmt *print = new Print_IC_Stmt();
	res_compile->append_ics(*print);

	r1->reset_register_occupied();
	r2->reset_register_occupied();
	return *res_compile;
 }

Code_For_Ast & Return_Ast::compile() {
	Code_For_Ast *res_compile = new Code_For_Ast;

	if(this->return_value != NULL)
	{
		Code_For_Ast & temp_compile =  this->return_value->compile();
		list<Icode_Stmt *> temp_list = temp_compile.get_icode_list();
		list<Icode_Stmt *>::iterator itr1;
		for(itr1 = temp_list.begin(); itr1 != temp_list.end(); itr1++) {
			res_compile->append_ics(**itr1);
		}

		Register_Addr_Opd *reg_opd = new Register_Addr_Opd(temp_compile.get_reg());
		Register_Addr_Opd *return_register;
		Move_IC_Stmt *result_register_set;
		Data_Type return_type = program_object.get_procedure_prototype(this->proc_name)->get_return_type();
		if(	return_type ==  int_data_type)
		{
			return_register = new Register_Addr_Opd(machine_desc_object.spim_register_table[v1]);
			result_register_set = new Move_IC_Stmt(mov, reg_opd, return_register);
		}
		else if(return_type == double_data_type)
		{
			return_register = new Register_Addr_Opd(machine_desc_object.spim_register_table[f0]);
			result_register_set = new Move_IC_Stmt(move_d, reg_opd, return_register);
		}

		res_compile->append_ics(*result_register_set);
		
		Label_IC_Stmt *jump_and_link;
		string epilogue = "epilogue_" + this->proc_name;
		jump_and_link = new Label_IC_Stmt(j, epilogue);
		res_compile->append_ics(*jump_and_link);

		if(temp_compile.get_reg() != NULL)
				temp_compile.get_reg()->reset_register_occupied();
		res_compile->set_reg(return_register->get_reg());
	}
	else 
	{
		Label_IC_Stmt *jump_and_link;
		string epilogue = "epilogue_" + this->proc_name;
		jump_and_link = new Label_IC_Stmt(j, epilogue);
		res_compile->append_ics(*jump_and_link);
	}

	return *res_compile;
}

Code_For_Ast & Return_Ast::compile_and_optimize_ast(Lra_Outcome & lra) {}

Code_For_Ast & Call_Ast::compile() {
	list<Ast *>::reverse_iterator itr;
	Code_For_Ast *res_compile = new Code_For_Ast;
	int stack_offset = 0;
	Procedure* p = program_object.get_procedure_prototype(this->procedure_name);
	int idx = actual_param_list.size();

	for(itr = actual_param_list.rbegin(); itr != actual_param_list.rend(); itr++) {
		Code_For_Ast & arg_compile =  (*itr)->compile();
		list<Icode_Stmt *> arg_list = arg_compile.get_icode_list();
		list<Icode_Stmt *>::iterator itr1;
		for(itr1 = arg_list.begin(); itr1 != arg_list.end(); itr1++) {
			res_compile->append_ics(**itr1);
		}


		string temp_name = p->get_variable_in_formal_list(idx);
		Symbol_Table_Entry *arg_entry = new Symbol_Table_Entry(temp_name,int_data_type,-1,sp_ref);
		arg_entry->set_symbol_scope(formal);
		arg_entry->set_start_offset(stack_offset);
		Mem_Addr_Opd *mem_addr = new Mem_Addr_Opd(*arg_entry);
		Register_Addr_Opd *reg_opd = new Register_Addr_Opd(arg_compile.get_reg());
		Move_IC_Stmt *store_stmt;
		if((*itr)->get_data_type() == int_data_type){
			stack_offset -= 4;
			store_stmt = new Move_IC_Stmt(store, reg_opd, mem_addr);
		}
		else if( (*itr)->get_data_type() == double_data_type ){
			stack_offset -= 8;
			store_stmt = new Move_IC_Stmt(store_d, reg_opd, mem_addr);
		}
		
		res_compile->append_ics(*store_stmt);

		if(arg_compile.get_reg() != NULL)
			arg_compile.get_reg()->reset_register_occupied();

		idx--;
	}

	Register_Addr_Opd *stack_pointer = new Register_Addr_Opd(machine_desc_object.spim_register_table[sp]);
	Const_Opd<int> *offset = new Const_Opd<int>(-stack_offset);
	
	if (stack_offset != 0) {
		Compute_IC_Stmt *subtract_stack = new Compute_IC_Stmt(sub, stack_pointer, offset, stack_pointer);
		res_compile->append_ics(*subtract_stack);
	}

	Label_IC_Stmt *jump_and_link;
	jump_and_link = new Label_IC_Stmt(jal, this->procedure_name);
	res_compile->append_ics(*jump_and_link);

	if(stack_offset != 0) {
		Compute_IC_Stmt *restore_stack = new Compute_IC_Stmt(add, stack_pointer, offset, stack_pointer);
		res_compile->append_ics(*restore_stack);
	}
	
	Register_Addr_Opd *result_reg;
	Register_Addr_Opd *return_register;
	Move_IC_Stmt *result_register_set;
	Data_Type return_type = program_object.get_procedure_prototype(this->procedure_name)->get_return_type();
	if(	return_type ==  int_data_type)
	{
		result_reg = new Register_Addr_Opd(machine_desc_object.get_new_register<int_reg>());
		return_register = new Register_Addr_Opd(machine_desc_object.spim_register_table[v1]);
		result_register_set = new Move_IC_Stmt(mov, return_register, result_reg);
	}
	else if(return_type == double_data_type)
	{
		result_reg = new Register_Addr_Opd(machine_desc_object.get_new_register<float_reg>());
		return_register = new Register_Addr_Opd(machine_desc_object.spim_register_table[f0]);
		result_register_set = new Move_IC_Stmt(move_d, return_register, result_reg);
	}
	else if (return_type == void_data_type) {
		return *res_compile;
	}

	res_compile->append_ics(*result_register_set);

	if(this->return_value_reg->is_used_for_fn_return())
	{
		res_compile->set_reg(result_reg->get_reg());
		this->set_register(result_reg->get_reg());		
	}
	else
	{
		result_reg->get_reg()->reset_register_occupied();
	}

	return *res_compile;
}

Code_For_Ast & Call_Ast::compile_and_optimize_ast(Lra_Outcome & lra) {}