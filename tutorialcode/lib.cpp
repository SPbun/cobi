#include <iostream>

extern "C" void enterFunction(char* name,char* filename, int linenumber);

extern "C" void exitFunction(char* name, char* filename, int linenumber);

extern "C" void enterFunctionShowCount(char* name, int count);

void enterFunction(char* name,char* filename, int linenumber) {
	std::cout << "enter: " << name << " in " 
		  << filename << "(" << linenumber << ")\n";
}

void exitFunction(char* name, char* filename, int linenumber) {
	std::cout << "exit: " << name << " in " 
		  << filename << "(" << linenumber << ")\n";
}

void enterFunctionShowCount(char* name, int count) {
	std::cout << "enter: " << name 
		  << " count: " << count << "\n";
}
