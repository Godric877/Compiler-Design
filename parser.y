%{
 extern "C"  void yyerror(char *);
 int yylex(void);
 extern int lineno;
 Procedure* curr_procedure;
 Symbol_Table glob_symtab;
%}

%token INTEGER_NUMBER
%token BBNUM
%token DOUBLE_NUMBER
%token NAME
%token RETURN
%token INTEGER
%token FLOAT
%token ASSIGN
%token VOID
%token UMINUS
%token IF
%token ELSE
%token WHILE
%token DO
%token EQUAL
%token NOT_EQUAL
%token GREATER_THAN
%token LESS_THAN
%token GREATER_THAN_EQUAL
%token LESS_THAN_EQUAL
%token AND
%token OR
%token NOT
%token PRINT

%left IF
%left ELSE
%left EQUAL NOT_EQUAL
%left GREATER_THAN GREATER_THAN_EQUAL LESS_THAN LESS_THAN_EQUAL
%right '?' ':'
%left OR
%left AND
%left '+' '-'
%left '*' '/'
%right UMINUS NOT


%union {
    int integer_value;
    int* ptr_int;
	double double_value;
	std::string * string_value;
	list<Ast *> * ast_list;
	Ast * ast;
	Symbol_Table * symbol_table;
	Symbol_Table_Entry * symbol_entry;
	list<Symbol_Table_Entry *> * symbol_table_entry_list;
	Basic_Block * basic_block;
	Procedure * procedure;
	// list<Procedure *> * proc_list;
	// Sequence_Ast * sequence_ast;
}

%type <procedure> procedure_definition
%type <symbol_table_entry_list> variable_list declaration variable_declaration variable_declaration_list optional_variable_declaration_list
%type <symbol_table> formal_paramter_list1 formal_paramter_list2
%type <symbol_entry> parameter parameter2
%type <ast> variable constant expr statement assignment_statement selection_statement print_statement function_call
%type <ast> iteration_statement boolean_expr rel_expr block_statement_list seq_statement_list return_statement
%type <ast_list> statement_list  argument_list
%type <string_value> NAME INTEGER FLOAT ASSIGN VOID 
%type <integer_value> INTEGER_NUMBER
%type <ptr_int> type
%type <double_value> DOUBLE_NUMBER 

%%

program:
	global_declaration_list procedure_definition_list
	{
		if(!program_object.is_procedure_exists("main")) {
			cout << "cs316: Error: line " << lineno << " : Main should be defined\n";
			exit(0);
		}
		else {
			Procedure * temp = program_object.get_procedure_prototype("main");
			if(temp->get_return_type() != void_data_type) {
				cout << "cs316: Error: line " << lineno << " : Main function return type should be void\n";	
				exit(0);	
			}
			program_object.called_proc_are_defined_check();
		}
	}
	|
	procedure_definition_list
	{
		if(!program_object.is_procedure_exists("main")) {
			cout << "cs316: Error: line " << lineno << " : Main should be defined\n";
			exit(0);
		}
		else {
			Procedure * temp = program_object.get_procedure_prototype("main");
			if(temp->get_return_type() != void_data_type) {
				cout << "cs316: Error: line " << lineno << " : Main function return type should be void\n";	
				exit(0);	
			}
			program_object.called_proc_are_defined_check();
		}
	}
	;


global_declaration_list:
	variable_declaration
	{
		list<Symbol_Table_Entry *>::iterator itr;
		for (itr = $1->begin(); itr!= $1->end(); itr++)
		{
			if( !(glob_symtab.variable_in_symbol_list_check((*itr)->get_variable_name())) )
			{
				glob_symtab.push_symbol(*itr);
			}
			else
			{
				fprintf(stderr, "cs316: Error : Line: %d: Variable is declared twice\n",lineno);
				exit(1);
			}
		}
		program_object.set_global_table(glob_symtab);
	}
	|
	procedure_declaration
	{

	}
	|
	global_declaration_list variable_declaration
	{
		// printf("== Declaration ==\n");
		list<Symbol_Table_Entry *>::iterator itr;
		for (itr = $2->begin(); itr!= $2->end(); itr++)
		{
			if( !(glob_symtab.variable_in_symbol_list_check((*itr)->get_variable_name())) )
			{
				glob_symtab.push_symbol(*itr);
			}
			else
			{
				fprintf(stderr, "cs316: Error : Line: %d: Variable is declared twice\n",lineno);
				exit(1);
			}
		}
		program_object.set_global_table(glob_symtab);
	}
	|
	global_declaration_list procedure_declaration
	{

	}
	;

