CXX = g++
CXXFLAGS = -lX11 -std=c++11 -L/usr/X11/lib -I/usr/X11/include -Wall -MMD -DDEBUG
EXEC = test/test
OBJECTS = test/test.o textview/textview.o graphicview/graphicview.o graphicview/window.o view.o ../public/global.o 
DEPENDS = ${OBJECTS:.o=.d}


${EXEC}: ${OBJECTS}
	 ${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
