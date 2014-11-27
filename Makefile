CXX = g++
CXXFLAGS = -Wall -MMD -DDEBUG #-DDEBUG_BOARD #-DMATCH_TEST #-DDEBUG_VIEW
EXEC = main
MODEL = model/board.o model/square.o
VIEW = view/view.o view/textview/textview.o view/cellviewabstract.o
CONTROL = controller/game.o
OTHERS = public/global.o main.o
OBJECTS = ${MODEL} ${VIEW} ${CONTROL} ${OTHERS}
DEPENDS = ${OBJECTS:.o=.d}


${EXEC}: ${OBJECTS}
	 ${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}
	 cp ${EXEC} tests/${EXEC}
	 ./tests/runSuite

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
