#ifndef COPY_TAC_HH
#define COPY_TAC_HH

#include "../tac.hh"

class tac_copy : public Tac
{
public:
    tac_copy(std::string result, std::string arg1)
    {
        set_arg1(arg1);
        set_result(result);
    }
    virtual std::string to_string() override
    {
        return get_result() + " := " + get_arg1() + "\\n";
    }
    virtual tac_type get_tac_type() override
    {
        return COPY;
    }

    virtual std::string to_bytecode()
    {
        std::string bytecode = "";
        std::string valueType = "";

        if (is_num(this->get_arg1()))
            valueType = "iconst ";
        else
            valueType = "iload ";
        bytecode += valueType + this->get_arg1() + "\n";
        bytecode += "istore " + this->get_result() + "\n";
        return bytecode;
    }
};

#endif // COPY_TAC_HH