#ifndef TAC_RETURN_HH
#define TAC_RETURN_HH

#include "../tac.hh"

class tac_return : public Tac
{
public:
    tac_return(std::string ret_val)
    {
        set_op("return");
        set_arg1(ret_val);
    }
    virtual std::string to_string()
    {
        return get_op() + " " + get_arg1() + "\\n";
    }
    virtual tac_type get_tac_type() override
    {
        return RETURN;
    }

    virtual std::string to_bytecode()
    {
        std::string bytecode = "";
        std::string valueType = is_num(this->get_arg1()) ? "iconst " : "iload ";
        bytecode += valueType + this->get_arg1() + "\n";
        bytecode += "ireturn\n";
        return bytecode;
    }
};

#endif // TAC_RETURN_HH