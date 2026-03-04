%{
//**************************************
// lang.y
//
// Parser definition file. bison uses this file to generate the parser.
//

#include <iostream>
#include "lex.h"
#include "astnodes.h"
#include "cOwnedNodes.h"
#include "cSymbolTable.h"

static bool g_semanticErrorHappened = false;

#define CHECK_ERROR() { if (g_semanticErrorHappened) \
    { g_semanticErrorHappened = false; } }
#define PROP_ERROR() { if (g_semanticErrorHappened) \
    { g_semanticErrorHappened = false; YYERROR; } }

static std::string StripQuotes(const std::string &text)
{
    if (text.size() >= 2)
    {
        return text.substr(1, text.size() - 2);
    }

    return text;
}

%}

%locations

%union{
    int             int_val;
    double          float_val;
    std::string*    str_val;
    cAstNode*       ast_node;
    cProgramNode*   program_node;
    cBlockNode*     block_node;
    cDeclsNode*     decls_node;
    cDeclNode*      decl_node;
    cStmtsNode*     stmts_node;
    cStmtNode*      stmt_node;
    cExprNode*      expr_node;
    cVarExprNode*   var_expr_node;
    cParamsNode*    params_node;
    cParamListNode* param_list_node;
    cSymbol*        symbol;
}

%{
    int yyerror(const char *msg);

    cAstNode *yyast_root;
%}

%start program

%token <symbol>    IDENTIFIER
%token <symbol>    TYPE_ID
%token <int_val>   CHAR_VAL
%token <int_val>   INT_VAL
%token <float_val> FLOAT_VAL
%token <int_val>   LE
%token <int_val>   GE
%token <int_val>   AND
%token <int_val>   OR
%token <int_val>   EQUALS
%token <int_val>   NOT_EQUALS
%token <str_val>   STRING_LIT

%token PROGRAM
%token PRINT PRINTS
%token WHILE IF ELSE ENDIF
%token STRUCT ARRAY
%token RETURN
%token JUNK_TOKEN

%type <program_node> program
%type <block_node> block
%type <ast_node> open
%type <ast_node> close
%type <ast_node> decl_marker
%type <ast_node> func_scope
%type <ast_node> func_close
%type <decls_node> decls
%type <decls_node> decls_opt
%type <decl_node> decl
%type <decl_node> var_decl
%type <decl_node> struct_decl
%type <decl_node> array_decl
%type <decl_node> func_decl
%type <params_node> paramsspec
%type <decl_node> paramspec
%type <stmts_node> stmts
%type <stmts_node> stmts_opt
%type <stmt_node> stmt
%type <var_expr_node> lval
%type <param_list_node> params
%type <expr_node> param
%type <expr_node> expr
%type <expr_node> or_expr
%type <expr_node> and_expr
%type <expr_node> eq_expr
%type <expr_node> rel_expr
%type <expr_node> addit
%type <expr_node> term
%type <expr_node> fact
%type <expr_node> func_call
%type <var_expr_node> varref
%type <symbol> varpart
%type <symbol> decl_name

%%

program: PROGRAM block
        {
            $$ = cOwnedNodes::Make<cProgramNode>($2);
            yyast_root = $$;
            if (yynerrs == 0)
                YYACCEPT;
            else
                YYABORT;
        }
        ;

block:   open decls_opt stmts_opt close
        { $$ = cOwnedNodes::Make<cBlockNode>($2, $3); }
        ;

open:   '{'
        {
            $$ = nullptr;
            g_symbolTable.IncreaseScope();
        }
        ;

close:  '}'
        {
            $$ = nullptr;
            g_symbolTable.DecreaseScope();
        }
        ;

decl_marker: %empty
        {
            $$ = nullptr;
            g_insertSymbol = true;
        }
        ;

decl_name: decl_marker IDENTIFIER
        {
            $$ = $2;
            g_insertSymbol = false;
        }
        ;

func_scope: %empty
        {
            $$ = nullptr;
            g_symbolTable.IncreaseScope();
        }
        ;

func_close: %empty
        {
            $$ = nullptr;
            g_symbolTable.DecreaseScope();
        }
        ;

decls_opt:
        %empty
        { $$ = nullptr; }
    |   decls
        { $$ = $1; }
        ;

decls:  decls decl
        {
            $$ = $1;
            $$->Insert($2);
        }
    |   decl
        { $$ = cOwnedNodes::Make<cDeclsNode>($1); }
        ;

