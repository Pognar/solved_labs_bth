#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <stack>
#include <list>
#include <unordered_map>

template <typename T>
void print_anything(T input)
{
    std::cout << input << std::endl;
}

void print_vector(std::vector<std::string> instruction_vector)
{
    for (auto instruction : instruction_vector)
    {
        std::cout << "instruction: " << instruction << std::endl;
    }
}

class Method
{
public:
    std::list<std::string> bytecode;
    Method() {}

    Method(std::list<std::string> lv, std::list<std::string> bc)
    {
        this->bytecode = bc;
        this->local_variables = lv;
    }

    void add_instructions(std::string new_instructions)
    {
        this->bytecode.push_back(new_instructions);
    }
    std::string get_instruction()
    {
        std::list<std::string>::iterator it = bytecode.begin();
        advance(it, this->program_counter);
        this->program_counter++;
        return *it;
    }

    void set_program_counter(std::string block)
    {
        int new_pc = blptr[block] - 1;
        this->program_counter = new_pc;
    }

    std::unordered_map<std::string, int> get_variables()
    {
        return this->variables;
    }

    void set_blptr(int pc, std::string value)
    {
        blptr[value] = pc;
    }

private:
    std::list<std::string> local_variables;
    std::unordered_map<std::string, int> blptr;
    std::unordered_map<std::string, int> variables;
    std::vector<std::vector<std::string>> start_points;
    int program_counter = 0;
};

class Scope
{
public:
    Method current_method;
    std::unordered_map<std::string, int> variables;
    int instruction_counter = 0;

    Scope()
    {
    }

    Scope(Method current_method)
    {
        this->current_method = current_method;
        this->variables = current_method.get_variables();
    }

    void add_variable(std::string variable_name, int value)
    {
        this->variables[variable_name] = value;
    }

    void print_variables()
    {
        for (auto variable : this->variables)
        {
            std::cout << "variable: " << variable.first << " value: " << variable.second << std::endl;
        }
    }

    Method get_method()
    {
        return this->current_method;
    }

    std::string next_instruction()
    {
        return this->current_method.get_instruction();
    }

    void set_instruction_counter(std::string block)
    {
        this->current_method.set_program_counter(block);
    }

    int get_variable(std::string variable_name)
    {
        if (this->variables.find(variable_name) != this->variables.end())
        {
            return this->variables[variable_name];
        }
        return NULL;
    }

    void set_variable(std::string variable_name, int value)
    {

        this->variables[variable_name] = value;
    }
};
class Instructions
{
public:
    void byte_code_loader(std::string bytecode_file)
    {
        std::ifstream file(bytecode_file);
        if (!file.is_open())
        {
            std::cout << "Error: File not found" << std::endl;
            return;
        }
        int program_counter = 0;
        std::string cur_instruction;
        std::string method_name;
        Method current_method = Method();
        int counter = 0;
        while (std::getline(file, cur_instruction))
        {
            bool flag = dot_in_string(cur_instruction); // look for class.method
            if (flag)
            {
                if (method_name != "")
                {
                    add_method(current_method, method_name);
                    current_method = Method();
                    counter = 0;
                }
                std::string func = cur_instruction.substr(cur_instruction.find(".") + 1, cur_instruction.size() - 1);
                method_name = func;
                counter++;
            }

            else if (w_in_string(cur_instruction, "block") && !w_in_string(cur_instruction, " "))
            {
                current_method.set_blptr(counter, cur_instruction);
            }
            else
            {
                current_method.add_instructions(cur_instruction);
                counter++;
            }
            add_method(current_method, method_name);
            program_counter++;
        }
        file.close();
    }

    void add_method(Method cur_method, std::string byte_code)
    {
        method_map[byte_code] = cur_method;
    }

    bool w_in_string(std::string instr, std::string value)
    {
        size_t found = instr.find(value);
        if (found != std::string::npos)
        {
            return true;
        }
        return false;
    }

    bool dot_in_string(std::string instr)
    {
        for (int instruction = 0; instruction < instr.length(); instruction++)
        {
            if (instr[instruction] == '.')
            {
                return true;
            }
        }
        return false;
    }

    Method get_(std::string method_name)
    {
        return method_map[method_name];
    }

private:
    std::unordered_map<std::string, Method> method_map;
};

class Interpreter
{
public:
    Instructions instructions = Instructions();
    std::stack<int> op_stack;
    std::list<Scope> scope_stack;
    Scope current_scope;

    Interpreter(Instructions instructions)
    {
        this->instructions = instructions;
        entry_point();
    }

    void print_stack()
    {
        std::stack<int> temp_stack = op_stack;
        while (!temp_stack.empty())
        {
            std::cout << temp_stack.top() << ",";
            temp_stack.pop();
        }
        print_anything("\n");
    }

