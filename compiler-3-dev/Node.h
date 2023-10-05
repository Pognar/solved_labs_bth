#ifndef NODE_H
#define NODE_H

#include <list>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include "symbol_t.h"
#include "class.h"

using namespace std;

class Node
{
public:
	int id;
	string type, value;
	list<Node *> children;
	Node(string t, string v) : type(t), value(v) {}
	Node()
	{
		type = "uninitialised";
		value = "uninitialised";
	} // Bison needs this.

	//------------------------------------------------------
	//----------------------GENERATE TREE-------------------
	//------------------------------------------------------
	void print_tree(int depth = 0)
	{
		for (int i = 0; i < depth; i++)
			cout << "  ";
		cout << type << ":" << value << endl;
		for (auto i = children.begin(); i != children.end(); i++)
			(*i)->print_tree(depth + 1);
	}

	void generate_tree()
	{
		std::ofstream outStream;
		outStream.open("tree.dot");

		int count = 0;
		outStream << "digraph {" << std::endl;
		generate_tree_content(count, &outStream);
		outStream << "}" << std::endl;
		outStream.close();

		std::cout << "\nBuilt a parse-tree:" << std::endl;
	}

	void generate_tree_content(int &count, ofstream *outStream)
	{
		id = count++;
		*outStream << "n" << id << " [label=\"" << type << ":" << value << "\"];" << endl;

		for (auto i = children.begin(); i != children.end(); i++)
		{
			(*i)->generate_tree_content(count, outStream);
			*outStream << "n" << id << " -> n" << (*i)->id << endl;
		}
	}

	//------------------------------------------------------
	//----------------------SYMBOL TABLE--------------------
	//------------------------------------------------------
	void create_st(SymbolTable *st)
	{
		cout << "Creating Symboltable" << endl;

		findIdentifier(this, st);
		//st->enterScope();
		execute(this->children, st);
		//st->exitScope();
		st->resetTable();
		st->printTable();
	}

	void execute(auto node, SymbolTable *st)
	{
		for (auto i = node.begin(); i != node.end(); i++)
		{

			findIdentifier((*i), st);
			if ((*i)->type == "MainClass" ||
				(*i)->type == "ClassDeclaration" ||
				(*i)->type == "MethodDeclaration param" ||
				(*i)->type == "MethodDeclaration")
			{
				st->enterScope();
			}
			if ((*i)->children.size() > 0)
			{
				execute((*i)->children, st);
			}
			if ((*i)->type == "MainClass" ||
				(*i)->type == "ClassDeclaration" ||
				(*i)->type == "MethodDeclaration param" ||
				(*i)->type == "MethodDeclaration")
			{
				st->exitScope();
			}
		}
	}

	void getParam(auto n, Method *m)
	{
		auto t = (*n)->children.begin();

		Variable *var = new Variable((*n)->value, (*t)->type);

		m->addParameter((*n)->value, var);
		//st->put((*n)->value, var);

		if ((*n)->children.size() > 1)
		{
			getParam((*n)->children.rbegin(), m);
		}
	}

	Record *findIdentifier(auto *n, SymbolTable *st)
	{
		//if (n->type == "Goal")
		//{
		//	st->put(n->value, new Class(n->value, n->type));
		//}
		if (n->type == "MainClass")
		{
			st->put(n->value, new Class(n->value, n->type));
		}
		//------------------------------------------------------
		else if (n->type == "MethodDeclaration")
		{
			auto c = n->children.begin();
			st->put(n->value, new Method(n->value, (*c)->type));
		}
		else if (n->type == "MethodDeclaration param")
		{
			auto l_front = n->children.begin();
			Method *m = new Method(n->value, (*l_front)->type);
			advance(l_front, 1);
			auto t = (*l_front)->children.begin();
			Variable *var = new Variable((*l_front)->value, (*t)->type);
			//st->put((*l_front)->value, var);
			m->addParameter(to_string(1), var);

			if ((*l_front)->children.size() > 1)
			{
				getParam((*l_front)->children.rbegin(), m);
			}
			st->put(n->value, m);
		}
		else if (n->type == "Expression list") {
			auto l_front = n->children.begin();
			Method *m = new Method(n->value, (*l_front)->type);
		}
		//------------------------------------------------------
		else if (n->type == "ClassDeclaration")
		{
			st->put(n->value, new Class(n->value, n->type));
		}
		//------------------------------------------------------
		else if (n->type == "VarDeclaration")
		{
			cout << "VarDec "
				 << ": " << n->value << endl;
			auto c = n->children.begin();
			if ((*c)->type == "Int[]")
			{
				st->put(n->value, new Variable(n->value, (*c)->type));
			}
			else if ((*c)->type == "<INTEGER_LITERAL>")
			{
				st->put(n->value, new Variable(n->value, (*c)->type));
			}
			else if ((*c)->type == "boolean")
			{
				st->put(n->value, new Variable(n->value, (*c)->type));
			}
		}
		else if (n->type == "parameterlist" || n->type == "parameter list") {
			auto c = n->children.begin();
			st->put(n->value, new Variable(n->value, (*c)->type));
		}
	}

