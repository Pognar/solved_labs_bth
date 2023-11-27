#ifndef TAC_CALL_METHOD_HH
#define TAC_CALL_METHOD_HH

#include "../tac.hh"

class tac_call_method : public Tac
{
public:
    tac_call_method(std::string arg1, std::string arg2, std::string result)
    {
        set_op("methodCall");
        set_arg1(arg1);
        set_arg2(arg2);
        set_result(result);
    }
    virtual std::string to_string() override
    {
        return get_result() + " := " + get_op() + " " + get_arg1() + ", " + get_arg2() + "\\n";
    }
    virtual tac_type get_tac_type() override
    {
        return METHOD_CALL;
    }

    virtual std::string to_bytecode()
    {
        std::string bytecode = "";
        bytecode += "invoke " + this->get_arg1() + "\n";
        bytecode += "istore " + this->get_result() + "\n";
        return bytecode;
    }
};

#endif // TAC_CALL_METHOD_HH