ifneq ($(DEVMAKE),)

cflags			=	-I../include
target			=	test-convert.exe
sources			=	$(wildcard *.cpp) $(wildcard ../example/*.cpp)

include $(DEVMAKE)/makefile

else	

all: $(wildcard *.cpp) $(wildcard ../example/*.cpp) 
	g++ $^ -I../include -I../../boost_1_55_0

endif