	//------------------------------------------------------
	//--------------------SEMANTIC ANALYSIS-----------------
	//------------------------------------------------------
	std::string look_in_sym(std::string name, SymbolTable *st) {
		Record* r = st->lookup(name);
		if(r == nullptr){
			std::cout << "Cannot find: " << name << " in symboltable" << std::endl;
			return "fault";
		}
		else {
			string statement_type = r->getType();
			return statement_type;
		}
	}

		std::vector<string> look_param(std::string name, SymbolTable *st) {
		Record* r = st->lookup(name);
		if(r == nullptr){
			std::cout << "Cannot find: " << name << " in symboltable" << std::endl;
			std::vector<string> ret = {"fault"};
			return ret;
		}
		else {
			std::vector<string> para = r->getParameter();
			return para;
		}
	}

	std::string check_expression_list(auto *node, SymbolTable *st) //expression list : 
	
	//int i = this.Init(sz)
	//expression list : Init
	//1 expression: this
	//2 indentifer: sz
	{
		//LOOK AT RETURN VALUE OF node->value;
		auto h = node->children.begin();
		if ((*h)->type == "Expression new identifier") {
			return "new";
		}
		//string type_method = look_in_sym(node->value, st);
		if ((*h)->type == "Expression") {
			if ((*h)->value != "THIS") {
				return "Wrong this";
			}
			else {
				string type;
				advance(h, 1);
				if ((*h)->type == "identifier") {
					type = look_in_sym((*h)->value, st);
					return type;
				}
				else if ((*h)->type == "Expression") {
					type = check_expression((*h),st);
				}
				else if ((*h)->type == "<INTEGER_LITERAL>") {
					type = "<INTEGER_LITERAL>";
				}
				else if ((*h)->type == "Expressionlist") {
					std::vector<string> para = look_param(node->value, st);
					int counter = 0;
					for (auto i = (*h)->children.begin(); i != (*h)->children.end(); i++)
					{
						string type = (*i)->type;
						if ((*i)->type == "identifier") {
							type = look_in_sym((*i)->value, st);
						}
						else if ((*i)->type == "Expression") {
							type = check_expression((*h),st);
						}
						if (para[counter] == type) {
							return para[counter];
						}
						counter++;
					}
				}
				else {
					cout << "No such expression" << endl;
					return "Wrong this";
				}
			}
		}
	}

