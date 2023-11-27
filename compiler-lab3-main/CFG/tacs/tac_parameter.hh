#ifndef TAC_PARAMETER_HH
#define TAC_PARAMETER_HH

#include "../tac.hh"

class tac_parameter : public Tac {
    public:
    tac_parameter(std::string arg1) {
        set_arg1(arg1);
        set_op("param");
    }
    virtual std::string to_string() override {
        return get_op() + " " + get_arg1() + "\\n";
    }
    virtual tac_type get_tac_type() override {
        return PARAMETER;
    }

    virtual std::string to_bytecode() {
        std::string bytecode = "";
        std::string valueType = is_num(this->get_arg1()) ? "iconst " : "iload ";
        bytecode += valueType + this->get_arg1() + "\n";
        return bytecode;
    }
};

#endif // TAC_PARAMETER_HH