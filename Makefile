CC := g++
CCF := -std=c++17 -O1
SRC := src/main.cpp src/server.cpp
TARGET := server
main:
	@$(CC) $(CCF) $(SRC) -o $(TARGET)
