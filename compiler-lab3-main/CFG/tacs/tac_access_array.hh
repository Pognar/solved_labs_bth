#ifndef TAC_ACCESS_ARRAY_HH
#define TAC_ACCESS_ARRAY_HH
#include "../tac.hh"

class tac_access_array : public Tac {
    public:
    tac_access_array(std::string op, std::string arg1, std::string arg2, std::string result){
        set_op(op);
        set_arg1(arg1);
        set_arg2(arg2);
        set_result(result);
    }
    virtual std::string to_string() override {
        if(get_op() == "copy") {
            return get_arg1() + "[" + get_arg2() + "] := " + get_result()+ "\\n";
        } else if(get_op() == "assign") {
            return get_result() + " := " + get_arg1()+ "[" + get_arg2() + "]\\n";
        }
    }
    virtual tac_type get_tac_type() override {
        return ARRAY_ACCESS;
    }

    virtual std::string to_bytecode() {
        return "";
    }
};

#endif // TAC_ACCESS_ARRAY_HH