#ifndef METHOD_H
#define METHOD_H

#include "variable.h"

using namespace std;

class Method : public Record { 
    private:
        map<string, Variable*> params;
        map<string, Variable*> variables;

    public:
        Method(string nId, string t){ setID(nId); setType(t); setRecord("Method"); }
        Method() {
            setID("method ID uni");
            setType("method Type uni");
        }
        ~Method(){}

        void addVariable(string nId, Variable* var) { variables.emplace(nId, var); };
        void addParameter(string nId, Variable* var) { params.emplace(nId, var); };
        virtual vector<string> getParameter() override { 
            vector<string> ret;
            map<string, Variable*>::iterator it;
            for (it = params.begin(); it != params.end(); it++) {
                ret.push_back(it->second->getType());
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