#ifndef SCOPE_H
#define SCOPE_H

#include "record.h"
#include <list>
#include <map>

using namespace std;

class Scope {
private:
	int next = 0; //next child to visit
	Scope* parent_scope; //parent scope
	vector<Scope*> children_scopes; //children scopes
	map<string, Record*> records; 

public:
	Scope() { records.clear(); parent_scope = nullptr; }
    Scope(Scope *nextChild) {
        records.clear();
        if(nextChild != nullptr){ parent_scope = nextChild; } 
        else{ parent_scope = nullptr; }	
    }
    ~Scope(){ this->children_scopes.clear(); this->records.clear(); }
    
    Scope* get_parent() { return parent_scope; }
    int get_next() { return next; }
	
    Scope* next_child() {
		Scope* next_child;
		if(next == children_scopes.size()) { // create new child Scope
			next_child = new Scope(this);
			children_scopes.push_back(next_child);
		} else { next_child = this->children_scopes[next]; } 
		next++;
		return next_child;
	}

    void put(string Key, Record* item){
        if(records.count(Key) > 0){ 
            return; 
        }
        records.emplace(Key, item); 
    }

	Record* lookups(string Key) {
		// if(this->records.count(Key) > 0) { return this->records.at(Key); }
		// if (this->parent_scope == nullptr) { return nullptr; } 
		// else { this->parent_scope->lookups(Key); }
        if (this->records.count(Key) == 1)
        {
            return this->records.at(Key);
        }
        else
        {
            if (this->parent_scope == nullptr)
            {
                return nullptr;
            }
            else
            {
                return this->parent_scope->lookups(Key);
            }
        }
	}

    void reset_scope() {
		next = 0;
		for (auto i = children_scopes.begin(); i != children_scopes.end(); i++) { 
            (*i)->reset_scope(); 
        }
	}

    void print_scope(){
        cout << "-----------------Scope-------------" << endl;
        cout << "Records: " << endl;
        map<string, Record*>::iterator i;
        for (i = records.begin(); i != records.end(); i++) {
            std::cout << i->first << ':';
            i->second->print();
        }
        cout << "----------------------------------" << endl;
        cout << endl;
        vector<Scope*>::iterator it;
        for(it = children_scopes.begin(); it != children_scopes.end(); it++) {
            (*it)->print_scope();
        }
    }


};
#endif