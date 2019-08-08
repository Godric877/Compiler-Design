%{
#include "parser.tab.h"
// #include "storetokens.hh"
int lineno = 1;
%}

ws [ \t]+                           
letter [A-Za-z_]
digit [0-9]
name {letter}({letter}|{digit})*
number {digit}+
float {digit}*\.{digit}+|{digit}+\.{digit}*
comment "//".* 
arithop [+\-"*""/"]
assignop =
metachar [,;(){}?:]

%%
\n 			{lineno++;}
{ws}  		{/*no action, no return*/}
{comment}   {/*no action, no return*/}
int			{store_token_name("INTEGER",yytext,yylineno); 
            // printf("(Token_type, lexeme) = [INTEGER: %s] \n", yytext); 
			return INTEGER;}
float		{store_token_name("FLOAT",yytext,yylineno); 
            // printf("(Token_type, lexeme) = [FLOAT: %s] \n", yytext);
			return FLOAT;}
void		{store_token_name("VOID",yytext,yylineno); 
            // printf("(Token_type, lexeme) = [VOID: %s] \n", yytext);
			return VOID;}
if          {store_token_name("IF",yytext,yylineno); 
            // printf("(Token_type, lexeme) = [ARITHOP: %s] \n", yytext);
			return IF;}
else        {store_token_name("ELSE",yytext,yylineno); 
            // printf("(Token_type, lexeme) = [ARITHOP: %s] \n", yytext);
			return ELSE;}
while      	{store_token_name("WHILE",yytext,yylineno); 
            // printf("(Token_type, lexeme) = [ARITHOP: %s] \n", yytext);
			return WHILE;}
do        	{store_token_name("DO",yytext,yylineno); 
            // printf("(Token_type, lexeme) = [ARITHOP: %s] \n", yytext);
			return DO;}
print       {store_token_name("PRINT",yytext,yylineno); 
            // printf("(Token_type, lexeme) = [ARITHOP: %s] \n", yytext);
			return PRINT;}
return 		{store_token_name("RETURN",yytext,yylineno); 
            // printf("(Token_type, lexeme) = [ARITHOP: %s] \n", yytext);
			return RETURN;}
{number}	{yylval.integer_value=atoi(yytext); 
			store_token_name("NUM",yytext,yylineno); 
            // printf("(Token_type, lexeme) = [INTEGER NUMBER %s] \n", yytext); 
			return INTEGER_NUMBER;}
{float}	    {yylval.double_value=atof(yytext); 
			store_token_name("FNUM",yytext,yylineno); 
            // printf("(Token_type, lexeme) = [FLOAT NUMBER %s] \n", yytext); 
			return DOUBLE_NUMBER;}
{name}	    {std::string *s = new std::string(yytext);
			yylval.string_value = s; 
			store_token_name("NAME",yytext,yylineno); 
            // printf("(Token_type, lexeme) = [ID: %s] \n", yytext); 
			return NAME;}
{assignop}	{store_token_name("ASSIGN_OP",yytext,yylineno); 
            // printf("(Token_type, lexeme) = [ASSIGNOP: %s] \n", yytext);
			return ASSIGN;}
{arithop}	{store_token_name("ARITHOP",yytext,yylineno); 
            // printf("(Token_type, lexeme) = [ARITHOP: %s] \n", yytext);
			return yytext[0];}
"=="        {store_token_name("EQUAL",yytext,yylineno); 
            // printf("(Token_type, lexeme) = [ARITHOP: %s] \n", yytext);
			return EQUAL;}
"<="        {store_token_name("LESS_THAN_EQUAL",yytext,yylineno); 
            // printf("(Token_type, lexeme) = [ARITHOP: %s] \n", yytext);
			return LESS_THAN_EQUAL;}
">="        {store_token_name("GREATER_THAN_EQUAL",yytext,yylineno); 
            // printf("(Token_type, lexeme) = [ARITHOP: %s] \n", yytext);
			return GREATER_THAN_EQUAL;}
"<"        	{store_token_name("LESS_THAN",yytext,yylineno); 
            // printf("(Token_type, lexeme) = [ARITHOP: %s] \n", yytext);
			return LESS_THAN;}
">"        	{store_token_name("GREATER_THAN",yytext,yylineno); 
            // printf("(Token_type, lexeme) = [ARITHOP: %s] \n", yytext);
			return GREATER_THAN;}
"!="        {store_token_name("NOT_EQUAL",yytext,yylineno); 
            // printf("(Token_type, lexeme) = [ARITHOP: %s] \n", yytext);
			return NOT_EQUAL;}
"&&"        {store_token_name("AND",yytext,yylineno); 
            // printf("(Token_type, lexeme) = [ARITHOP: %s] \n", yytext);
			return AND;}
"||"        {store_token_name("OR",yytext,yylineno); 
            // printf("(Token_type, lexeme) = [ARITHOP: %s] \n", yytext);
			return OR;}
"!"        	{store_token_name("NOT",yytext,yylineno); 
            // printf("(Token_type, lexeme) = [ARITHOP: %s] \n", yytext);
			return NOT;}
{metachar}  {store_token_name("META CHAR",yytext,yylineno); 
            // printf("(Token_type, lexeme) = [METACHAR: %s] \n", yytext);
			return yytext[0];}
.           {printf("cs316: Error : Found Lexical Error\n");}

