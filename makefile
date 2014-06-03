ifneq ($(DEVMAKE),)

cflags			=	-Iinclude
target			=	test-convert.exe
sources			=	$(wildcard test/*.cpp) $(wildcard example/*.cpp)

include $(DEVMAKE)/makefile

else	

all: $(wildcard test/*.cpp) $(wildcard example/*.cpp) 
	g++ $^ -I./include -I../boost_1_55_0

endif

