#ifndef VARIABLE_H
#define VARIABLE_H

#include "record.h"
#include <unordered_map>

using namespace std;

class Variable : public Record {
	private:

	public:
		Variable(string nId, string t) { 
			setID(nId); 
			setType(t); 
			setRecord("Variable");
		}
		Variable() { 
			setID("Variable ID uni"); 
			setType("Variable Type uni"); 
			setRecord("Record");
		}
		~Variable() {}

		virtual vector<string> getParameter() override { 
            vector<string> ret;  
            return ret; 
        };
        virtual vector<string> get_n_Parameter() override { 
            vector<string> ret;  
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
#endif