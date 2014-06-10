#include <iostream>

extern "C" void enterFunction(char* name,char* filename, int linenumber);

extern "C" void exitFunction(char* name, char* filename, int linenumber);

extern "C" void msPrint(char* name, int count);

extern "C" void msPrintStr(char * str);

extern "C" void thrashFloat();

void thrashFloat() {
	double a = 1.0;
	double b = a * 2.0;
	float f1 = 4.0;
	float f2 = f1 * 2.0;
}

void enterFunction(char* name,char* filename, int linenumber) {
	std::cout << "enter: " << name << " in " 
		  << filename << " (" << linenumber << ")\n";
}

void exitFunction(char* name, char* filename, int linenumber) {
	std::cout << "exit: " << name << " in " 
		  << filename << " (" << linenumber << ")\n";
}

void msPrint(char* name, int count) {
	std::cout << name << count << "\n";
}

void msPrintStr(char * str) {
	std::cout << str;
}
