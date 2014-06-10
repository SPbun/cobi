CXX=g++
CC=gcc
FLAGS=-O0 -g -Wl,-rpath,.
SHAREDFLAGS=-fPIC -shared

all: clean
	$(CXX) $(FLAGS) -o count main.cpp
	$(CXX) $(FLAGS) $(SHAREDFLAGS) -o mlib.so lib.cpp
	$(CC) $(FLAGS) -o param param.c
	$(CC) $(FLAGS) -o thrash thrash.c

	$(CXX) -g -O2 -Wl,-rpath,. -o loops loops.cpp

instrument:
	../cobi -f filter.xml -a adapter.xml --use totalcount,count -b count --out m_count
	../cobi -f filter.xml -a adapter.xml --use param -b param --out m_param
	../cobi -f filter.xml -a adapter.xml --use thrash -b thrash --out m_thrash
	../cobi -f filter.xml -a adapter.xml --use loops -b loops --out m_loops
	../cobi -f filter.xml -a adapter.xml --use loops -b loops --out m_loops_report --report report_loops.html
	../cobi -f filter.xml -a adapter.xml --use loopsone -b loops --out m_loops_one
	../cobi -f filter.xml -a adapter.xml --use callsites -b count --out m_callsites
	../cobi -f filter.xml -a adapter.xml --use regex -b count --out m_regex
clean:
	rm -f mlib.so
	rm -f count
	rm -f param
	rm -f thrash
	rm -f loops
	rm -f report_*
	rm -f *.o
	rm -f m_*
