all:
	g++ src/main.cpp src/graph.cpp src/utils.cpp -o main -std=c++11
clean:
	rm main
