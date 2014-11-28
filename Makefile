CXX = g++
CXXFLAGS = -Wall -MMD  -DDEBUG -std=c++0x #-DDEBUG_BOARD #-DMATCH_TEST #-DDEBUG_VIEW
EXEC = main
OBJECTS = public/global.o view/cellviewabstract.o view/textview/textview.o view/view.o controller/game.o model/board.o model/square.o model/generator.o main.o
DEPENDS = ${OBJECTS:.o=.d}


${EXEC}: ${OBJECTS}
	 ${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}
	 cp ${EXEC} tests/${EXEC}

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
