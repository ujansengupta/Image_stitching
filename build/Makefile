CC = g++

INC = -I../hdr -I/home/uno/Linux_ifs_9.0/hdr

CFLAGS = -Wall -Wextra
CFLAGS += -D_cplusplus
CFLAGS += $(INC)

LIBDIR = $(IFSHOME)/lib
BINDIR = ../bin
SRCDIR = ../src
OBJDIR = ../obj

SOURCES=$(wildcard ../src/*.cpp)
OBJECTS =$(subst src,obj,$(SOURCES:.cpp=.o))

LIB = $(LIBDIR)/libifs.a
BIN = $(BINDIR)/main.out


all: $(BIN)

$(BIN): $(LIB) $(OBJECTS)
	@echo $(info [${OBJECTS}])
	$(CC) -g $(CFLAGS) -o $(BIN) $(OBJECTS) $(LIB)

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	$(CC) -g $(CFLAGS) -c $< -o $@

clean:
	rm -f $(BIN) ../src/*.o ../obj/*.o ../output/*.ifs
