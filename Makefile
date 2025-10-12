CC := g++
CC_FLAGS := -std=c++17 -O1
TARGET := server
SOURCE_FILES := src/main.cpp src/server.cpp

OBJS := $(SOURCE_FILES:.cpp=.o)

%.o : %.cpp
	$(CC) $(CC_FLAGS) -c $< -o $@

$TARGET: $(OBJS)
	$(CC) $(CC_FLAGS) $(OBJS) -o $(TARGET)
	@rm -f $(OBJS)
