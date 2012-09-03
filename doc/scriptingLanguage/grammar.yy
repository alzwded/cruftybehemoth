%{
#include <stdio.h>
#include <string.h>
#include <ctype.h>
/*
    preliminary grammar

    this particular file is not meant to compile, nor is it final

    treat this as documentation

    it will need to be reentrant

    TODO quoted string vs. identifier
        could return quoted string from the lexer itself?

    TODO increment, decrement operations

    bison -dtv *.yy
*/
extern FILE* yyin;
#define Entity void
#define Level void
#define YYDEBUG 1
int yyerror(Entity* THIS, Level* LVL, char*);
%}

%define api.pure
%define api.push_pull "push"
/*%pure-parser*/

%parse-param { Entity* THIS }
%parse-param { Level* LVL }

%union {
    char* str_val;
    long int_val;
}

%start thing

%token <str_val> IDENTIFIER
%token <str_val> STRING
%token <int_val> INT

%type <int_val> logLevel 
%type <str_val> string

%nonassoc BNOT
%left BAND BOR BXOR
%left SHL SHR
%right PWR SQRT
%nonassoc UNEG
%left MULT DIV
%left PLUS MINUS
%left MOD
%nonassoc NOT
%left GT GTE LT LTE
%left EQ NEQ
%right COMMA

%destructor { free($$); } <str_val>

%%

thing:      /* empty */
    |       varDef
    |       include
    |       func
    ;

include:    "include" IDENTIFIER ';'
            {
                char* fileName = $2;
            }
    ;

func:       "func" IDENTIFIER '(' paramListDef ')' ':' IDENTIFIER ';' 
                statement
            "cnuf"
            {
                char* name = $2;
                char* type = $7;
            }
    ;

varDef:     IDENTIFIER ":" string ';'
            {
                char* name = $1;
                char* type = $3;
            }
    ;

nonEmptyParamListDef:
            varDef
    |       nonEmptyParamListDef varDef
    ;

paramListDef:
            /* empty */
    |       nonEmptyParamListDef
    ;

paramList:  /* empty */
    |       IDENTIFIER
    |       paramList "," IDENTIFIER
    ;

nonEmptyElsifStatement:
            "elsif" '(' expression ')'
                statement
    |       nonEmptyElsifStatement
            "elsif" '(' expression ')'
                statement
    ;

ifStatementTail:
            "fi"
    |       "else"
                statement
            "fi"
    |       nonEmptyElsifStatement
            "fi"
    |       nonEmptyElsifStatement
            "else"
                statement
            "fi"
    ;
            
ifStatement:
            "if" "(" expression ")"
                statement
            ifStatementTail
    ;

memberAccess:
            IDENTIFIER
    |       memberAccess "." IDENTIFIER
            {
                //$$ = Find($1, $3);
            }
    ;

varName:    "this"
    |       "other"
    |       "likeme"
    |       memberAccess
    ;

assignationStatement:
            varName "=" expression ';'
    ;

functionCall:
            varName "(" paramList ")" ';'
            {
                //$$ = Call($1, $3);
            }
    ;

returnStatement:
            "return" expression ';'
    ;

yieldStatement:
            "yield" expression ';'
    ;

unary:      BNOT basic_expression
    |       NOT basic_expression
    |       UNEG basic_expression
    ;

binary:     basic_expression PLUS basic_expression
    |       basic_expression MINUS basic_expression
    |       basic_expression MULT basic_expression
    |       basic_expression DIV basic_expression
    |       basic_expression PWR basic_expression
    |       basic_expression SQRT basic_expression
    |       basic_expression BAND basic_expression
    |       basic_expression BOR basic_expression
    |       basic_expression BXOR basic_expression
    |       basic_expression SHL basic_expression
    |       basic_expression SHR basic_expression
    |       basic_expression MOD basic_expression
    |       basic_expression GT basic_expression
    |       basic_expression GTE basic_expression
    |       basic_expression LT basic_expression
    |       basic_expression LTE basic_expression
    |       basic_expression EQ basic_expression
    |       basic_expression NEQ basic_expression
    ;

integer:    INT
    ;

real:       INT '.'
    |       '.' INT
    |       INT '.' INT
    ;

string:     '"' STRING '"' { $$ = $2; }
    |       '\'' STRING '\'' { $$ = $2; }
    |       "@@@\"" STRING "\"@@@" { $$ = $2; }
    ;

basic_expression:
            unary
    |       binary
    |       varName
    |       string
    |       integer
    |       real
    |       functionCall
    ;

expression: basic_expression
    |       expression COMMA basic_expression
    |       '(' expression ')'
    ;

loopStatement:
            "loop" '(' expression ')'
                statement
            "pool"
    ;

logLevel:   "FATAL" { $$ = -2; }
    |       "ERROR" { $$ = -1; }
    |       "WARNING" { $$ = 0; }
    |       "INFO" { $$ = 1; }
    |       "DEBUG" { $$ = 2; }
    |       "TRACE" { $$ = 3; }
    ;

logElement: logLevel string paramList ';'
    |       logElement logLevel string paramList ';'
    ;

logStatement:
            "log" logElement "gol"
    ;

statement:  ifStatement
    |       loopStatement
    |       assignationStatement
    |       returnStatement
    |       yieldStatement
    |       expression
    |       logStatement
    ;

%%

int yylex(YYSTYPE *lvalp);

int yyerror(Entity* THIS, Level* LVL, char* s)
{
    /*
    fprintf(stderr, "ERROR: %s near '", s); 
    for(int i = 0; i < strlen(yytext); ++i) {
        if(isprint(yytext[i]) && yytext[i] != '\n') putc(yytext[i], stderr);
        else fprintf(stderr, "#%X", yytext[i]);
    }   
    fprintf(stderr, "' on line %d col %d\n", yylineno, yycolno);
    //exit(1);*/
}

int main() { return 0; }

