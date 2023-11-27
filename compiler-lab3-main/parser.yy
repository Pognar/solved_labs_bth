%skeleton "lalr1.cc" 
%defines
%define parse.error verbose
%define api.value.type variant
%define api.token.constructor

%code requires{
  #include <string>
  #include "Node.h"
}
%code{
  #define YY_DECL yy::parser::symbol_type yylex()

  YY_DECL;
  
  Node* root;
  
}
// definition of set of tokens. All tokens are of type string //define factors here aswell
%token <std::string> PLUSOP MINUSOP MULTOP INT CLASS IDENTIFIER BOOL
%token END 0 "end of file"
%token <std::string> LEFTM PUBLIC STATIC VOID MAIN LEFTP STRING LEFTB RIGHTB RIGHTP RIGHTM INTT EXTENDS IF ELSE WHILE SOP SEMIC EQOP ANDOP OROP LESSOP MOREOP EQIF MINOP DIVOP RETURN COMMA DOT LENGTH TRUE FALSE THIS NEW EXQL
// definition of the production rules. All production rules are of type Node
%type <Node *>  goal identifier classdeclaration classdeclarationlist mainclass vardeclarationlist methoddeclarationlist statement type statementlist  methoddeclaration vardeclaration parameterlist expression expressionlist stringarr

//expression  parameter 
//classdeclarationlist mainclass vardeclarationlist methoddeclarationlist
%left OROP
%left ANDOP
%left EQIF
%left LESSOP MOREOP
%left PLUSOP MINOP
%left MULTOP DIVOP
%left EXQL
%start goal
%%

goal: mainclass classdeclarationlist END{
	$$ = new Node("Goal", "Root");
	$$->children.push_back($1);
	$$->children.push_back($2);
	root = $$;
};

mainclass: PUBLIC CLASS identifier LEFTM PUBLIC STATIC VOID MAIN LEFTP stringarr RIGHTP LEFTM statement RIGHTM RIGHTM{
    $$ = new Node("MainClass", $3->value);
    $$->children.push_back($10);
    $$->children.push_back($13);
};

stringarr: STRING LEFTB RIGHTB identifier{
	$$ = new Node("String[]", $4->value);
}

statementlist: {
		$$ = new Node("Statementlist","");
	}
	| statement statementlist {
		$$ = $2;
		$$->children.push_front($1);
	}
;

statement: LEFTM statementlist RIGHTM{
		$$ = new Node("Statement", "");
		$$->children.push_back($2);
	};
	| IF LEFTP expression RIGHTP statement{
		$$ = new Node("Statement", "if");
		$$->children.push_back($3);
		$$->children.push_back($5);
	}
	| IF LEFTP expression RIGHTP statement ELSE statement{
		$$ = new Node("Statement", "if");
		$$->children.push_back($3);
		$$->children.push_back($5);
		if ($7) {
			$$->children.push_back($7);
		}
	}
	
	| WHILE LEFTP expression RIGHTP statement {
		$$ = new Node("Statement", "while");
		$$->children.push_back($3);
		$$->children.push_back($5);
	}
	| SOP LEFTP expression RIGHTP SEMIC {
		$$ = new Node("Statement", "sys.out");
		$$->children.push_back($3);
	}
	| identifier EQOP expression SEMIC {
		$$ = new Node("Statement assign", $1->value);
		$$->children.push_back($3);
	}
	| identifier LEFTB expression RIGHTB EQOP expression SEMIC {
		$$ = new Node("Statementlist assign", $1->value); 
		$$->children.push_back($3); $$->children.push_back($6);
	}
;

expressionlist: {$$ = new Node("Empty","");}
	| expression COMMA expressionlist {$$ = new Node ("Expressionlist", "");
		$$->children.push_back($1);
		$$->children.push_back($3);
	}
	| expression {
		$$ = $1;}
;

