#ifndef NODE_H
#define NODE_H

#include "class.h"
#include "symbol_t.h"
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <vector>

using namespace std;

class Node
{
public:
	int id;
	string ret_type, ret_name;
	string type, value;
	bool visited;
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
	template <typename T>
	void print_anything(T input)
	{
		std::cout << input << std::endl;
	}
	void print_tree(int depth = 0)
	{
		for (int i = 0; i < depth; i++)
			cout << "  ";
		print_anything(Node::value);
		print_anything(Node::type);
		cout << type << ":" << value << endl;
		for (list<Node *>::iterator i = children.begin(); i != children.end(); i++)
			(*i)->print_tree(depth + 1);
	}

	void generate_tree()
	{
		ofstream outStream;
		outStream.open("tree.dot");

		int count = 0;
		outStream << "digraph {" << endl;
		generate_tree_content(count, &outStream);
		outStream << "}" << endl;
		outStream.close();

		cout << "\nBuilt a parse-tree:" << endl;
	}

	void generate_tree_content(int &count, ofstream *outStream)
	{
		id = count++;
		*outStream << "n" << id << " [label=\"" << type << ":" << value << "\"];" << endl;

		for (list<Node *>::iterator i = children.begin(); i != children.end(); i++)
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
		// st->enterScope();
		execute(this->children, st);
		// st->exitScope();
		st->resetTable();
		// st->printTable();
	}

	void execute(list<Node *> node, SymbolTable *st)
	{
		for (list<Node *>::iterator i = node.begin(); i != node.end(); i++)
		{

			findIdentifier((*i), st);
			if ((*i)->type == "MainClass" || (*i)->type == "ClassDeclaration" ||
				(*i)->type == "MethodDeclaration param" ||
				(*i)->type == "MethodDeclaration")
			{
				st->enterScope();
			}
			if ((*i)->children.size() > 0)
			{
				execute((*i)->children, st);
			}
			if ((*i)->type == "MainClass" || (*i)->type == "ClassDeclaration" ||
				(*i)->type == "MethodDeclaration param" ||
				(*i)->type == "MethodDeclaration")
			{
				st->exitScope();
			}
		}
	}

	void getParam(list<Node *>::reverse_iterator n, Method *m)
	{
		list<Node *>::iterator t = (*n)->children.begin();

		Variable *var = new Variable((*n)->value, (*t)->type);

		m->addParameter((*n)->value, var);
		// st->put((*n)->value, var);

		if ((*n)->children.size() > 1)
		{
			getParam((*n)->children.rbegin(), m);
		}
	}

