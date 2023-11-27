#ifndef BASIC_BLOCK_HH
#define BASIC_BLOCK_HH

#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include "tac.hh"
#include "tacs/tac_jump.hh"

class basic_block
{

public:
	basic_block()
	{
		visited = false;
		exit = nullptr;
		next = nullptr;
		this->tracker = 0;
		this->tracker = 0;
		code = "Empty";
		id = 0;
	}

	basic_block(int current_block_id)
	{
		visited = false;
		exit = nullptr;
		next = nullptr;
		this->current_id = current_block_id;
		this->label = "block_" + std::to_string(current_block_id);
		this->tracker = 0;
		this->tracker = 0;
		code = "Empty";
		id = 0;
	}

	basic_block &operator=(const basic_block *source)
	{
		if (source != nullptr)
		{
			this->label = source->label;
			this->tracker = source->tracker;
			this->tac_instructions = source->tac_instructions;
			this->condition = source->condition;
			this->exit = source->exit;
			this->next = source->next;
		}
		return *this;
	}

	basic_block &operator=(basic_block source)
	{
		std::swap(label, source.label);
		std::swap(tracker, source.tracker);
		std::swap(tac_instructions, source.tac_instructions);
		std::swap(condition, source.condition);
		std::swap(id, source.id);
		std::swap(exit, source.exit);
		std::swap(next, source.next);
		return *this;
	}

	int get_id() { return this->id; }

	int get_tracker() { return this->tracker; }
	std::string get_label() { return this->label; }
	std::string get_code() { return this->code; }
	basic_block *get_exit() { return this->exit; }
	basic_block *get_next() { return this->next; }

	void set_tracker(int &new_tracker) { this->tracker = new_tracker++; }
	void set_label(std::string &new_label) { this->label = new_label; }
	void set_code(std::string &new_code) { this->code = new_code; }
	void set_exit(basic_block *new_exit)
	{
		this->exit = new_exit;
	}
	void set_next(basic_block *new_next) { this->next = new_next; }

	void set_exit_true(basic_block &new_next) { this->next = &new_next; }
	void set_exit_false(basic_block &new_next) { this->next = &new_next; }

	void set_method(std::string current_method) { this->current_method = current_method; }
	std::string get_method() { return this->current_method; }

	void set_class_name(std::string class_name) { this->class_name = class_name; }
	std::string get_class_name() { return this->class_name; }

	void set_condition(Tac &new_condition) { this->condition = &new_condition; }
	void add_instruction(Tac &new_instruction)
	{
		this->tac_instructions.push_back(&new_instruction);
	}

	void add_copy_instruction(std::string result, std::string arg1)
	{
		this->tac_instructions.push_back(new tac_copy(result, arg1));
	}

	void add_jump_instruction(std::string label)
	{
		this->tac_instructions.push_back(new tac_jump(label));
	}

	void add_conditional_jump_instruction(std::string arg1, std::string arg2, std::string label)
	{
		this->tac_instructions.push_back(new tac_conditional_jmp(arg1, arg2, label));
	}

	void add_access_array_instruction(std::string op, std::string arg1, std::string arg2, std::string result)
	{
		this->tac_instructions.push_back(new tac_access_array(op, arg1, arg2, result));
	}

	void add_expression_instruction(std::string op, std::string arg1, std::string arg2, std::string result)
	{
		this->tac_instructions.push_back(new tac_expression(op, arg1, arg2, result));
	}

	void add_length_instruction(std::string arg1, std::string result)
	{
		this->tac_instructions.push_back(new tac_length(arg1, result));
	}

	void add_new_instruction(std::string arg1, std::string result)
	{
		this->tac_instructions.push_back(new tac_new(arg1, result));
	}

	void add_new_array_instruction(std::string arg1, std::string arg2, std::string result)
	{
		this->tac_instructions.push_back(new tac_new_array(arg1, arg2, result));
	}

	void add_parameter_instruction(std::string arg1)
	{
		this->tac_instructions.push_back(new tac_parameter(arg1));
	}

	void add_print_instruction(std::string arg1)
	{
		this->tac_instructions.push_back(new tac_print(arg1));
	}

	void add_return_instruction(std::string arg1)
	{
		this->tac_instructions.push_back(new tac_return(arg1));
	}

	void add_unary_expression_instruction(std::string op, std::string arg1, std::string result)
	{
		this->tac_instructions.push_back(new tac_unary_expression(op, arg1, result));
	}

	void add_call_method_instruction(std::string arg1, std::string arg2, std::string result)
	{
		this->tac_instructions.push_back(new tac_call_method(arg1, arg2, result));
	}

	void add_exit_instruction()
	{
		this->tac_instructions.push_back(new tac_exit());
	}

	list<Tac *> get_instructions() { return this->tac_instructions; }
	int get_current_id() { return this->current_id; }

	void generate_cfg(std::ofstream &file)
	{
		if (visited)
		{
			return;
		}
		visited = true;
		file << label << " [label=\"" << label << "\n"
			 << to_string() << "\"];" << std::endl;
		if (exit)
		{
			file << label << " -> " << exit->get_label() << ";" << std::endl;
			exit->generate_cfg(file);
		}
		if (next)
		{
			file << label << " -> " << next->get_label() << ";" << std::endl;
			next->generate_cfg(file);
		}
	}

	void cfg_content(std::ofstream &file)
	{
		if (visited)
		{
			return;
		}
		visited = true;
		file << label << " [label=\"" << label << ": " << to_string() << "\"];" << std::endl;
		if (exit)
		{
			file << label << " -> " << exit->get_label() << ";" << std::endl;
			exit->cfg_content(file);
		}
		if (next)
		{
			file << label << " -> " << next->get_label() << ";" << std::endl;
			next->cfg_content(file);
		}
	}

	std::string to_string()
	{
		std::string ss = "";

		for (auto i = tac_instructions.begin(); i != tac_instructions.end(); ++i)
		{
			ss += (*i)->to_string();
		}
		return ss;
	}

	int get_parameter_number()
	{
		int num_param = 0;
		std::list<Tac *>::reverse_iterator it;
		for (it = tac_instructions.rbegin(); it != tac_instructions.rend(); ++it)
		{
			if ((*it)->get_op() == "methodCall")
			{
				break;
			}
			if ((*it)->get_op() == "param")
			{
				num_param = num_param + 1;
			}
		}
		return num_param;
	}

private:
	int tracker;
	bool visited;
	std::string label;
	std::string code;
	basic_block *exit, *next;
	string class_name = "";
	std::string current_method = "";
	Tac *condition;
	int id;
	int current_id;
	bool truthy;
	std::list<Tac *> tac_instructions;
};

#endif // BASIC_BLOCK_HH
