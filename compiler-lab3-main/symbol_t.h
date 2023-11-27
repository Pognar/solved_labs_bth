#ifndef SYMBOL_T_H
#define SYMBOL_T_H

#include "scope.h"

using namespace std;

class SymbolTable {
private:
	Scope root;
	Scope *current;
	
public:
    SymbolTable() { root = new Scope(nullptr); current = &root; }
    ~SymbolTable() { this->current = nullptr; }

	void enterScope() { current = current->next_child(); }
	void exitScope() { current = current->get_parent(); }

	void put(string Key, Record* item) { 
		current->put(Key, item); 
	}

	Scope* getParent() { current->get_parent(); }

	void printTable() { root.print_scope(); }
	void resetTable() { 
		root.reset_scope(); 
		current = &root;  
	}
	void printCurr() { current->print_scope(); }

	Record* lookup(string Key) { return current->lookups(Key); }
	Record* looky(string Key) { return root.print_kids_per_method(Key); }

};

#endif