	std::string check_expression(auto *node, SymbolTable *st)
	{
		//cout << node->type << ":" << node->value << endl;
		if (node->value == "or")
		{
			string type_one = node->children.front()->type;
			string type_two = node->children.back()->type;

			if (type_one == "identifier") {
				type_one = look_in_sym(node->children.front()->value, st);
			}
			if (type_two == "identifier") {
				type_two = look_in_sym(node->children.back()->value, st);
			}
			if (type_one == "Expression") {
				type_one = check_expression(node->children.front(), st);
			}		
			if (type_two == "Expression") {
				type_two = check_expression(node->children.back(), st);
			}
			if (type_one != "<INTEGER_LITERAL>" || type_two != "<INTEGER_LITERAL>")
			{
				std::cout << node->value << " Wrong types" << std::endl;
				std::cout << "Check expressions match: " << type_one << " : " << type_two << endl;
			}
			return type_one;
		} 
		else if (node->type == "Expression[]"){
			string ident = look_in_sym(node->children.front()->value, st);
			string number = node->children.back()->type;
			if (number == "identifier") {
				number = look_in_sym(node->children.back()->value, st);
			}
			if (number != "<INTEGER_LITERAL>" || ident != "Int[]") {
				return "fault";
			}
			else{
				return "<INTEGER_LITERAL>";
			}
		}
		else if (node->value == "less than" || 
			node->value == "ADD" || 
			node->value == "subtract" || 
			node->value == "multiplication" || 
			node->value == "Divided" || 
			node->value == "larger"
			) {
			string type_one = node->children.front()->type;
			string type_two = node->children.back()->type;
			if (type_one == "identifier") {
				type_one = look_in_sym(node->children.front()->value, st);
			}
			if (type_two == "identifier") {
				type_two = look_in_sym(node->children.back()->value, st);
			}
			if (type_one == "Expression") {
				type_one = check_expression(node->children.front(), st);
			}		
			if (type_two == "Expression") {
				type_two = check_expression(node->children.back(), st);
			}

			if (type_one != "<INTEGER_LITERAL>" || type_two != "<INTEGER_LITERAL>")
			{
				std::cout << node->value << " Wrong types: " << type_one << " : " << type_two << endl;
			}
			return type_one;
		}
		else if (node->value == "NEW INT")
		{
			string type_one = node->children.front()->type;
			string type;
			if (type_one == "identifier") {
				type = look_in_sym(node->children.front()->value, st);
			}
			else if (type_one == "Expression") {
				type = check_expression(node->children.front(), st);
			}
			else if (type_one == "<INTEGER_LITERAL>") {
				type = "<INTEGER_LITERAL>";
			}

			if (type == "<INTEGER_LITERAL>") {
				return "Int[]";
			}
			else {
				return "wrong";
			}
		} 
		else if (node->value == "and")
		{
			string type_one = node->children.front()->type;
			string type_two = node->children.back()->type;

			if (type_one == "identifier") {
				type_one = look_in_sym(node->children.front()->value, st);
			}
			if (type_two == "identifier") {
				type_two = look_in_sym(node->children.back()->value, st);
			}
			if (type_one == "Expression") {
				type_one = check_expression(node->children.front(), st);
			}		
			if (type_two == "Expression") {
				type_two = check_expression(node->children.back(), st);
			}
			if (type_one != "<INTEGER_LITERAL>" || type_two != "<INTEGER_LITERAL>")
			{
				std::cout << node->value << " has wrong types" << std::endl;
				std::cout << type_one << " : " << type_two << endl;
			}
			return type_one;
		} 
		else if (node->value == "TRUE" || node->value == "FALSE")
		{
			return "boolean";
		}
		else if (node->value == "NOT")
		{
			std::string s = check_expression(node->children.front(), st);
			return s;
		}
		else if (node->value == "param")
		{
			std::string s = look_in_sym(node->children.front()->value, st);
			if (s != "fault") { 
				std::vector<string> p = look_param(node->children.front()->value, st);
				std::string g = check_expression_list(node->children.front(), st);

				if (p[0] == g) { 
					return g;
				}
				return "Worng parameter type";
			}
		}
		else if (node->value == "len") {
			std::string s = look_in_sym(node->children.front()->value, st);
			if (s == "Int[]") {
				return "<INTEGER_LITERAL>";
			}
		}
		return "NULL";
	}

