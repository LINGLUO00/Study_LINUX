SRC=$(wildcard *.cpp)
OBJ=$(patsubst %.cpp, %.o, $(SRC))

ProcessPool:$(OBJ)
	g++ -o $@ $^ -std=c++11


$(OBJ):$(SRC)
	g++ -c $^ -o $@ -std=c++11

.PHONY:clean
clean:
	rm -f ProcessPool *.o

