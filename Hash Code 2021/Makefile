CXX       := g++
CXX_FLAGS := -std=c++17 -ggdb

BIN     := bin
SRC     := src
INCLUDE := include
IO      := io

LIBRARIES   :=
EXECUTABLE  := main


all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE) run $(ARGS) | tee $(IO)/$(ARGS)

score: clean all
	clear
	./$(BIN)/$(EXECUTABLE) run a.txt | tee $(IO)/a.txt
	./$(BIN)/$(EXECUTABLE) run b.txt | tee $(IO)/b.txt
	./$(BIN)/$(EXECUTABLE) run c.txt | tee $(IO)/c.txt
	./$(BIN)/$(EXECUTABLE) run d.txt | tee $(IO)/d.txt
	./$(BIN)/$(EXECUTABLE) run e.txt | tee $(IO)/e.txt
	./$(BIN)/$(EXECUTABLE) run f.txt | tee $(IO)/f.txt

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) -std=c++14 -I$(INCLUDE) $^ -o $@ $(LIBRARIES)

clean:
	-rm $(BIN)/*


# run: build
# 	bin/tooObv $(ARGS) | tee io/outputf.in

# build:
# 	g++ *.cpp -o bin/tooObv
