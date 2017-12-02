BIN_NAME = eit
CC  = g++
LD  = g++


INCLUDE = -I./
CPP_FLAGS := -Wall -Wextra -std=c++0x -c $(INCLUDE)
LD_FLAGS := -Wall -Wextra -lgsl -lblas
LIB_FLAGS := -lgsl -lblas -lps

CPP_OBJS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))

$(BIN_NAME): $(CPP_OBJS)
	@echo LINKING...
	@echo $(CPP_OBJS)
	@$(CC) $(LD_FLAGS) $(CPP_OBJS) $(LIB_FLAGS) -o $@ 

$(CPP_OBJS): %.o: %.cpp
	@echo CREATING $@ FROM $<
	@$(CC) $(CPP_FLAGS) $< -o $@	
all: $(BIN_NAME)
	
.PHONY:	clean

clean:
	rm *.o

