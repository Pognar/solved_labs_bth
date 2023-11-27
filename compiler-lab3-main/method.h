#ifndef METHOD_H
#define METHOD_H

#include "variable.h"
#include <unordered_map>

using namespace std;

class Method : public Record { 
    private:
        unordered_map<string, Variable*> params;
        unordered_map<string, Variable*> variables;

    public:
        Method(string nId, string t){ setID(nId); setType(t); setRecord("Method"); }
        Method() {
            setID("current_method ID uni");
            setType("current_method Type uni");
        }
        ~Method(){}

        void addVariable(string nId, Variable* var) { 
            std::cout << "Adding variable: " << nId << " to method: " << getID() << std::endl;
            variables.emplace(nId, var); };
        void addParameter(string nId, Variable* var) {
            std::cout << "Adding parameter: " << nId << " to method: " << getID() << std::endl;
            params.emplace(nId, var); };
        virtual vector<string> getParameter() override { 
            vector<string> ret;
            unordered_map<string, Variable*>::iterator it;
            for (it = params.begin(); it != params.end(); it++) {
                ret.push_back(it->second->getType());
            }    
            return ret; 
        }; 

        virtual vector<string> get_n_Parameter() override { 
            vector<string> ret;
            unordered_map<string, Variable*>::iterator it;
            for (it = params.begin(); it != params.end(); it++) {
                ret.push_back(it->second->getID());
            }    
            return ret; 
        }; 



        virtual void print() override {
			if (getID() == "") {
				cout << "   name: " << "Not declared" << "| Record: " << getRecord() << endl;
			}
			else {
				cout << "   name: " << getID() << " | Type:  "<< getType() << " | Record: " << getRecord() << endl;
			}
		}
};

#endif //METHOD_H