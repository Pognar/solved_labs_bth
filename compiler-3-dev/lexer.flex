%top{
#include "parser.tab.hh"
#define YY_DECL yy::parser::symbol_type yylex()
#include "Node.h"
}
%option noyywrap nounput batch noinput stack
%%

"//"[^\n]*              {printf(yytext);}
"class"                 {return yy::parser::make_CLASS(yytext);}
"public"                {return yy::parser::make_PUBLIC(yytext);}
"static"                {return yy::parser::make_STATIC(yytext);}
"void"                  {return yy::parser::make_VOID(yytext);}
"main"                  {return yy::parser::make_MAIN(yytext);}
"String"                {return yy::parser::make_STRING(yytext);}
"System.out.println"    {return yy::parser::make_SOP(yytext);}
"true"                  {return yy::parser::make_TRUE(yytext);}
"false"                 {return yy::parser::make_FALSE(yytext);}
"this"                  {return yy::parser::make_THIS(yytext);}
"new"                   {return yy::parser::make_NEW(yytext);}
"length"                {return yy::parser::make_LENGTH(yytext);}
"int"                   {return yy::parser::make_INTT(yytext);}
"return"                {return yy::parser::make_RETURN(yytext);}

"boolean"               {return yy::parser::make_BOOL(yytext);}
"if"                    {return yy::parser::make_IF(yytext);}
"else"                  {return yy::parser::make_ELSE(yytext);}
"while"                 {return yy::parser::make_WHILE(yytext);}
"extends"               {return yy::parser::make_EXTENDS(yytext);}
[a-zA-Z][a-zA-Z0-9_]*    {return yy::parser::make_IDENTIFIER(yytext);}
0|[1-9][0-9]*           {return yy::parser::make_INT(yytext);}
"("                     {return yy::parser::make_LEFTP(yytext);}
"["                     {return yy::parser::make_LEFTB(yytext);}
"]"                     {return yy::parser::make_RIGHTB(yytext);}
")"                     {return yy::parser::make_RIGHTP(yytext);}
"{"                     {return yy::parser::make_LEFTM(yytext);}
"}"                     {return yy::parser::make_RIGHTM(yytext);}

"+"                     {return yy::parser::make_PLUSOP(yytext);}
"*"                     {return yy::parser::make_MULTOP(yytext);}

";"                     {return yy::parser::make_SEMIC(yytext);}
"="                     {return yy::parser::make_EQOP(yytext);}
"&&"                    {return yy::parser::make_ANDOP(yytext);}
"||"                    {return yy::parser::make_OROP(yytext);}
"<"                     {return yy::parser::make_LESSOP(yytext);}
">"                     {return yy::parser::make_MOREOP(yytext);}
"=="                    {return yy::parser::make_EQIF(yytext);}
","                     {return yy::parser::make_COMMA(yytext);}
"."                     {return yy::parser::make_DOT(yytext);}
"-"                     {return yy::parser::make_MINOP(yytext);}
"/"                     {return yy::parser::make_DIVOP(yytext);}

"!"                     {return yy::parser::make_EXQL(yytext);}






[ \t\n]+                  {}

<<EOF>>                 return yy::parser::make_END();
%%