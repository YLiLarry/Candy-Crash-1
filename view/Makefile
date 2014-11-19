CXX = g++
CXXFLAGS = -Wall -MMD -DDEBUG_VIEW
EXEC = test/test
OBJECTS = test/test.o cellview.o textview/textview.o
DEPENDS = ${OBJECTS:.o=.d}


${EXEC}: ${OBJECTS}
	 ${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
