CXX = g++
CXXFLAGS = -Wall -MMD -DDEBUG -DDEBUG_BOARD
EXEC = test
OBJECTS = test.o ../controller/game.o ../model/board.o ../model/square.o ../view/view.o ../view/cellviewabstract.o ../view/textview/textview.o 
DEPENDS = ${OBJECTS:.o=.d}


${EXEC}: ${OBJECTS}
	 ${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
