all: nreview

nreview: main.cpp session.cpp
	clang++ main.cpp session.cpp -Wall -std=c++17 -lncurses -o nreview

debug: main.cpp session.cpp
	clang++ main.cpp session.cpp -Wall -std=c++17 -lncurses -g -o nreview