	void find_identifier(auto *node, SymbolTable *st)
	{
		if (node->type == "Statement")
		{
			//std::cin.get();
			if (node->value == "if" || node->value == "while") {
				for (auto i = node->children.begin(); i != node->children.end(); i++)
				{
					if ((*i)->type == "Expression") { 
						check_expression(node, st);
					}
					else if ((*i)->type == "Statement" ||
					(*i)->type == "Statement assign")
					{
						find_identifier((*i), st);
					}
				}
			}
			if (node->value == "sys.out") {
				auto t = node->children.begin();
				if ((*t)->type == "Expression" || (*t)->type == "Expression[]" ) {
					std::string type_two = check_expression((*t), st);
					if(type_two == "NULL" || type_two != "<INTEGER_LITERAL>"){
						std::cout << "Sys.out has wrong Expression or not of type int: " << type_two << std::endl;
					}
				}
				else if ((*t)->type == "Expression list") //också klass
				{
					string assign_type = check_expression_list((*t), st);
					if (assign_type == "Wrong this") {
						cout << "Expression list dont have correct types" << endl;
					}
					else if (assign_type != "new" && assign_type != "<INTEGER_LITERAL>") { 
						cout << "Expression list dont have correct types" << endl;
					}
				}
				else {
					if ((*t)->type != "<INTEGER_LITERAL>") { 
						//cout << "Expression list dont have correct types" << endl;
						cout << "sys.out no int" << endl;
					}
				}				
			}
		}
		if (node->type == "Statement assign")
		{
			std::string assign_type = "";
			std::string statement_type = "";
			statement_type = look_in_sym(node->value, st);			
			
			auto i = node->children.begin();
			if ((*i)->type == "identifier") {
				assign_type = look_in_sym((*i)->value, st);
			}
			else if ((*i)->type == "Expression") {
				assign_type = check_expression((*i), st);
			}
			else if ((*i)->type == "Expression list") {
				assign_type = check_expression_list((*i), st);
				if (assign_type == "Wrong this") {
					assign_type = "Expression list dont have correct types";
				}
				if (assign_type == "new") { assign_type = statement_type; }
			}
			if ((*i)->type == "Expression.identifier") {
				auto h = (*i)->children.begin();
				if ((*h)->value != "THIS") {
					cout << "identifier not THIS" << endl;
				}
				assign_type = statement_type;
			}
			else if ((*i)->type == "Expression[]") {
				assign_type = check_expression((*i), st);
			}
			else if ((*i)->type == "<INTEGER_LITERAL>") {
				assign_type = "<INTEGER_LITERAL>";
			}
			if (statement_type != assign_type) {
				cout << "failed to assign: " << node->value << endl;
			}

		}
		if (node->type == "Statementlist assign") {
			bool true_1, true_2;
			true_1 = true;
			true_2 = true;
			if (node->children.front()->type != "<INTEGER_LITERAL>"){
				true_1 = false;
			}
			if(true_1 == false){
				if (look_in_sym(node->children.front()->value, st) != "<INTEGER_LITERAL>" && true_1 != true) {
					true_2 = false;
				}
			} 
			if(true_1 == true || true_2 == true){
				string assign_type;
				string type = node->children.back()->type;
				if (type == "identifier") {
					assign_type = look_in_sym(node->children.back()->value, st);
				}
				else if (type == "Expression") {
					assign_type = check_expression(node->children.back(), st);
				}
				else if (type == "Expression list") {
					assign_type = check_expression_list(node->children.back(), st);
					if (assign_type == "Wrong this") {
						assign_type = "Expression list dont have correct types";
					}
					if (assign_type == "new") { assign_type = type; }
				}
				else if (type == "Expression[]") {
					assign_type = check_expression(node->children.back(), st);
				}
				else if (type == "<INTEGER_LITERAL>") {
					assign_type = "<INTEGER_LITERAL>";
				}	
			}else{
				if(true_1 == false){
					cout << "Invalid index type " << node->children.front()->type << endl;
				}
			}
		} 
	}

	void semantic_traverse(auto node, SymbolTable *st)
	{
		for (auto i = node.begin(); i != node.end(); i++)
		{
			find_identifier((*i), st);
			if ((*i)->type == "MainClass" ||
				(*i)->type == "ClassDeclaration" ||
				(*i)->type == "MethodDeclaration param" ||
				(*i)->type == "MethodDeclaration")
			{
				st->enterScope();
			}
			if ((*i)->children.size() > 0)
			{
				semantic_traverse((*i)->children, st);
			}
			if ((*i)->type == "MainClass" ||
				(*i)->type == "ClassDeclaration" ||
				(*i)->type == "MethodDeclaration param" ||
				(*i)->type == "MethodDeclaration")
			{
				st->exitScope();
			}
		}
	}
	
	void semantic_analysis(SymbolTable *st)
	{
		std::cout << "Starting semantic analysis:" << std::endl;
		std::cout << "---------------------------" << std::endl;
		st->resetTable();
		//st->enterScope();
		semantic_traverse(this->children, st);
		//st->exitScope();
		st->resetTable();
	}
};
#endif