all:
	g++ src/main.cpp src/graph.cpp src/utils.cpp -o escalona -std=c++11
clean:
	rm escalona
