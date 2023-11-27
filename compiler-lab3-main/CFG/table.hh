#ifndef CFG_TABLE_HH
#define CFG_TABLE_HH

#include "basic-block.hh"

#include "tacs/tac_access_array.hh"
#include "tacs/tac_conditional_jump.hh"
#include "tacs/tac_copy.hh"
#include "tacs/tac_exit.hh"
#include "tacs/tac_expression.hh"
#include "tacs/tac_jump.hh"
#include "tacs/tac_length.hh"
#include "tacs/tac_call_method.hh"
#include "tacs/tac_new.hh"
#include "tacs/tac_new_array.hh"
#include "tacs/tac_parameter.hh"
#include "tacs/tac_print.hh"
#include "tacs/tac_return.hh"
#include "tacs/tac_unary_expression.hh"
#include "tac.hh"
#include "../symbol_t.h"

#include <list>
#include <string>
#include <vector>
#include <iostream>

class cfg_table
{

    public:
        cfg_table()
        {
            std::string sblock = "block_" + std::to_string(this->current_id);
            std::string init_code = "";
            this->id = 1;
            basic_block *bb = new basic_block();
            bb->set_label(sblock);
            bb->set_code(init_code);

            blocks.push_back(bb);
            this->current = bb;
        }

        void new_block()
        {
            std::string init_code = "";
            basic_block *bb = new basic_block();
            this->current = bb;
            std::string tmp_label = "block_" + std::to_string(this->id);
            this->current->set_label(tmp_label);
            this->current->set_code(init_code);
            this->id = this->id + 1;
        }

        void set_class_name(std::string input) { this->class_name = input; }
        std::string get_class_name() { return this->class_name; }

        void set_current(basic_block *bb) { this->current = bb; }

        basic_block *get_block(int id)
        {
            int i = 0;
            for (auto &block : blocks)
            {
                if (i == id)
                {
                    return block;
                }
                i++;
            }
            return nullptr;
        }

        basic_block *create_block()
        {
            this->current_id++;
            basic_block *bb = new basic_block(this->current_id);
            blocks.push_back(bb);
            this->current = bb;
            return bb;
        }

        basic_block *get_current() { return this->current; }

        basic_block *create_entry()
        {
            std::string sblock = "block_0";
            std::string init_code = "";
            this->id = 1;
            basic_block *bb = new basic_block();
            bb->set_label(sblock);
            bb->set_code(init_code);
            blocks.push_back(bb);
            this->current = bb;
            return bb;
        }

        basic_block *create_exit(bool boole)
        {
            return this->current;
        }

        void insert_code(std::string code)
        {
            this->current->set_code(code);
        }

        list<basic_block *> get_blocks() { return this->blocks; }

        std::string get_code()
        {
            return current->get_code();
        }



        void add_tac(Tac *newInstructions)
        {
            this->current->add_instruction(*newInstructions);
        }

        void generate_cfg(std::ofstream &file)
        {
            for (auto &block : blocks)
            {
                block->generate_cfg(file);
            }
        }

        std::string set_unique_name()
        {
            return "_t" + std::to_string(this->uniqueNameNum++);
        }

        string to_string()
        {
            std::string s = "";
            for (auto &block : blocks)
            {
                s += block->to_string();
                s += "\n";
            }
            return s;
        }
    //------------ BYTE CODE RELATED ------------//

        std::vector<string> split(std::string str, char delimiter) {
            std::vector<string> internal;

            int i = 0;
            std::string tmp = "";
            while (i < str.length()) {
                if (str[i] == delimiter) {
                    internal.push_back(tmp);
                    tmp = "";
                } else {
                    tmp += str[i];
                }
                i++;
            }
            internal.push_back(tmp);
            return internal;
        }

        void generate_bytecode(SymbolTable *st){
            std::cout << "GENERATING BYTECODE" << std::endl;
            std::cout << "_____________________________________" << std::endl;
            std::ofstream bytecode_file;
            std::string temp_class_name = "";
            std::string temp_method_name = "";

            bytecode_file.open("bytecode");
            int pc = 0;
            for(auto i = blocks.begin(); i != blocks.end(); i++){
                std::string t1 = "";
                std::string t2 = "";
                if((*i)->get_method() != ""){
                    bytecode_file << (*i)->get_method() << std::endl;

                    std::string classDotMethod = (*i)->get_method();
                    std::vector<std::string> tokens = split(classDotMethod, '.');
                    //std::cout << "Method: " << (*i)->get_method() << " " << tokens[1] << std::endl;
                    if (tokens[1] == "main") {
                       Record* r = st->looky(tokens[0]);
                    }
                    else
                    {
                        Record* r = st->looky(tokens[1]);
                        std::vector<std::string> params = r->get_n_Parameter();
                        for (int it = 0; it < params.size(); it++) {
                            std::string str = "istore " + params[it];
                            //std::cout << str << std::endl;
                            bytecode_file << str << std::endl;
                        }
                    }
                }
                (bytecode_file) << (*i)->get_label() << std::endl;
                std::cout << (*i)->to_string() << std::endl;
                list <Tac*> tac_instructions = (*i)->get_instructions();
                for(auto instruction = tac_instructions.begin(); instruction != tac_instructions.end(); instruction++) {
                    bytecode_file << (*instruction)->to_bytecode();
                    
                }
            }        
            bytecode_file.close();
        }


    private:
        std::list<basic_block *> blocks;
        int id;
        int uniqueNameNum = 0;
        basic_block *current;
        int block_number = 0;
        int current_id = 0;
        std::string class_name = "";
};

#endif // CFG_TABLE_H
