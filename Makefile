#compiler
CC = g++

#compile flags
CFLAGS = -Wall -g

#includes
INCLUDES = -I/usr/include/Awesomium -I/usr/include/SDL2 -I/usr/include/jconfig

#libraries
LIBS = -lSDL2 -lawesomium-1-7 -ljconfig
#LFLAGS = -Ldependencies/bin/libawesomium-1-7.so 

#sources
SRCS = src/main.cpp src/startup.cpp src/util.cpp src/mirror.cpp src/view.cpp src/timer.cpp

#C object files
OBJS = $(SRCS:.c=.o)

#executable
MAIN = jmirror

#main build routine

.PHONY: depend clean

all: $(MAIN)
	 @echo Successfully compiled jMirror

$(MAIN): $(OBJS)
		$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LIBS) #$(LFLAGS)

#build .o's from .c's

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) *.o *~ $(MAIN)

depend: $(SRCS)
		makedepend $(INCLUDES) $^
