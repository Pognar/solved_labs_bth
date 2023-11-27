#ifndef TAC_EXPRESSION_HH
#define TAC_EXPRESSION_HH
#include "../tac.hh"

class tac_expression : public Tac
{
public:
    tac_expression(std::string op, std::string arg1, std::string arg2, std::string result)
    {
        set_op(op);
        set_arg1(arg1);
        set_arg2(arg2);
        set_result(result);
    }
    virtual std::string to_string() override
    {
        return get_result() + " := " + get_arg1() + " " + get_op() + " " + get_arg2() + "\\n";
    }
    virtual tac_type get_tac_type() override
    {
        return EXPRESSION;
    }

    virtual std::string to_bytecode()
    {
        std::string bytecode = "";

        std::string value_type = is_num(this->get_arg1()) ? "iconst " : "iload ";
        std::string value_type2 = is_num(this->get_arg2()) ? "iconst " : "iload ";

        bytecode += value_type + this->get_arg1() + "\n";
        bytecode += value_type2 + this->get_arg2() + "\n";

        std::string op = this->get_op();
        if (op == "ADD")
        {
            bytecode += "iadd\n";
        }
        else if (op == "subtract")
        {
            bytecode += "isub\n";
        }
        else if (op == "multiplication")
        {
            bytecode += "imul\n";
        }
        else if (op == "Divided")
        {
            bytecode += "idiv\n";
        }
        else if (op == "larger")
        {
            bytecode += "igt\n";
        }
        else if (op == "less than")
        {
            bytecode += "ilt\n";
        }
        else if (op == "if equal")
        {
            bytecode += "ieq\n";
        }
        else if (op == "and")
        {
            bytecode += "iand\n";
        }
        else if (op == "or")
        {
            bytecode += "ior\n";
        }

        bytecode += "istore " + this->get_result() + "\n";

        return bytecode;
    }
};

#endif // TAC_EXPRESSION_HH