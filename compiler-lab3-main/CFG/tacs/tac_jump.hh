#ifndef TAC_JUMP_HH
#define TAC_JUMP_HH

#include "../tac.hh"

class tac_jump : public Tac {
    public:
    tac_jump(std::string label){
        set_op("goto");
        set_result(label);
    }

    virtual std::string to_string() override {
        return get_op() + " " + get_result();
    }
    virtual tac_type get_tac_type() override {
        return JUMP;
    }

    virtual std::string to_bytecode() {
        std::string bytecode = "";
        bytecode += "goto block_" + this->get_result() + "\n";
        return bytecode;
    }
};
#endif // TAC_JUMP_HH