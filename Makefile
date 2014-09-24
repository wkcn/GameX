CXX = g++
TARGET = GameX
ARCH := $(shell uname -m)
ALLEGRO_LIBS := $(shell allegro-config --libs 2>/dev/null)
CWD := $(shell pwd)
CODE_DIR := $(shell dirname "$(CWD)")
LIBGAMEX = GameXLib.a

INCLUDE = 							\
	-I.							\
	-ILibraries/glfw-3.0.3/include				\
	-ILibraries/FreeImage/Dist/				\
	-I/usr/include/freetype2				\
	-ILibraries/FMOD/inc					\
	-ILibraries/FMOD/inc

LIBS = 									\
	$(LIBGAME)							\
	Libraries/glfw-3.0.3/src/libglfw3.a				

ifneq ($(ANGEL_DISABLE_FMOD),1)
	ifeq ($(ARCH),x86_64)
		LIBS += Libraries/FMOD/lib/libfmodex64.so
	else
		LIBS += Libraries/FMOD/lib/libfmodex.so
	endif
endif

SHLIBS = -lGL -lGLU -lfreetype -lXrandr -lX11 -lpthread -lrt -lXxf86vm -lXi -lfreeimage
SHLIBS += $(ALLEGRO_LIBS)


SRCS =								\
	Defines.cpp						\
	GameX.cpp						\
	GraphEngine.cpp						\
	SoundEngine.cpp						\
	main.cpp						\
	test.cpp

SYSOBJS = $(patsubst %.cpp,%.o,$(SYSSRCS))
OBJS = $(patsubst %.cpp,%.o,$(SRCS))

%.o: %.cpp
	$(CXX) -c $(INCLUDE) -Wno-write-strings -Wno-deprecated -o $@ $^

all: $(TARGET)



$(TARGET): $(LIBANGEL) $(OBJS) $(SYSOBJS)
	$(CXX) -o $@ $(OBJS) $(SYSOBJS) $(LIBS) $(SHLIBS)

clean:
	rm -f $(OBJS) $(SYSOBJS) $(TARGET)	
