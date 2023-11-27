#ifndef TAC_EXIT_HH
#define TAC_EXIT_HH

#include "../tac.hh"

class tac_exit : public Tac {
    public:
    tac_exit(){
        set_result("exit");
    }

    virtual std::string to_string() override {
        return get_result();
    }
    virtual tac_type get_tac_type() override {
        return EXIT;
    }

    virtual std::string to_bytecode() {
        std::string bytecode = "";
        bytecode += "exit\n";
        return bytecode;
    }
};
#endif // TAC_EXIT_HH