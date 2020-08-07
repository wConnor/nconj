all: nconj

nconj: main.cpp session.cpp menu.cpp
	clang++ main.cpp session.cpp menu.cpp -Wall -std=c++17 -lncurses -o nconj

debug: main.cpp session.cpp menu.cpp
	clang++ main.cpp session.cpp menu.cpp -Wall -std=c++17 -lncurses -g -o nconj
