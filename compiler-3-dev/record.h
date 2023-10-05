#ifndef RECORD_H
#define RECORD_H

#include <iostream>
#include <fstream>
#include <vector>
#include <map>

using namespace std;

class Record {
    private:
        string id;
        string type;
        string record;

    public:
	    Record(string nId, string t) { this->type = t; this->id = nId; this->record = "record"; };
        Record() : type("unknown type"), id("unknown id"), record("Record") {};
        ~Record(){};

        string getID() { return this->id; }
        string getType() { return this->type; }
        string getRecord() { return this->record; }

        void setID(string nId) { this->id = nId; }
        void setType(string t) { this->type = t; }
        void setRecord(string r) { this->record = r; }

        virtual vector<string> getParameter() = 0;

        virtual void print() = 0;
};

#endif //RECORD_H