#include <iostream>
#include <sstream>

int loops(int a) {
	int r = 1;
	for(int i = 0; i < a; i++) {
		if(i%3==0) continue;
		for(int j = i; j < a; j++) {
			r = r + j;
		}
		r = r * i;
	}
	return r;
}

int main(int argc, char** argv) {
	int a = (argc+1) * 8;
	std::cout << "Value: " << loops(a);
}
