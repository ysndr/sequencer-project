# inspired by https://www.cs.swarthmore.edu/~newhall/unixhelp/howto_makefiles.html

CC = gcc
CFLAGS = -Wall -g
CFEATURES =
INCLUDES = -I./include



LD = ld
LFLAGS =
LIBS = -pthread

SRCDIR = ./src
DEPDIR = ./include
OBJDIR = ./build
TARGETDIR = ./target

SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

DEPS = makeargv
DEP_OPJS = $(DEPS:%=$(DEPDIR)/%.o)



MAIN = sequencer

#
# The following part of the makefile is generic; it can be used to
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#


$(TARGETDIR): ; @mkdir -p $@
$(OBJDIR): ; @mkdir -p $@


$(TARGETDIR)/$(MAIN): $(TARGETDIR) $(OBJDIR) $(OBJS) $(DEP_OPJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGETDIR)/$(MAIN) $(OBJS) $(DEP_OPJS) $(LFLAGS) $(LIBS)

# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file)
# (see the gnu make manual section about automatic variables)
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(CFEATURES) $(INCLUDES) -c $<  -o $@

$(DEPDIR)/%.o: $(DEPDIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@



clean:
	$(RM) -r $(OBJDIR) $(TARGETDIR) $(DEP_OPJS)

.PHONY: clean

all: $(TARGETDIR) $(TARGETDIR)/$(MAIN)
	@echo done.
