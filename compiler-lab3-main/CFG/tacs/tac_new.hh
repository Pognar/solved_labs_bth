#ifndef TAC_NEW_HH
#define TAC_NEW_HH

#include "../tac.hh"


class tac_new : public Tac {
    public:
    tac_new(std::string type, std::string result){
        set_op("new");
        set_arg1(type);
        set_result(result);
    }
    virtual std::string to_string() override {
        return get_result() + " := " + get_op() + " " + get_arg1() + "\\n";
    }
    virtual tac_type get_tac_type() override {
        return NEW;
    }

    virtual std::string to_bytecode() {
        return "";
    }
};

#endif // TAC_NEW_HH