procedure_declaration:
	type NAME '(' formal_paramter_list1 ')' ';'
	{
		map<int,Data_Type> m;
		m[0] = void_data_type;
		m[1] = int_data_type;
		m[2] = double_data_type;

		// if(program_object.variable_in_proc_map_check(*$2)) {
		// 	cout << "cs316: Error : Redeclaration of the same function\n";
		// 	exit(0);
		// }

		string name;
		if(*$2 != "main") 
			name = *$2 + "_";
		else
			name = *$2;
		curr_procedure = new Procedure(m[*$1], name, lineno);
		program_object.set_proc_to_map(name,curr_procedure);
		$4->set_table_scope(formal);
		curr_procedure->set_formal_param_list(*$4);

		// cout << name << " procedure declared : debug" << endl;
	}
	|
	type NAME '(' formal_paramter_list2 ')' ';'
	{
		map<int,Data_Type> m;
		m[0] = void_data_type;
		m[1] = int_data_type;
		m[2] = double_data_type;

		// if(program_object.variable_in_proc_map_check(*$2)) {
		// 	cout << "cs316: Error : Redeclaration of the same function\n";
		// 	exit(0);
		// }

		string name = *$2 + "_";
		curr_procedure = new Procedure(m[*$1], name, lineno);
		program_object.set_proc_to_map(name,curr_procedure);
		$4->set_table_scope(formal);
		curr_procedure->set_formal_param_list(*$4);

		// cout << name << " procedure declared : debug" << endl;
	}
	;

procedure_definition_list:
	procedure_definition_list procedure_definition
	|
	procedure_definition
	;

optional_variable_declaration_list:
	/* empty */
	{
		// printf("Empty variable declaration list\n");
		$$ = new list<Symbol_Table_Entry *>;
	}
	|	variable_declaration_list
	{
		// printf("Non Empty variable declaration list\n");
		$$ = $1;
	}
	;

variable_declaration_list:
	variable_declaration
	{
		// printf("== Declaration ==\n");
		$$ = new list<Symbol_Table_Entry *>;
		list<Symbol_Table_Entry *>::iterator itr;
		for (itr = $1->begin(); itr!= $1->end(); itr++)
		{
			$$->push_back(*itr);
		}
	}
	|	variable_declaration_list
		variable_declaration
	{
		// printf("== Declaration ==\n");
		list<Symbol_Table_Entry *>::iterator itr;
		for (itr = $2->begin(); itr!= $2->end(); itr++)
		{
			$$->push_back(*itr);
		}
	}
	;

variable_declaration:
	declaration ';'
	{
		$$ = $1;
	}
	;

declaration:
	type variable_list
	{
		map<int,Data_Type> m;
		m[0] = void_data_type;
		m[1] = int_data_type;
		m[2] = double_data_type;
		// printf("Integer variable list\n");
		list<Symbol_Table_Entry *>::iterator itr;
		for (itr = $2->begin(); itr != $2->end(); itr++)
		{
			(*itr)->set_data_type(m[*$1]);
		}
		$$ = $2;
	}
    ;

variable_list:
    NAME
	{
		// std::cout << "Variable " << *$1 << std::endl;
		$$ = new list<Symbol_Table_Entry *>;
		string name = *$1 + "_"; 
		Symbol_Table_Entry* symtab_entry = new Symbol_Table_Entry(name, (Data_Type) NULL, lineno);
		$$->push_back(symtab_entry);
	}
	|   variable_list ',' NAME
	{
		// std::cout << "Variable " << *$3 << std::endl;
		string name = *$3 + "_"; 
		Symbol_Table_Entry* symtab_entry = new Symbol_Table_Entry(name, (Data_Type) NULL, lineno);
		$$->push_back(symtab_entry);
	}
	;

formal_paramter_list1:
	/* empty */
	{
		$$ = new Symbol_Table;
	}
	|
	parameter
	{
		$$ = new Symbol_Table;
		$$->push_symbol($1);
	}
	|
	formal_paramter_list1 ',' parameter
	{
		$$ = $1;
		$$->push_symbol($3);
	}
	;

formal_paramter_list2:
	parameter2
	{
		$$ = new Symbol_Table;
		$$->push_symbol($1);
	}
	|
	formal_paramter_list2 ',' parameter2
	{
		$$ = $1;
		$$->push_symbol($3);
	}
	;

