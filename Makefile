CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -g
LDFLAGS = -lm
SRCDIR = src
TESTDIR = tests
OBJDIR = obj
BINDIR = bin

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))
TEST_SOURCES = $(wildcard $(TESTDIR)/*.c)
TEST_OBJECTS = $(patsubst $(TESTDIR)/%.c, $(OBJDIR)/%.o, $(TEST_SOURCES))

all: $(BINDIR)/nn

# compile engine.c into engine.o but do not link into an executable
$(OBJDIR)/engine.o: $(SRCDIR)/engine.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# create the nn executable by linking nn.o with engine.o
$(BINDIR)/nn: $(OBJDIR)/nn.o $(OBJDIR)/engine.o
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# compile nn.c into nn.o
$(OBJDIR)/nn.o: $(SRCDIR)/nn.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: clean all
