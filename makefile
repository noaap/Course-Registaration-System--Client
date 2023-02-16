CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude
LDFLAGS:=-lboost_system -lboost_thread -pthread


all: bin/BGRSclient


bin/BGRSclient: bin/connectionHandler.o bin/KeyBoardReader.o bin/ServerReader.o bin/BGRSclient.o
	@echo 'Building target: BGRSclient'
	@echo 'Invoking: C++ Linker'
	g++ -o bin/BGRSclient bin/connectionHandler.o bin/KeyBoardReader.o bin/ServerReader.o bin/BGRSclient.o $(LDFLAGS)
	@echo 'Finished building target: BGRSclient'
	@echo ''



bin/connectionHandler.o: src/connectionHandler.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/connectionHandler.o src/connectionHandler.cpp

bin/KeyBoardReader.o: src/KeyBoardReader.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/KeyBoardReader.o src/KeyBoardReader.cpp


bin/ServerReader.o: src/ServerReader.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/ServerReader.o src/ServerReader.cpp

bin/BGRSclient.o: src/BGRSclient.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/BGRSclient.o src/BGRSclient.cpp


#Clean the build directory
clean:
	rm -f bin/*
