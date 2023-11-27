#ifndef TAC_CONDITIONAL_JUMP_HH
#define TAC_CONDITIONAL_JUMP_HH

#include "../tac.hh"

class tac_conditional_jmp : public Tac
{
public:
    tac_conditional_jmp(std::string arg1, std::string arg2, std::string result)
    {
        set_op("goto");
        set_arg1(arg1);
        set_arg2(arg2);
        set_result(result);
    }
    virtual std::string to_string() override
    {
        return get_result() + " " + get_arg1() + " " + get_op() + " " + get_arg2();
    }
    virtual tac_type get_tac_type() override
    {
        return COND_JUMP;
    }

    virtual std::string to_bytecode()
    {
        std::string bytecode = "";
        std:: string value = "";
        if (is_num(this->get_arg1()))
        {
            value = "iconst ";
        }
        else
        {
            value = "iload ";
        }
        bytecode += value + this->get_arg1() + "\n";
        bytecode += "iffalse goto block_" + this->get_arg2() + "\n";
        return bytecode;
    }
};
#endif // TAC_CONDITIONAL_JUMP_HH