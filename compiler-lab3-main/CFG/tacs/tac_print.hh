#ifndef TAC_PRINT_HH
#define TAC_PRINT_HH

#include "../tac.hh"

class tac_print : public Tac {
    public:
    tac_print(std::string arg1) {
        set_arg1(arg1);
        set_op("print");
    }
    virtual std::string to_string() override {
        return get_op() + " " + get_arg1() + "\\n";
    }
    virtual tac_type get_tac_type() override {
        return PRINT;
    }

    virtual std::string to_bytecode() {
        std::string bytecode = "";
        std::string valueType = is_num(this->get_arg1()) ? "iconst " : "iload ";
        bytecode += valueType + this->get_arg1() + "\n";
        bytecode += "print\n";
        return bytecode;
    }
};

#endif // TAC_PRINT_HH