decl:   var_decl ';'
        { $$ = $1; }
    |   array_decl ';'
        { $$ = $1; }
    |   struct_decl ';'
        { $$ = $1; }
    |   func_decl
        { $$ = $1; }
        ;

var_decl: TYPE_ID decl_name
        { $$ = cOwnedNodes::Make<cVarDeclNode>($1, $2); CHECK_ERROR(); }
        ;

struct_decl: STRUCT open decls close decl_name
        {
            $$ = cOwnedNodes::Make<cStructDeclNode>($3, $5);
            CHECK_ERROR();
        }
        ;

array_decl: ARRAY TYPE_ID '[' INT_VAL ']' decl_name
        {
            $$ = cOwnedNodes::Make<cArrayDeclNode>($2, $6, $4);
            CHECK_ERROR();
        }
        ;

func_decl:
        TYPE_ID decl_name '(' func_scope ')' func_close ';'
        { $$ = cOwnedNodes::Make<cFuncDeclNode>($1, $2); CHECK_ERROR(); }
    |   TYPE_ID decl_name '(' func_scope paramsspec ')' func_close ';'
        { $$ = cOwnedNodes::Make<cFuncDeclNode>($1, $2, $5); CHECK_ERROR(); }
    |   TYPE_ID decl_name '(' func_scope ')' '{' decls stmts '}' func_close
        { $$ = cOwnedNodes::Make<cFuncDeclNode>($1, $2, nullptr, $7, $8); CHECK_ERROR(); }
    |   TYPE_ID decl_name '(' func_scope ')' '{' stmts '}' func_close
        { $$ = cOwnedNodes::Make<cFuncDeclNode>($1, $2, nullptr, nullptr, $7); CHECK_ERROR(); }
    |   TYPE_ID decl_name '(' func_scope paramsspec ')' '{' decls stmts '}' func_close
        { $$ = cOwnedNodes::Make<cFuncDeclNode>($1, $2, $5, $8, $9); CHECK_ERROR(); }
    |   TYPE_ID decl_name '(' func_scope paramsspec ')' '{' stmts '}' func_close
        { $$ = cOwnedNodes::Make<cFuncDeclNode>($1, $2, $5, nullptr, $8); CHECK_ERROR(); }
        ;

paramsspec: paramsspec ',' paramspec
        {
            $$ = $1;
            $$->Insert($3);
        }
    |   paramspec
        { $$ = cOwnedNodes::Make<cParamsNode>($1); }
        ;

paramspec: var_decl
        { $$ = $1; }
        ;

stmts_opt:
        %empty
        { $$ = nullptr; }
    |   stmts
        { $$ = $1; }
        ;

stmts:  stmts stmt
        {
            $$ = $1;
            $$->Insert($2);
        }
    |   stmt
        { $$ = cOwnedNodes::Make<cStmtsNode>($1); }
        ;

stmt:   IF '(' expr ')' stmts ENDIF ';'
        { $$ = cOwnedNodes::Make<cIfNode>($3, $5); }
    |   IF '(' expr ')' stmts ELSE stmts ENDIF ';'
        { $$ = cOwnedNodes::Make<cIfNode>($3, $5, $7); }
    |   WHILE '(' expr ')' stmt
        { $$ = cOwnedNodes::Make<cWhileNode>($3, $5); }
    |   PRINT '(' expr ')' ';'
        { $$ = cOwnedNodes::Make<cPrintNode>($3); }
    |   PRINTS '(' STRING_LIT ')' ';'
        {
            $$ = cOwnedNodes::Make<cPrintsNode>(StripQuotes(*$3));
            delete $3;
        }
    |   lval '=' expr ';'
        { $$ = cOwnedNodes::Make<cAssignNode>($1, $3); }
    |   func_call ';'
        { $$ = $1; }
    |   block
        { $$ = $1; }
    |   RETURN expr ';'
        { $$ = cOwnedNodes::Make<cReturnNode>($2); }
    |   error ';'
        { $$ = nullptr; }
        ;

func_call: IDENTIFIER '(' params ')'
        { $$ = cOwnedNodes::Make<cFuncExprNode>($1, $3); CHECK_ERROR(); }
    |   IDENTIFIER '(' ')'
        { $$ = cOwnedNodes::Make<cFuncExprNode>($1); CHECK_ERROR(); }
        ;

