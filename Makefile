appname := $(shell basename `pwd`).out

CXX := g++
CXXFLAGS := -std=c++11

srcfiles := $(shell find . -name "*.cpp" -not -path "./exercise/*") 
objects  := $(patsubst %.cpp, %.o, $(srcfiles))
LDFLAGS := -Llib/
LDLIBS := -lnanogui -lGL -lGLEW
INC := -Iinclude/

all: $(appname)
	$(shell LD_LIBRARY_PATH=../lib)
	export LD_LIBRARY_PATH=lib/ ; ./$(appname) || true
	
$(appname): $(objects)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -g -o $(appname) $(objects) $(LDFLAGS) $(LDLIBS) $(INC)

%.o: %.cpp
	@echo  $(value objects)
	
	$(CXX) -g -c  $(CXXFLAGS) $(INC) $< -o $@

depend: .depend

.depend: $(srcfiles)
	rm -f ./.depend
	$(CXX) $(CXXFLAGS) $(INC) -MM $^>>./.depend;

clean:
	rm -f $(objects) || true
	rm -f $(appname) || true
#in case we changed the base directory name - clean also all .out files
	rm -f *.out || true

dist-clean: clean
	rm -f *~ .depend

include .depend