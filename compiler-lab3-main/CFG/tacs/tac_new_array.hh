#ifndef TAC_NEW_ARRAY_HH
#define TAC_NEW_ARRAY_HH

#include "../tac.hh"

class tac_new_array : public Tac {
    public:
    tac_new_array(std::string arg1, std::string arg2, std::string result) {
        set_op("new");
        set_arg1(arg1);
        set_arg2(arg2);
        set_result(result);
    }
    virtual std::string to_string() override {
        return get_result() + " := " + get_op() + " " + get_arg1() + ", " + get_arg2() + "\\n";
    }
    virtual tac_type get_tac_type() override {
        return NEW_ARRAY;
    }

    virtual std::string to_bytecode() {
        return "";
    }
};
#endif // TAC_NEW_ARRAY_HH