    void entry_point()
    {
        Method entry_method = instructions.get_("main");
        current_scope = Scope(entry_method);
        std::string method_name = "main";
        std::string instruction = current_scope.next_instruction();

        int stop_spam = 0;

        // while (instruction != "exit" && stop_spam < 100)
        while (instruction != "exit")
        {
            stop_spam++;
            std::istringstream iss(instruction);
            std::string token;
            std::vector<std::string> tokens;
            while (iss >> token)
            {
                tokens.push_back(token);
            }
            if (tokens.empty())
            {
                continue; // Ignore empty lines.
            }

            std::string operation = tokens[0];

            if (operation == "invoke")
            {
                method_name = tokens[1];
                scope_stack.push_front(current_scope);
                current_scope = Scope(instructions.get_(method_name));
            }
            else if (operation == "iconst")
            {
                if (tokens.size() < 2)
                {
                    std::cout << "Error: Missing value for iconst." << std::endl;
                }
                else
                {
                    int value;
                    if (std::istringstream(tokens[1]) >> value)
                    {
                        op_stack.push(value);
                    }
                    else
                    {
                        std::cout << "Error: Invalid value for iconst." << std::endl;
                    }
                }
            }
            else if (operation == "goto")
            {
                current_scope.set_instruction_counter(tokens[1]);
            }
            else if (operation == "iffalse")
            {
                int a = op_stack.top();
                if (!a)
                {
                    std::string block = tokens[2];
                    op_stack.pop();
                    current_scope.set_instruction_counter(block);
                }
                else
                {
                    op_stack.pop();
                }
            }
            else if (operation == "iload")
            {
                if (tokens.size() < 2)
                {
                    std::cout << "Error: Missing variable name in iload." << std::endl;
                }
                else
                {
                    std::string variable_name = tokens[1];
                    op_stack.push(current_scope.get_variable(variable_name));
                }
            }
            else if (operation == "istore")
            {
                if (tokens.size() < 2)
                {
                    std::cout << "Error: Missing variable name in istore." << std::endl;
                }
                else
                {
                    std::string variable_name = tokens[1];
                    if (op_stack.empty())
                    {
                        current_scope.set_variable(variable_name, 0);
                        std::cout << "Error: Operand stack is empty in istore." << std::endl;
                    }
                    else
                    {
                        current_scope.set_variable(variable_name, op_stack.top());
                        op_stack.pop();
                    }
                }
            }
            else if (operation == "iadd")
            {
                if (op_stack.size() < 2)
                {
                    std::cout << "Error: Not enough operands for iadd." << std::endl;
                }
                else
                {
                    int b = op_stack.top();
                    op_stack.pop();
                    int a = op_stack.top();
                    op_stack.pop();
                    op_stack.push(a + b);
                }
            }
            else if (operation == "imul")
            {
                if (op_stack.size() < 2)
                {
                    std::cout << "Error: Not enough operands for imul." << std::endl;
                }
                else
                {
                    int b = op_stack.top();
                    op_stack.pop();
                    int a = op_stack.top();
                    op_stack.pop();
                    op_stack.push(a * b);
                }
            }
            else if (operation == "idiv")
            {
                if (op_stack.size() < 2)
                {
                    std::cout << "Error: Not enough operands for idiv." << std::endl;
                }
                else
                {
                    int b = op_stack.top();
                    op_stack.pop();
                    int a = op_stack.top();
                    op_stack.pop();
                    op_stack.push(a / b);
                }
            }
            else if (operation == "isub")
            {
                if (op_stack.size() < 2)
                {
                    std::cout << "Error: Not enough operands for isub." << std::endl;
                }
                else
                {
                    int b = op_stack.top();
                    op_stack.pop();
                    int a = op_stack.top();
                    op_stack.pop();
                    op_stack.push(a - b);
                }
            }
            else if (operation == "print")
            {
                if (op_stack.empty())
                {
                    std::cout << "Error: Operand stack is empty in print." << std::endl;
                }
                else
                {
                    print_anything(op_stack.top());
                    op_stack.pop();
                }
            }
            else if (operation == "inot")
            {
                if (op_stack.empty())
                {
                    std::cout << "Error: Operand stack is empty in inot." << std::endl;
                }
                else
                {
                    int a = op_stack.top();
                    op_stack.pop();
                    op_stack.push(!a);
                }
            }
            else if (operation == "iand")
            {
                if (op_stack.size() < 2)
                {
                    std::cout << "Error: Not enough operands for iand." << std::endl;
                }
                else
                {
                    int b = op_stack.top();
                    op_stack.pop();
                    int a = op_stack.top();
                    op_stack.pop();
                    op_stack.push(a && b);
                }
            }
            else if (operation == "ior")
            {
                if (op_stack.size() < 2)
                {
                    std::cout << "Error: Not enough operands for ior." << std::endl;
                }
                else
                {
                    int b = op_stack.top();
                    op_stack.pop();
                    int a = op_stack.top();
                    op_stack.pop();
                    op_stack.push(a || b);
                }
            }
            else if (operation == "ieq")
            {
                if (op_stack.size() < 2)
                {
                    std::cout << "Error: Not enough operands for ieq." << std::endl;
                }
                else
                {
                    int b = op_stack.top();
                    op_stack.pop();
                    int a = op_stack.top();
                    op_stack.pop();
                    op_stack.push(a == b);
                }
            }
            else if (operation == "ilt")
            {
                if (op_stack.size() < 2)
                {
                    std::cout << "Error: Not enough operands for ilt." << std::endl;
                }
                else
                {
                    int b = op_stack.top();
                    op_stack.pop();
                    int a = op_stack.top();
                    op_stack.pop();
                    op_stack.push(a < b);
                }
            }
            else if (operation == "igt")
            {
                if (op_stack.size() < 2)
                {
                    std::cout << "Error: Not enough operands for ilt." << std::endl;
                }
                else
                {
                    int b = op_stack.top();
                    op_stack.pop();
                    int a = op_stack.top();
                    op_stack.pop();
                    op_stack.push(a > b);
                }
            }
            else if (operation == "ireturn")
            {
                current_scope = scope_stack.front();
                scope_stack.pop_front();
            }
            else if (operation == "exit")
            {
                std::cout << "Exiting" << token << std::endl;
            }
            instruction = current_scope.next_instruction();
        }
    }
};

int main()
{
    Instructions instructions = Instructions();
    std::cout << "_______________OUTPUT________________" << std::endl;
    instructions.byte_code_loader("bytecode");
    Interpreter interpreter = Interpreter(instructions);
    std::cout << "_____________________________________" << std::endl;
    return 0;
}