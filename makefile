DEVROOT			=	$(HOME)/dev
DEVMAKE			=	$(DEVROOT)/make

run-parallel   ?=	yes
cflags			=	-I..
target			=	test-convert.exe
sources			=	$(wildcard libs/convert/test/test_convert.cpp)

include $(DEVMAKE)/makefile

