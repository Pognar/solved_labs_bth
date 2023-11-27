#ifndef TAC_HH
#define TAC_HH

#include <string>
#include <iostream>

enum tac_type {
    EXPRESSION,
    COPY,
    UNERY_EXPRESSION,
    RETURN,
    PRINT,
    PARAMETER,
    NEW_ARRAY,
    NEW,
    METHOD_CALL,
    LENGTH,
    JUMP,
    COND_JUMP,
    ARRAY_ACCESS,
    EXIT
};

class Tac{
    public:
        Tac(){
            op = "op";
            arg1 = "lhs";
            arg2 = "rhs";
            result = "result";
        }

        virtual std::string to_string() = 0;
        virtual std::string to_bytecode() = 0;
        virtual tac_type get_tac_type() = 0;

        std::string get_op(){return op;}
        std::string get_arg1(){return arg1;}
        std::string get_arg2(){return arg2;}
        std::string get_result(){return result;}
        void set_op(std::string op){this->op = op;}
        void set_arg1(std::string arg1){this->arg1 = arg1;}
        void set_arg2(std::string arg2){this->arg2 = arg2;}
        void set_result(std::string result){this->result = result;}

        bool is_num(std::string str)
        {
            for (char c : str)
            {
                if (!isdigit(c))
                {
                    return false;
                }
            }
            return true;
        }
    private:
        std::string op, arg1, arg2, result;

};

#endif // TAC_HH