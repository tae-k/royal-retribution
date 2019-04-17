C++ = g++
CXXFLAGS=-std=c++11 -Wall -Wextra -O2 -D_DEFAULT_SOURCE -g
CXXFLAGS+=`pkg-config --cflags sdl2 SDL2_ttf SDL2_image SDL2_mixer`

INCLUDE_DIR = ./include
SRC_DIR = ./src
OBJS_DIR = ./obj
BIN_DIR = ./bin

LDLIBS=`pkg-config --libs sdl2 SDL2_ttf SDL2_image SDL2_mixer` -lm -lstdc++

objects := $(patsubst $(SRC_DIR)%, $(OBJS_DIR)%, $(patsubst %.cpp, %.o, $(wildcard $(SRC_DIR)/*.cpp)))

$(BIN_DIR)/game: $(objects)
	$(C++) $(objects) $(LDLIBS) -o $@
 
$(OBJS_DIR)/%.o: src/%.cpp
	$(C++) $(CXXFLAGS) -I$(INCLUDE_DIR) -c -o $@ $<

.PHONY: clean
clean:
	rm -rf $(BIN_DIR)/* $(OBJS_DIR)/* data/* loading_times