expression: expression ANDOP expression {
		$$ = new Node("Expression", "and");
		$$->children.push_back($1);
		$$->children.push_back($3);
	}
	| expression OROP expression {$$ = new Node("Expression", "or");
		$$->children.push_back($1);
		$$->children.push_back($3);
	} 
	| expression LESSOP expression {$$ = new Node("Expression", "less than");
		$$->children.push_back($1);
		$$->children.push_back($3);
	} 
	| expression MOREOP expression {$$ = new Node("Expression", "larger");
		$$->children.push_back($1);
		$$->children.push_back($3);
	}
	| expression EQIF expression {$$ = new Node("Expression", "if equal");
		$$->children.push_back($1);
		$$->children.push_back($3);
	} 
	| expression PLUSOP expression {$$ = new Node("Expression", "ADD");
		$$->children.push_back($1);
		$$->children.push_back($3);
	} 
	| expression MINOP expression {$$ = new Node("Expression", "subtract");
		$$->children.push_back($1);
		$$->children.push_back($3);
	} 
	| expression MULTOP expression {$$ = new Node("Expression", "multiplication");
		$$->children.push_back($1);
		$$->children.push_back($3);
	}
	| expression DIVOP expression {$$ = new Node("Expression", "Divided");
		$$->children.push_back($1);
		$$->children.push_back($3);
	}

	| expression LEFTB expression RIGHTB{
		$$ = new Node("Expression[]", "");
		$$->children.push_back($1);
		$$->children.push_back($3);
	}
	| expression DOT LENGTH{
		$$ = new Node("Expression", "len");
		$$->children.push_back($1);
	}
	| expression DOT identifier LEFTP expressionlist RIGHTP{
		$$ = new Node("Expression list", $3->value);
		$$->children.push_back($1);
		$$->children.push_back($5);
	}
	| expression DOT identifier LEFTP RIGHTP{
		$$ = new Node("Expression.identifier", $3->value);
		$$->children.push_back($1);
	}
	| INT{
		$$ = new Node("<INTEGER_LITERAL>", $1);
	}
	| TRUE{
		$$ = new Node("Expression", "TRUE");
	}
	| FALSE{
		$$ = new Node("Expression", "FALSE");
	}
	| identifier{
		$$ = $1;
	}
	| THIS{
		$$ = new Node("Expression", "THIS");
	}
	| NEW INTT LEFTB expression RIGHTB{
		$$ = new Node("Expression", "NEW INT");
		$$->children.push_back($4);
	}
	| NEW identifier LEFTP RIGHTP{
		$$ = new Node("Expression new identifier", $2->value);
	}
	| EXQL expression{
		$$ = new Node("Expression", "NOT");
		$$->children.push_back($2);
	}
	| LEFTP expression RIGHTP{
		$$ = new Node("Expression", "param");
		$$->children.push_back($2);
	}
;

methoddeclaration: 
	PUBLIC type identifier LEFTP parameterlist RIGHTP LEFTM statementlist RETURN expression SEMIC RIGHTM{
		$$ = new Node("MethodDeclaration param", $3->value);
		$$->children.push_back($2);
		$$->children.push_back($5);
		$$->children.push_back($8);
		$$->children.push_back($10);
	}
	| PUBLIC type identifier LEFTP parameterlist RIGHTP LEFTM vardeclarationlist statementlist RETURN expression SEMIC RIGHTM{
		$$ = new Node("MethodDeclaration param", $3->value); 
		$$->children.push_back($2);
		$$->children.push_back($5);
		$$->children.push_back($8);
		$$->children.push_back($9);
		$$->children.push_back($11);
	}
	| PUBLIC type identifier LEFTP  RIGHTP LEFTM vardeclarationlist statementlist RETURN expression SEMIC RIGHTM{
		$$ = new Node("MethodDeclaration", $3->value); 
		$$->children.push_back($2);
		$$->children.push_back($7);
		$$->children.push_back($8);
		$$->children.push_back($10);
	}
	| PUBLIC type identifier LEFTP RIGHTP LEFTM statementlist RETURN expression SEMIC RIGHTM{
		$$ = new Node("MethodDeclaration", $3->value);
		$$->children.push_back($2);
		$$->children.push_back($7);
		$$->children.push_back($9); 
	}
;

classdeclaration: 
	CLASS identifier EXTENDS identifier LEFTM vardeclarationlist methoddeclarationlist RIGHTM{
		$$ = new Node("ClassDeclaration", $2->value);
		$$->children.push_back($4);
		$$->children.push_back($6);
		$$->children.push_back($7);
	}
	| CLASS identifier EXTENDS identifier LEFTM  methoddeclarationlist RIGHTM{
		$$ = new Node("ClassDeclaration", $2->value);
		$$->children.push_back($4);
		$$->children.push_back($6);
	}
	| CLASS identifier LEFTM vardeclarationlist methoddeclarationlist RIGHTM{
		$$ = new Node("ClassDeclaration", $2->value);
		$$->children.push_back($4);
		$$->children.push_back($5);
	}
	| CLASS identifier LEFTM  methoddeclarationlist RIGHTM{
		$$ = new Node("ClassDeclaration", $2->value);
		$$->children.push_back($4);
	}
;

classdeclarationlist: {
		$$ = new Node("Classdeclarationlist","");
	}
	| classdeclarationlist classdeclaration {$$ = $1;
		$$->children.push_back($2);
	}
;
vardeclarationlist: {$$ = new Node("Vardeclarationlist","");}
	| vardeclarationlist vardeclaration {$$ = $1;
		$$->children.push_back($2);
	}
;

vardeclaration: type identifier SEMIC {
		$$ = new Node("VarDeclaration", $2->value);
		$$->children.push_back($1);
	}
;


//parameter: 

parameterlist: type identifier {
		$$ = new Node("parameter list", $2->value);
		$$->children.push_back($1);
	}
	| type identifier COMMA parameterlist{
		$$ = new Node("parameterlist", $2->value);
		$$->children.push_back($1);
		$$->children.push_back($4);
	}
;

methoddeclarationlist: {
		$$ = new Node("MethodDeclarationlist","");
	}
	| methoddeclarationlist methoddeclaration {
		$$ = $1;
		$$->children.push_back($2);
	}
;



identifier: IDENTIFIER {
		$$ = new Node("identifier", $1);
	};

type: INTT LEFTB RIGHTB {
		$$ = new Node("Int[]", "");
	}
	| BOOL{
		$$ = new Node("boolean", "boolean");
	}
	| INTT{
		$$ = new Node("<INTEGER_LITERAL>", "");
	}
	| identifier{$$ = $1;}
;