parameter:
	INTEGER NAME
	{
		string name = *$2 + "_";
		$$ = new Symbol_Table_Entry(name,int_data_type,lineno);
	}
	|
	FLOAT NAME
	{
		string name = *$2 + "_";
		$$ = new Symbol_Table_Entry(name,double_data_type,lineno);
	}
	;


parameter2:
	INTEGER
	{
		string s = "";
		$$ = new Symbol_Table_Entry(s,int_data_type,lineno);
	}
	|
	FLOAT
	{
		string s = "";
		$$ = new Symbol_Table_Entry(s,double_data_type,lineno);
	}
	;


type:
	VOID
	{
		int* a = new int;
		*a = 0;
		$$ = a;
	}
	|
	INTEGER
	{
		int* a = new int;
		*a = 1;
		$$ = a;
	}
	|
	FLOAT
	{
		int* a = new int;
		*a = 2;
		$$ = a;
	}
	;

procedure_definition:
	type NAME '(' formal_paramter_list1 ')'
    '{'
    {
		map<int,Data_Type> m;
		m[0] = void_data_type;
		m[1] = int_data_type;
		m[2] = double_data_type;

		string name;
		if(*$2 != "main") 
			name = *$2 + "_";
		else
			name = *$2;
		if(program_object.variable_in_proc_map_check(name)) {
			Procedure * temp = program_object.get_procedure_prototype(name);
			if(temp->get_return_type() != m[*$1]) {
				cout << "cs316: Error: line " << lineno << " : return type in declaration and definition not matching\n";
				exit(0);
			}
		}

		curr_procedure = new Procedure(m[*$1], name, lineno);
		$4->set_table_scope(formal);
		curr_procedure->set_formal_param_list(*$4);
		curr_procedure->set_proc_is_defined();
		program_object.set_proc_to_map(name,curr_procedure);
    }
	optional_variable_declaration_list
	{
		// printf("======== Local Declaration ========\n");

		Symbol_Table* local_table = new Symbol_Table();
		list<Symbol_Table_Entry *>::iterator itr;

		for (itr = $8->begin(); itr!= $8->end(); itr++)
		{
			if( !(local_table->variable_in_symbol_list_check((*itr)->get_variable_name())) )
			{
				local_table->push_symbol(*itr);
			}
			else
			{
				fprintf(stderr, "cs316: Error : Line: %d: Variable is declared twice\n",lineno);
				exit(1);
			}
		}


		local_table->set_table_scope(local);
		curr_procedure->set_local_list(*local_table);

		// printf("======== Local variable list defined ========\n");
	}
    statement_list
    '}'
	{
		// cout << "======== Statement list ========" << endl;

		curr_procedure->set_ast_list(*$10);
		$$ = curr_procedure;

		// cout << "======== Statement list defined ========" << endl;

		// cout << *$2 << " procedure defined : debug" << endl;
	}
	;


statement_list:
	/* empty */
	{
		// printf("Empty statement list\n");
		$$ = new list<Ast *>;
	}
	|	statement_list
		statement
		{
			// printf("== Statement ==\n");
			$$->push_back($2);
			// printf("== Statements added ==\n");
		}
	;

block_statement_list:
	/* empty */
	{
		cout << "\ncs316: Error :  Block of statements cannot be empty (Invariant at line 241, file parser.y).\n" << endl;
		// $$ = new Sequence_Ast(lineno);
		exit(1);
	}  
	|
	seq_statement_list
	;

seq_statement_list:
	statement
	{
		// printf("Sequence_Ast created\n");
		$$ = new Sequence_Ast(lineno);
		((Sequence_Ast*)$$)->ast_push_back($1);
		// printf("Sequence_Ast pushed\n");
	}
	|	seq_statement_list
		statement
		{
			// printf("== Sequence Statement ==\n");
			((Sequence_Ast*)$$)->ast_push_back($2);
			// printf("== Sequence Statement added ==\n");

		}
	;

statement:
	assignment_statement
	|
	selection_statement
	|
	iteration_statement
	|
	print_statement
	|
	return_statement
	|
	'{' block_statement_list '}'
	{
		$$ = $2;
	}
	|
	function_call ';'
	;

