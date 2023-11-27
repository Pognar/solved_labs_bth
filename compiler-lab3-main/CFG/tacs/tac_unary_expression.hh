#ifndef TAC_UNARY_EXPRESSION_HH
#define TAC_UNARY_EXPRESSION_HH
#include "../tac.hh"

class tac_unary_expression : public Tac
{
public:
    tac_unary_expression(std::string arg1, std::string op, std::string result)
    {
        set_arg1(arg1);
        set_op(op);
        set_result(result);
    }
    virtual std::string to_string() override
    {
        return get_result() + " := " + get_op() + " " + get_arg1() + "\\n";
    }
    virtual tac_type get_tac_type() override
    {
        return UNERY_EXPRESSION;
    }

    virtual std::string to_bytecode()
    {
        std::string bytecode = "";
        std::string valueType = is_num(this->get_arg1()) ? "iconst " : "iload ";

        bytecode += valueType + this->get_arg1() + "\n";
        bytecode += "inot\n";
        bytecode += "istore " + this->get_result() + "\n";
        
        return bytecode;
    }
};

#endif // TAC_UNARY_EXPRESSION_HH