all: clean
	g++ -O0 -g -o app main.cpp
	g++ -O0 -g -o mlib.so -fPIC -shared lib.cpp

instrument:
	../cobi --filter filter.xml --adapter adapter.xml --use tutcount --bin app --out mutated_count
	../cobi --filter filter.xml --adapter adapter.xml --use tutorial --bin app --out mutated_events

clean:
	rm -f app
	rm -f mlib.so
	rm -f mutated_count
	rm -f mutated_events