return_statement:
	RETURN expr ';'
	{
		if(curr_procedure->get_return_type() != $2->get_data_type()) {
			cout << "cs316: Error: line " << lineno << " : Return type not matching\n";
			exit(0);
		}
		$$ = new Return_Ast($2, curr_procedure->get_proc_name(), lineno);	
	}
	|
	RETURN ';'
	{
		if(curr_procedure->get_return_type() != void_data_type) {
			cout << "cs316: Error: line " << lineno << " : Return type not matching\n";
			exit(0);
		}
		$$ = new Return_Ast(NULL, curr_procedure->get_proc_name(), lineno);
	}
	;


print_statement:
	PRINT variable ';'
	{
		$$ = new Print_Ast($2, lineno);
	}
	;


assignment_statement:
	variable ASSIGN expr ';'
	{
		Symbol_Table_Entry symtab_entry;

		if (curr_procedure->variable_in_symbol_list_check( $1->get_symbol_entry().get_variable_name() ))
		{
			symtab_entry = curr_procedure->get_symbol_table_entry( $1->get_symbol_entry().get_variable_name() );
		}
		else if (curr_procedure->variable_in_formal_list_check( $1->get_symbol_entry().get_variable_name() ))
		{
			symtab_entry = curr_procedure->get_formal_param_entry( $1->get_symbol_entry().get_variable_name() );
		}
		else if (program_object.variable_in_symbol_list_check( $1->get_symbol_entry().get_variable_name() ))
		{
			symtab_entry = program_object.get_symbol_table_entry( $1->get_symbol_entry().get_variable_name() );
		}
		else {
			fprintf(stderr, "cs316: Error : Line: %d: Variable has not been declared \n",lineno);
			exit(1);
		}

		$$ = new Assignment_Ast($1, $3, lineno);
		$$->check_ast();
	}
	| 
	variable ASSIGN function_call ';'
	{
		Symbol_Table_Entry symtab_entry;

		if (curr_procedure->variable_in_symbol_list_check( $1->get_symbol_entry().get_variable_name() ))
			symtab_entry = curr_procedure->get_symbol_table_entry( $1->get_symbol_entry().get_variable_name() );

		else if (program_object.variable_in_symbol_list_check( $1->get_symbol_entry().get_variable_name() ))
			symtab_entry = program_object.get_symbol_table_entry( $1->get_symbol_entry().get_variable_name() );

		else {
			fprintf(stderr, "cs316: Error : Line: %d: Variable has not been declared \n",lineno);
			exit(1);
		}

		$$ = new Assignment_Ast($1, $3, lineno);
		$$->check_ast();
		Register_Descriptor* reg = new Register_Descriptor(none, "none", int_num, fn_result);
		reg->set_used_for_fn_return();
		((Call_Ast*)$3)->set_register(reg);
	}
	;

function_call:
	NAME '(' argument_list ')'
	{
		string proc_name = *$1 + "_";
		if(program_object.variable_in_proc_map_check(proc_name)) {
			$$ = new Call_Ast(proc_name, lineno);
			((Call_Ast*)$$)->set_actual_param_list(*$3);
			Procedure* p = program_object.get_procedure_prototype(proc_name);
    		Symbol_Table & param_list = p->get_formal_param_list();
			((Call_Ast*)$$)->check_actual_formal_param(param_list);
			p->set_proc_is_called();
		}
		else {
			cout << "cs316: Error : Line: " << lineno << " : Procedure corresponding to the name is not found\n";
		}
	}
	;

argument_list:
	/* empty */
	{
		$$ = new list<Ast *>;
	}
	|
	expr
	{
		$$ = new list<Ast *>;
		$$->push_back($1);
	}
	|
	argument_list ',' expr
	{
		$$ = $1;
		$$->push_back($3);
	}
	;


selection_statement:
	IF '(' boolean_expr ')' statement ELSE statement %prec ELSE
	{
		$$ = new Selection_Statement_Ast($3, $5, $7, lineno);
	}
	|
	IF '(' boolean_expr ')' statement %prec IF
	{
		$$ = new Selection_Statement_Ast($3, $5, NULL, lineno);
	}
	;

iteration_statement:
	WHILE '(' boolean_expr ')' statement
	{
		$$ = new Iteration_Statement_Ast( $3, $5, lineno, false);
	}
	|
	DO statement WHILE '(' boolean_expr ')' ';'
	{
		$$ = new Iteration_Statement_Ast( $5, $2, lineno, true);
	}
	;

