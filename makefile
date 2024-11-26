# good ol' make
# switch commented out lines to cross platform
# compile and run everything in one go:  g++ -std=c++17 -o main main.cpp LidarDriver.cpp && main

# CC = clang++
CC = g++
FLAGS = -std=c++14

OBJS = objs
DEST = dest\main.exe
# DEST = dest/main

$(DEST): $(OBJS)/main.o $(OBJS)/LidarDriver.o
	$(CC) $(FLAGS) $(OBJS)/main.o $(OBJS)/LidarDriver.o -o $(DEST)

$(OBJS)/main.o: main.cpp
	$(CC) $(FLAGS) -c main.cpp -o $(OBJS)/main.o

$(OBJS)/LidarDriver.o: LidarDriver.cpp include/LidarDriver.h
	$(CC) $(FLAGS) -c LidarDriver.cpp -o $(OBJS)/LidarDriver.o

clean:
	del $(OBJS)\*.o
	del $(DEST)
# rm -f $(OBJS)\*.o
# rm -f $(DEST)

run:
	$(DEST)