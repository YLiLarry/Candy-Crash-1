CXX = g++
CXXFLAGS = -lX11 -L/usr/X11/lib -I/usr/X11/include -Wall -std=c++0x -MMD -DDEBUG_GRAPHIC #-DDEBUG
EXEC = test/test
OBJECTS = test/test.o textview/textview.o graphicview/graphicview.o graphicview/animation.o graphicview/window.o view.o ../public/global.o 
DEPENDS = ${OBJECTS:.o=.d}


${EXEC}: ${OBJECTS}
	 ${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
	
re:
	make clean
	make
	test/test