boolean_expr:
	boolean_expr OR boolean_expr
	{
		$$ = new Logical_Expr_Ast($1, _logical_or, $3, lineno);
	}
	|
	boolean_expr AND boolean_expr
	{
		$$ = new Logical_Expr_Ast($1, _logical_and, $3, lineno);
	}
	|
	NOT boolean_expr
	{
		$$ = new Logical_Expr_Ast(NULL, _logical_not, $2, lineno);
	}
	|
	'(' boolean_expr ')'
	{
		$$ = $2; 
	}
	|
	rel_expr	
	;

rel_expr:
	expr EQUAL expr
	{
		$$ = new Relational_Expr_Ast($1, equalto, $3, lineno);
		$$->check_ast();
	}
	|
	expr NOT_EQUAL expr
	{
		$$ = new Relational_Expr_Ast($1, not_equalto, $3, lineno);
		$$->check_ast();
	}
	|
	expr GREATER_THAN expr
	{
		$$ = new Relational_Expr_Ast($1, greater_than, $3, lineno);
		$$->check_ast();
	}
	|
	expr GREATER_THAN_EQUAL expr
	{
		$$ = new Relational_Expr_Ast($1, greater_equalto, $3, lineno);
		$$->check_ast();
	}
	|
	expr LESS_THAN expr
	{
		$$ = new Relational_Expr_Ast($1, less_than, $3, lineno);
		$$->check_ast();
	}
	|
	expr LESS_THAN_EQUAL expr
	{
		$$ = new Relational_Expr_Ast($1, less_equalto, $3, lineno);
		$$->check_ast();
	}
	;

variable:
	NAME
	{
		string name = *$1 + "_";
		if (curr_procedure->variable_in_symbol_list_check(name))
		{
			$$ = new Name_Ast(name, curr_procedure->get_symbol_table_entry(name), lineno);			
		} 
		else if (curr_procedure->variable_in_formal_list_check(name))
		{
			$$ = new Name_Ast(name, curr_procedure->get_formal_param_entry(name), lineno);
		}
		else if (program_object.variable_in_symbol_list_check(name))
		{
			$$ = new Name_Ast(name, program_object.get_symbol_table_entry(name), lineno);
		}
		else {
			fprintf(stderr, "cs316: Error : Line: %d: Variable has not been declared\n",lineno);
			exit(1);
		}			
	}
	;

constant:
	INTEGER_NUMBER
	{ $$ = new Number_Ast<int>($1, int_data_type, lineno); }
	| DOUBLE_NUMBER
	{ $$ = new Number_Ast<double>($1, double_data_type, lineno); }
	;

expr: 
	boolean_expr '?' expr ':' expr
	{
		$$ = new Conditional_Expression_Ast($1, $3, $5, lineno);
		$$->set_data_type($3->get_data_type());
		$$->check_ast();
	}
	| 
	expr '+' expr
	{
		// printf("Found a PLUS expression %d\n", lineno);
		$$ = new Plus_Ast($1, $3, lineno);
		$$->set_data_type($1->get_data_type());
		$$->check_ast();
	}
	|
	expr '-' expr
	{
		// printf("Found a MINUS expression %d\n", lineno);
		$$ = new Minus_Ast($1, $3, lineno);
		$$->set_data_type($1->get_data_type());
		$$->check_ast();
	}
	|
	expr '*' expr
	{
		// printf("Found a MULT expression %d\n", lineno);
		$$ = new Mult_Ast($1, $3, lineno);
		$$->set_data_type($1->get_data_type());
		$$->check_ast();
	}
	|
	expr '/' expr
	{
		// printf("Found a DIVIDE expression %d\n", lineno);
		$$ = new Divide_Ast($1, $3, lineno);
		$$->set_data_type($1->get_data_type());
		$$->check_ast();
	}
	|
	'(' expr ')'
	{
		$$ = $2;
		$$->set_data_type($2->get_data_type());
	}
	|
	'-' expr %prec UMINUS
	{
		// printf("Found a UNARY MINUS expression %d\n", lineno);
		$$ = new UMinus_Ast($2, NULL, lineno);
		$$->set_data_type($2->get_data_type());
		// printf("created a UNARY MINUS ast %d\n", lineno);
	}
	|
	variable
	{
		// printf("Found an expression consisting of a variable\n");
		$$ = $1;
		$$->set_data_type($1->get_data_type());
	}
	|
	constant
	{
		// printf("Found an expression consisting of a number\n");
		$$ = $1;
	}
	;

%%

extern "C" void yyerror(char *s) {
 fprintf(stderr, "%s\n", s);
 return;
}

