CXX = g++
CXXFLAGS = -Wall -MMD -DDEBUG_VIEW -DDEBUG_BOARD
EXEC = ss5k
OBJECTS = view/cellviewabstract.o view/textview/textview.o view/view.o control/game.o model/board.o main.o model/square.o
DEPENDS = ${OBJECTS:.o=.d}


${EXEC}: ${OBJECTS}
	 ${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