varref: varref '.' varpart
        {
            $1->AddPart($3);
            $$ = $1;
        }
    |   varref '[' expr ']'
        {
            $1->AddPart($3);
            $$ = $1;
        }
    |   varpart
        { $$ = cOwnedNodes::Make<cVarExprNode>($1); CHECK_ERROR(); }
        ;

varpart: IDENTIFIER
        { $$ = $1; }
        ;

lval:   varref
        { $$ = $1; }
        ;

params: params ',' param
        {
            $$ = $1;
            $$->Insert($3);
        }
    |   param
        { $$ = cOwnedNodes::Make<cParamListNode>($1); }
        ;

param:  expr
        { $$ = $1; }
        ;

expr:   or_expr
        { $$ = $1; }
        ;

or_expr: or_expr OR and_expr
        { $$ = cOwnedNodes::Make<cBinaryExprNode>($1, cOwnedNodes::Make<cOpNode>($2), $3); }
    |   and_expr
        { $$ = $1; }
        ;

and_expr: and_expr AND eq_expr
        { $$ = cOwnedNodes::Make<cBinaryExprNode>($1, cOwnedNodes::Make<cOpNode>($2), $3); }
    |   eq_expr
        { $$ = $1; }
        ;

eq_expr: eq_expr EQUALS rel_expr
        { $$ = cOwnedNodes::Make<cBinaryExprNode>($1, cOwnedNodes::Make<cOpNode>($2), $3); }
    |   eq_expr NOT_EQUALS rel_expr
        { $$ = cOwnedNodes::Make<cBinaryExprNode>($1, cOwnedNodes::Make<cOpNode>($2), $3); }
    |   rel_expr
        { $$ = $1; }
        ;

rel_expr: rel_expr '<' addit
        { $$ = cOwnedNodes::Make<cBinaryExprNode>($1, cOwnedNodes::Make<cOpNode>('<'), $3); }
    |   rel_expr '>' addit
        { $$ = cOwnedNodes::Make<cBinaryExprNode>($1, cOwnedNodes::Make<cOpNode>('>'), $3); }
    |   rel_expr LE addit
        { $$ = cOwnedNodes::Make<cBinaryExprNode>($1, cOwnedNodes::Make<cOpNode>($2), $3); }
    |   rel_expr GE addit
        { $$ = cOwnedNodes::Make<cBinaryExprNode>($1, cOwnedNodes::Make<cOpNode>($2), $3); }
    |   addit
        { $$ = $1; }
        ;

addit:  addit '+' term
        { $$ = cOwnedNodes::Make<cBinaryExprNode>($1, cOwnedNodes::Make<cOpNode>('+'), $3); }
    |   addit '-' term
        { $$ = cOwnedNodes::Make<cBinaryExprNode>($1, cOwnedNodes::Make<cOpNode>('-'), $3); }
    |   term
        { $$ = $1; }
        ;

term:   term '*' fact
        { $$ = cOwnedNodes::Make<cBinaryExprNode>($1, cOwnedNodes::Make<cOpNode>('*'), $3); }
    |   term '/' fact
        { $$ = cOwnedNodes::Make<cBinaryExprNode>($1, cOwnedNodes::Make<cOpNode>('/'), $3); }
    |   term '%' fact
        { $$ = cOwnedNodes::Make<cBinaryExprNode>($1, cOwnedNodes::Make<cOpNode>('%'), $3); }
    |   fact
        { $$ = $1; }
        ;

fact:   '(' expr ')'
        { $$ = $2; }
    |   '-' fact
        { $$ = cOwnedNodes::Make<cBinaryExprNode>(cOwnedNodes::Make<cIntExprNode>(0), cOwnedNodes::Make<cOpNode>('-'), $2); }
    |   INT_VAL
        { $$ = cOwnedNodes::Make<cIntExprNode>($1); }
    |   FLOAT_VAL
        { $$ = cOwnedNodes::Make<cFloatExprNode>($1); }
    |   varref
        { $$ = $1; }
    |   func_call
        { $$ = $1; }
        ;

%%

int yyerror(const char *msg)
{
    std::cerr << "ERROR: " << msg << " at symbol "
        << yytext << " on line " << yylineno << "\n";

    return 0;
}

void SemanticParseError(std::string error)
{
    std::cout << "ERROR: " << error << " near line "
              << yylineno << "\n";
    g_semanticErrorHappened = true;
    yynerrs++;
}

void SemanticParseError(std::string error, int line)
{
    std::cout << "ERROR: " << error << " near line "
              << line << "\n";
    g_semanticErrorHappened = true;
    yynerrs++;
}
