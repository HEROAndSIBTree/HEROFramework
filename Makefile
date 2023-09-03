CXX = g++
CPPFLAGS = -std=c++11 -O3 -march=native
RM = rm -f
OBJS = graph.o SIBTree.o util.o reorder.o tc.o mc.o sl.o

all: $(OBJS)
	$(CXX) $(CPPFLAGS) -o reorder reorder.o graph.o util.o
	$(CXX) $(CPPFLAGS) -o tc tc.o graph.o SIBTree.o
	$(CXX) $(CPPFLAGS) -o mc mc.o graph.o SIBTree.o
	$(CXX) $(CPPFLAGS) -o sl sl.o graph.o SIBTree.o

%.o: %.cpp
	$(CXX) $(CPPFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) reorder tc mc sl

