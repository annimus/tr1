CC = g++

SDIR = src
HDIR = headers
ODIR = obj

_OBJ = 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

CFLAGS = -I$(HDIR) -Wall -Wextra -std=c++11
LIBS =

all: T1

debug: CFLAGS += -g
debug: all

$(ODIR)/%.o: $(SDIR)/%.cpp
	@mkdir -p $(@D)
	$(CC) -c -o $@ $< $(CFLAGS)

T1: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean
clean:
	-rm -r $(ODIR) T1

