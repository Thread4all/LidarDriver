# good ol' make
# switch commented out lines to cross platform
# compile and run everything in one go:  g++ -std=c++17 -o main main.cpp LidarDriver.cpp && main

# CC = clang++
CC = g++
FLAGS = -std=c++14

DEST = dest\main.exe
# DEST = dest/main

$(DEST): objs/main.o objs/LidarDriver.o
	$(CC) $(FLAGS) objs/main.o objs/LidarDriver.o -o $(DEST)

objs/main.o: src/main.cpp
	$(CC) $(FLAGS) -c src/main.cpp -o objs/main.o

objs/LidarDriver.o: src/LidarDriver.cpp include/LidarDriver.h
	$(CC) $(FLAGS) -c src/LidarDriver.cpp -o objs/LidarDriver.o

clean:
	del objs\*.o
	del $(DEST)
# rm -f objs\*.o
# rm -f $(DEST)

run:
	$(DEST)