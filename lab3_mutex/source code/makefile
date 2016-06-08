SOURCE=main.cpp lock.cpp autolock.cpp stacklist.cpp

OBJECTS=main.o lock.o autolock.o stacklist.o

TARGET=threadpro

CC=g++

LIBS=./

OPT=-l pthread

CFLAG=-g -Wall -w

$(TARGET):$(OBJECTS)
	$(CC) $(CFLAG) $(OBJECTS) -L$(LIBS) -o $(TARGET) $(OPT)

$(OBJECTS):$(SOURCE)
	$(CC) $(CFLAG) -c $(SOURCE) 

clean:
	rm -rf $(OBJECTS) $(TARGET) *.log
