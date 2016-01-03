all: mem_leak.so

mem_leak.so:src/info_lookup.cpp src/overload.cpp 
	g++ -shared -fPIC src/overload.cpp src/info_lookup.cpp -o mem_leak.so -lpthread -ldl -std=c++11

clean:
	rm -rf mem_leak.so
