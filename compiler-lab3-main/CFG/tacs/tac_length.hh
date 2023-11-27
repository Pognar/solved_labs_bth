#ifndef TAC_LENGTH_HH
#define TAC_LENGTH_HH

#include "../tac.hh"

class tac_length : public Tac{
    public:
    tac_length(std::string array, std::string result){
        set_op("length");
        set_arg1(array);
        set_result(result);
    }

    virtual std::string to_string() override {
        return get_result() + " := " + get_op() + " " + get_arg1() + "\\n";
    }
    virtual tac_type get_tac_type() override {
        return LENGTH;
    }

    virtual std::string to_bytecode() {
        return "";
    }
};

#endif // TAC_LENGTH_HH