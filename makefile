all:
	g++  *ClientSide/*.cpp *Common/*.cpp *Common/Factories/*.cpp *SearchAlgo/*.cpp -std=c++0x -pthread -lm -lboost_serialization -o client.out

	g++  *ServerSide/*.cpp *Common/*.cpp *Common/Factories/*.cpp *SearchAlgo/*.cpp *Multithreading/*.cpp *Multithreading/Tasks/*.cpp -std=c++0x -pthread -lm -lboost_serialization -o server.out
