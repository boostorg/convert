ifneq ($(DEVMAKE),)

cflags  = -O3 -Iinclude -I../boost_1_55_0 -Wno-unused-local-typedefs -Wno-unused-variable -Wno-uninitialized
target  = test-convert.exe
sources = $(wildcard test/*.cpp) $(wildcard example/*.cpp)

include $(DEVMAKE)/makefile

else

all: $(wildcard test/*.cpp) $(wildcard example/*.cpp) 
	g++ $^ -I./include -I../boost_1_55_0

endif