	Record *findIdentifier(Node *n, SymbolTable *st)
	{
		// if (n->type == "Goal")
		//{
		//	st->put(n->value, new Class(n->value, n->type));
		// }
		if (n->type == "MainClass")
		{
			st->put(n->value, new Class(n->value, n->type));
		}
		//------------------------------------------------------
		else if (n->type == "MethodDeclaration")
		{
			list<Node *>::iterator c = n->children.begin();
			st->put(n->value, new Method(n->value, (*c)->type));
		}
		else if (n->type == "MethodDeclaration param")
		{
			list<Node *>::iterator l_front = n->children.begin();
			Method *m = new Method(n->value, (*l_front)->type);
			advance(l_front, 1);
			list<Node *>::iterator t = (*l_front)->children.begin();
			Variable *var = new Variable((*l_front)->value, (*t)->type);
			// st->put((*l_front)->value, var);
			m->addParameter((*l_front)->value, var);

			if ((*l_front)->children.size() > 1)
			{
				getParam((*l_front)->children.rbegin(), m);
			}
			st->put(n->value, m);
		}
		else if (n->type == "Expression list")
		{
			list<Node *>::iterator l_front = n->children.begin();
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
			list<Node *>::iterator c = n->children.begin();
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
		return nullptr;
	}

	//------------------------------------------------------
	//--------------------SEMANTIC ANALYSIS-----------------
	//------------------------------------------------------
	string look_in_sym(string name, SymbolTable *st)
	{
		Record *r = st->lookup(name);
		if (r == nullptr)
		{
			cout << "Cannot find: " << name << " in symboltable" << endl;
			return "fault";
		}
		else
		{
			string statement_type = r->getType();
			return statement_type;
		}
	}

	vector<string> look_param(string name, SymbolTable *st)
	{
		Record *r = st->lookup(name);
		if (r == nullptr)
		{
			cout << "Cannot find: " << name << " in symboltable" << endl;
			vector<string> ret = {"fault"};
			return ret;
		}
		else
		{
			vector<string> para = r->getParameter();
			return para;
		}
	}

	string check_expression_list(Node *node, SymbolTable *st) // expression list :

	// int i = this.Init(sz)
	// expression list : Init
	// 1 expression: this
	// 2 indentifer: sz
	{
		// LOOK AT RETURN VALUE OF node->value;
		list<Node *>::iterator h = node->children.begin();
		if ((*h)->type == "Expression new identifier")
		{
			return "new";
		}
		// string type_method = look_in_sym(node->value, st);
		if ((*h)->type == "Expression")
		{
			if ((*h)->value != "THIS")
			{
				return "Wrong this";
			}
			else
			{
				string type;
				advance(h, 1);
				if ((*h)->type == "identifier")
				{
					type = look_in_sym((*h)->value, st);
					return type;
				}
				else if ((*h)->type == "Expression")
				{
					type = check_expression((*h), st);
				}
				else if ((*h)->type == "<INTEGER_LITERAL>")
				{
					type = "<INTEGER_LITERAL>";
				}
				else if ((*h)->type == "Expressionlist")
				{
					vector<string> para = look_param(node->value, st);
					int counter = 0;
					for (list<Node *>::iterator i = (*h)->children.begin(); i != (*h)->children.end(); i++)
					{
						string type = (*i)->type;
						if ((*i)->type == "identifier")
						{
							type = look_in_sym((*i)->value, st);
						}
						else if ((*i)->type == "Expression")
						{
							type = check_expression((*h), st);
						}
						if (para[counter] == type)
						{
							return para[counter];
						}
						counter++;
					}
				}
				else
				{
					cout << "No such expression" << endl;
					return "Wrong this";
				}
			}
		}
	}

	string check_expression(Node *node, SymbolTable *st)
	{
		// cout << node->type << ":" << node->value << endl;
		if (node->value == "or")
		{
			string type_one = node->children.front()->type;
			string type_two = node->children.back()->type;

			if (type_one == "identifier")
			{
				type_one = look_in_sym(node->children.front()->value, st);
			}
			if (type_two == "identifier")
			{
				type_two = look_in_sym(node->children.back()->value, st);
			}
			if (type_one == "Expression")
			{
				type_one = check_expression(node->children.front(), st);
			}
			if (type_two == "Expression")
			{
				type_two = check_expression(node->children.back(), st);
			}
			if (type_one != "<INTEGER_LITERAL>" || type_two != "<INTEGER_LITERAL>")
			{
				cout << node->value << " Wrong types" << endl;
				cout << "Check expressions match: " << type_one << " : " << type_two << endl;
			}
			return type_one;
		}
		else if (node->type == "Expression[]")
		{
			string ident = look_in_sym(node->children.front()->value, st);
			string number = node->children.back()->type;
			if (number == "identifier")
			{
				number = look_in_sym(node->children.back()->value, st);
			}
			if (number != "<INTEGER_LITERAL>" || ident != "Int[]")
			{
				return "fault";
			}
			else
			{
				return "<INTEGER_LITERAL>";
			}
		}
		else if (node->value == "less than" || node->value == "ADD" ||
				 node->value == "subtract" || node->value == "multiplication" ||
				 node->value == "Divided" || node->value == "larger")
		{
			string type_one = node->children.front()->type;
			string type_two = node->children.back()->type;
			if (type_one == "identifier")
			{
				type_one = look_in_sym(node->children.front()->value, st);
			}
			if (type_two == "identifier")
			{
				type_two = look_in_sym(node->children.back()->value, st);
			}
			if (type_one == "Expression")
			{
				type_one = check_expression(node->children.front(), st);
			}
			if (type_two == "Expression")
			{
				type_two = check_expression(node->children.back(), st);
			}

			if (type_one != "<INTEGER_LITERAL>" || type_two != "<INTEGER_LITERAL>")
			{
				cout << node->value << " Wrong types: " << type_one << " : " << type_two << endl;
			}
			return type_one;
		}
		else if (node->value == "NEW INT")
		{
			string type_one = node->children.front()->type;
			string type;
			if (type_one == "identifier")
			{
				type = look_in_sym(node->children.front()->value, st);
			}
			else if (type_one == "Expression")
			{
				type = check_expression(node->children.front(), st);
			}
			else if (type_one == "<INTEGER_LITERAL>")
			{
				type = "<INTEGER_LITERAL>";
			}

			if (type == "<INTEGER_LITERAL>")
			{
				return "Int[]";
			}
			else
			{
				return "wrong";
			}
		}
		else if (node->value == "and")
		{
			string type_one = node->children.front()->type;
			string type_two = node->children.back()->type;

			if (type_one == "identifier")
			{
				type_one = look_in_sym(node->children.front()->value, st);
			}
			if (type_two == "identifier")
			{
				type_two = look_in_sym(node->children.back()->value, st);
			}
			if (type_one == "Expression")
			{
				type_one = check_expression(node->children.front(), st);
			}
			if (type_two == "Expression")
			{
				type_two = check_expression(node->children.back(), st);
			}
			if (type_one != "<INTEGER_LITERAL>" || type_two != "<INTEGER_LITERAL>")
			{
				cout << node->value << " has wrong types" << endl;
				cout << type_one << " : " << type_two << endl;
			}
			return type_one;
		}
		else if (node->value == "TRUE" || node->value == "FALSE")
		{
			return "boolean";
		}
		else if (node->value == "NOT")
		{
			string s = check_expression(node->children.front(), st);
			return s;
		}
		else if (node->value == "param")
		{
			string s = look_in_sym(node->children.front()->value, st);
			if (s != "fault")
			{
				vector<string> p = look_param(node->children.front()->value, st);
				string g = check_expression_list(node->children.front(), st);

				if (p[0] == g)
				{
					return g;
				}
				return "Worng parameter type";
			}
		}
		else if (node->value == "len")
		{
			string s = look_in_sym(node->children.front()->value, st);
			if (s == "Int[]")
			{
				return "<INTEGER_LITERAL>";
			}
		}
		return "NULL";
	}

	void find_identifier(Node *node, SymbolTable *st)
	{
		if (node->type == "Statement")
		{
			// cin.get();
			if (node->value == "if" || node->value == "while")
			{
				for (list<Node *>::iterator i = node->children.begin(); i != node->children.end(); i++)
				{
					if ((*i)->type == "Expression")
					{
						check_expression(node, st);
					}
					else if ((*i)->type == "Statement" || (*i)->type == "Statement assign")
					{
						find_identifier((*i), st);
					}
				}
			}
			if (node->value == "sys.out")
			{
				list<Node *>::iterator t = node->children.begin();
				if ((*t)->type == "Expression" || (*t)->type == "Expression[]")
				{
					string type_two = check_expression((*t), st);
					if (type_two == "NULL" || type_two != "<INTEGER_LITERAL>")
					{
						cout << "Sys.out has wrong Expression or not of type int: " << type_two << endl;
					}
				}
				else if ((*t)->type == "Expression list") // också klass
				{
					string assign_type = check_expression_list((*t), st);
					if (assign_type == "Wrong this")
					{
						cout << "Expression list dont have correct types" << endl;
					}
					else if (assign_type != "new" && assign_type != "<INTEGER_LITERAL>")
					{
						cout << "Expression list dont have correct types" << endl;
					}
				}
				else
				{
					if ((*t)->type != "<INTEGER_LITERAL>")
					{
						// cout << "Expression list dont have correct types" << endl;
						cout << "sys.out no int" << endl;
					}
				}
			}
		}
		if (node->type == "Statement assign")
		{
			string assign_type = "";
			string statement_type = "";
			statement_type = look_in_sym(node->value, st);

			list<Node *>::iterator i = node->children.begin();
			if ((*i)->type == "identifier")
			{
				assign_type = look_in_sym((*i)->value, st);
			}
			else if ((*i)->type == "Expression")
			{
				assign_type = check_expression((*i), st);
			}
			else if ((*i)->type == "Expression list")
			{
				assign_type = check_expression_list((*i), st);
				if (assign_type == "Wrong this")
				{
					assign_type = "Expression list dont have correct types";
				}
				if (assign_type == "new")
				{
					assign_type = statement_type;
				}
			}
			if ((*i)->type == "Expression.identifier")
			{
				list<Node *>::iterator h = (*i)->children.begin();
				if ((*h)->value != "THIS")
				{
					cout << "identifier not THIS" << endl;
				}
				assign_type = statement_type;
			}
			else if ((*i)->type == "Expression[]")
			{
				assign_type = check_expression((*i), st);
			}
			else if ((*i)->type == "<INTEGER_LITERAL>")
			{
				assign_type = "<INTEGER_LITERAL>";
			}
			if (statement_type != assign_type)
			{
				cout << "failed to assign: " << node->value << endl;
			}
		}
		if (node->type == "Statementlist assign")
		{
			bool true_1, true_2;
			true_1 = true;
			true_2 = true;
			if (node->children.front()->type != "<INTEGER_LITERAL>")
			{
				true_1 = false;
			}
			if (true_1 == false)
			{
				if (look_in_sym(node->children.front()->value, st) !=
						"<INTEGER_LITERAL>" &&
					true_1 != true)
				{
					true_2 = false;
				}
			}
			if (true_1 == true || true_2 == true)
			{
				string assign_type;
				string type = node->children.back()->type;
				if (type == "identifier")
				{
					assign_type = look_in_sym(node->children.back()->value, st);
				}
				else if (type == "Expression")
				{
					assign_type = check_expression(node->children.back(), st);
				}
				else if (type == "Expression list")
				{
					assign_type = check_expression_list(node->children.back(), st);
					if (assign_type == "Wrong this")
					{
						assign_type = "Expression list dont have correct types";
					}
					if (assign_type == "new")
					{
						assign_type = type;
					}
				}
				else if (type == "Expression[]")
				{
					assign_type = check_expression(node->children.back(), st);
				}
				else if (type == "<INTEGER_LITERAL>")
				{
					assign_type = "<INTEGER_LITERAL>";
				}
			}
			else
			{
				if (true_1 == false)
				{
					cout << "Invalid index type " << node->children.front()->type << endl;
				}
			}
		}
	}

	void semantic_traverse(list<Node *> node, SymbolTable *st)
	{
		for (list<Node *>::iterator i = node.begin(); i != node.end(); i++)
		{
			find_identifier((*i), st);
			if ((*i)->type == "MainClass" || (*i)->type == "ClassDeclaration" ||
				(*i)->type == "MethodDeclaration param" ||
				(*i)->type == "MethodDeclaration")
			{
				st->enterScope();
			}
			if ((*i)->children.size() > 0)
			{
				semantic_traverse((*i)->children, st);
			}
			if ((*i)->type == "MainClass" || (*i)->type == "ClassDeclaration" ||
				(*i)->type == "MethodDeclaration param" ||
				(*i)->type == "MethodDeclaration")
			{
				st->exitScope();
			}
		}
	}

	void semantic_analysis(SymbolTable *st)
	{
		cout << "Starting semantic analysis:" << endl;
		cout << "---------------------------" << endl;
		st->resetTable();
		// st->enterScope();
		semantic_traverse(this->children, st);
		// st->exitScope();
		// st->resetTable();
	}

//------------------------------------------------------
//--------------------------IR--------------------------
//------------------------------------------------------
#include "CFG/basic-block.hh"
#include "CFG/table.hh"
#include "CFG/tac.hh"
#include <fstream>
#include <iostream>

	list<Node *>::iterator find_child_node(int index, string non = "")
	{
		if (children.size() > index)
		{
			list<Node *>::iterator node = children.begin();
			advance(node, index);
			return node;
		}
		else
			cout << "ERROR: NODE INDEX OUT OF BOUNDS" << endl;
	}

	void find_statement(list<Node *> node, cfg_table *cfg)
	{
		for (list<Node *>::iterator n = node.begin(); n != node.end(); n++)
		{
			string node_type = (*n)->type;
			string node_value = (*n)->value;
			if (node_type == "ClassDeclaration")
			{
				cfg->set_class_name(node_value);
			}
			if (
				node_type == "MethodDeclaration param" ||
				node_type == "MethodDeclaration")
			{
				std::string cn = cfg->get_class_name();
				cfg->create_block();
				basic_block *method_block = cfg->get_current();
				method_block->set_method(cn + "." + node_value);
			}

			if (node_type == "Statement")
			{
				if (node_value == "if")
				{
					int nr_children = (*n)->children.size();

					list<Node *>::iterator condition_node = (*n)->find_child_node(0);

					basic_block *condition_block = cfg->get_current();

					string res = (*condition_node)->eval_expression(cfg, condition_block);

					basic_block *true_block = cfg->create_block();
					list<Node *>::iterator true_node = (*n)->find_child_node(1);
					(*true_node)->find_statement((*true_node)->children, cfg);

					basic_block *latest_true_block = cfg->get_blocks().back();

					basic_block *false_block = nullptr;
					if (nr_children > 2)
					{
						false_block = cfg->create_block();
						list<Node *>::iterator false_node = (*n)->find_child_node(2);
						(*false_node)->find_statement((*false_node)->children, cfg);

						int falseBlockId = false_block->get_current_id();
						condition_block->add_conditional_jump_instruction(res, to_string(falseBlockId), "iffalse");
					}
					else
					{
						false_block = cfg->create_block();
						int falseBlockId = false_block->get_current_id();
						condition_block->add_conditional_jump_instruction(res, to_string(falseBlockId), "iffalse");
					}

					condition_block->set_exit(true_block);
					condition_block->set_next(false_block);

					list<Tac *> true_instructions = true_block->get_instructions();

					basic_block *next_block = cfg->create_block();

					if (true_instructions.back()->get_op() != "goto")
					{
						true_block->add_jump_instruction(to_string(next_block->get_current_id()));
						true_block->set_next(next_block);
					}
					else
					{
						latest_true_block->add_jump_instruction(to_string(next_block->get_current_id()));
						latest_true_block->set_next(next_block);
					}

					if (nr_children > 2)
					{
						list<Tac *> false_instructions = false_block->get_instructions();

						basic_block *latest_false_block = cfg->get_blocks().back();

						if (false_instructions.back()->get_op() != "goto")
						{
							false_block->add_jump_instruction(to_string(next_block->get_current_id()));
							false_block->set_next(next_block);
						}
						else
						{
							latest_false_block->add_jump_instruction(to_string(next_block->get_current_id()));
							latest_false_block->set_next(next_block);
						}
					}
					else
					{
						false_block->add_jump_instruction(to_string(next_block->get_current_id()));
						false_block->set_next(next_block);
					}
				}
				else if (node_value == "while")
				{
					list<Node *>::iterator condition_node = (*n)->find_child_node(0);
					basic_block *current_block = cfg->get_current();

					basic_block *condition_block = cfg->create_block();

					current_block->add_jump_instruction(to_string(condition_block->get_current_id()));
					current_block->set_next(condition_block);

					string res = (*condition_node)->eval_expression(cfg, condition_block);

					if ((*n)->children.size() > 0)
					{
						basic_block *true_block = cfg->create_block();
						list<Node *>::iterator body_node = (*n)->find_child_node(1);

						(*body_node)->find_statement((*body_node)->children, cfg);

						basic_block *latest_block = cfg->get_current();

						basic_block *false_block = cfg->create_block();

						condition_block->add_conditional_jump_instruction(res, to_string(false_block->get_current_id()), "iffalse");

						condition_block->set_exit(true_block);
						condition_block->set_next(false_block);

						latest_block->add_instruction(*new tac_jump(to_string(condition_block->get_current_id())));
						latest_block->set_next(condition_block);
					}
					else
					{
						cout << "Has no body" << endl;
					}
				}
				else if (node_value == "sys.out")
				{
					basic_block *bl = cfg->get_current();
					list<Node *>::iterator condition_node = (*n)->find_child_node(0);
					string res = (*condition_node)->eval_expression(cfg, bl);
					bl->add_print_instruction(res);
				}
			}
			else if (node_type == "Statement assign")
			{
				string assigned = node_value;
				string statement_name = value;
				basic_block *bl = cfg->get_current();

				list<Node *>::iterator current_node = (*n)->find_child_node(0);
				string arg1 = (*current_node)->eval_expression(cfg, bl);
				if (arg1 == "default")
				{
					arg1 = (*n)->value;
				}

				bl->add_copy_instruction(assigned, arg1);
			}
			else if (node_type == "Statementlist assign")
			{
				string assigned = node_value;
				string statement_name = value;
				basic_block *bl = cfg->get_current();

				list<Node *>::iterator current_node = (*n)->find_child_node(0);
				string arg1 = (*current_node)->eval_expression(cfg, bl);
				if (arg1 == "default")
				{
					arg1 = (*n)->value;
				}

				list<Node *>::iterator current_node2 = (*n)->find_child_node(1);
				string arg2 = (*current_node2)->eval_expression(cfg, bl);
				if (arg2 == "default")
				{
					arg2 = (*n)->value;
				}
				bl->add_access_array_instruction("copy", assigned, arg1, arg2);
			}
			else
			{
				find_statement((*n)->children, cfg);
			}
			if (
				node_type == "MethodDeclaration param" ||
				node_type == "MethodDeclaration")
			{
				basic_block *bl = cfg->get_current();

				list<Node *>::iterator current_node = (*n)->find_child_node((*n)->children.size() - 1);
				string res = (*current_node)->eval_expression(cfg, bl);

				bl->add_return_instruction(res);
			}

			if (node_type == "MainClass")
			{
				basic_block *bl = cfg->get_current();
				list<Node *>::iterator current_node = (*n)->find_child_node(1);
				if ((*current_node)->value == "")
				{
					list<Node *>::iterator Statementlist_node = (*current_node)->find_child_node(0);
					find_statement((*Statementlist_node)->children, cfg);
				}
				bl->set_method(node_value + ".main");
				bl->add_exit_instruction();
			}
		}
	}

	string eval_expression(cfg_table *cfg, basic_block *block)
	{
		string res = "default";
		if (type == "<INTEGER_LITERAL>")
		{
			return value;
		}
		else if (type == "identifier")
		{
			return value;
		}
		else if (type == "Expression")
		{
			if (value == "TRUE" || value == "FALSE" || value == "THIS")
			{
				return value;
			}
			else if (
				value == "and" ||
				value == "or" ||
				value == "ADD" ||
				value == "subtract" ||
				value == "Divided" ||
				value == "multiplication" ||
				value == "less than" ||
				value == "larger")
			{
				string arg1 = (*find_child_node(0))->eval_expression(cfg, block);
				string arg2 = (*find_child_node(1))->eval_expression(cfg, block);
				string expression_res = cfg->set_unique_name();
				block->add_expression_instruction(value, arg1, arg2, expression_res);
				res = expression_res;
			}
			else if (value == "NOT")
			{
				string arg1 = (*find_child_node(0))->eval_expression(cfg, block);
				string expression_res = cfg->set_unique_name();
				block->add_unary_expression_instruction(arg1, "NOT", expression_res);
				res = expression_res;
			}
			else if (value == "len")
			{
				string arg1 = (*find_child_node(0))->eval_expression(cfg, block);
				string expression_res = cfg->set_unique_name();
				block->add_length_instruction(arg1, expression_res);
				res = expression_res;
			}
			else if (value == "NEW INT") // index access x = arr1[y]
			{
				string arg1, arg2, result;
				arg2 = (*find_child_node(0))->eval_expression(cfg, block);
				result = cfg->set_unique_name();
				res = result;
				block->add_new_array_instruction("int[]", arg2, result);
			}

			else if (value == "param") // index access x = arr1[y]
			{
				return (*find_child_node(0))->eval_expression(cfg, block);
			}
			else
			{

				if (children.size() == 0)
				{
					return value;
				}

				string arg1 = (*find_child_node(0))->eval_expression(cfg, block);
				string arg2 = (*find_child_node(1))->eval_expression(cfg, block);
				string operand = value;
				string expression_res = cfg->set_unique_name();
				block->add_expression_instruction(operand, arg1, arg2, expression_res);
				res = expression_res;
			}
		}
		else if (type == "Expression[]") // index access x = arr1[y]
		{
			string arg1, arg2, result;
			arg1 = (*find_child_node(0))->eval_expression(cfg, block);
			arg2 = (*find_child_node(1))->eval_expression(cfg, block);
			result = cfg->set_unique_name();
			res = result;
			block->add_access_array_instruction("assign", arg1, arg2, result);
		}
		else if (type == "Expression list")
		{
			if ((*find_child_node(1))->type == "Expression.identifier")
			{
				string arg2 = (*find_child_node(1))->eval_expression(cfg, block);
				string arg1 = (*find_child_node(0))->eval_expression(cfg, block);
				if (arg1 != "default")
				{
					block->add_parameter_instruction(arg1);
				}
				if (arg2 != "default")
				{
					block->add_parameter_instruction(arg2);
				}
			}
			else
			{
				string arg1 = (*find_child_node(0))->eval_expression(cfg, block);
				if (arg1 != "default")
				{
					block->add_parameter_instruction(arg1);
				}
				string arg2 = (*find_child_node(1))->eval_expression(cfg, block);
				if (arg2 != "default")
				{
					block->add_parameter_instruction(arg2);
				}
			}
			int num_param = block->get_parameter_number();
			string expression_res = cfg->set_unique_name();
			block->add_call_method_instruction(value, to_string(num_param), expression_res);
			res = expression_res;
		}
		else if (type == "Expressionlist")
		{
			string arg1 = (*find_child_node(0))->eval_expression(cfg, block);
			block->add_parameter_instruction(arg1);

			string arg2 = (*find_child_node(1))->eval_expression(cfg, block);
			if (arg2 != "default")
			{
				block->add_parameter_instruction(arg2);
			}
		}
		else if (type == "Expression.identifier")
		{
			string arg1 = (*find_child_node(0))->eval_expression(cfg, block);
			block->add_parameter_instruction(arg1);

			int num_param = block->get_parameter_number();

			string expression_res = cfg->set_unique_name();
			block->add_call_method_instruction(value, to_string(num_param), expression_res);
			res = expression_res;
		}
		else if (type == "Expression new identifier")
		{
			string expression_res = cfg->set_unique_name();
			block->add_new_instruction(value, expression_res);
			res = expression_res;
		}
		return res;
	}

	void ir(SymbolTable *st)
	{

		cout << "____________________________________\n";
		cout << "GENERATING IR\n";
		cout << "____________________________________\n";
		cfg_table *cfg = new cfg_table();
		find_statement(this->children, cfg);
		ofstream myfile;
		myfile.open("cfg.dot");
		myfile << "digraph {" << endl;
		myfile << "graph [splines=ortho]" << endl;
		myfile << "node [shape=box];" << endl;
		cfg->generate_cfg(myfile);
		myfile << "\n}" << endl;
		myfile.close();
		cfg->generate_bytecode(st);
	}
};
#endif
