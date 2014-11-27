CXX = g++
CXXFLAGS = -Wall -lX11 -L/usr/X11/lib -I/usr/X11/include -MMD -DDEBUG_GRAPHIC #-DDEBUG #-DDEBUG_BOARD #-DMATCH_TEST #-DDEBUG_VIEW
EXEC = main
MODEL = model/board.o model/square.o
VIEW = view/view.o view/textview/textview.o view/graphicview/graphicview.o view/graphicview/window.o
CONTROL = controller/game.o
OTHERS = public/global.o main.o
OBJECTS = ${MODEL} ${VIEW} ${CONTROL} ${OTHERS}
DEPENDS = ${OBJECTS:.o=.d}


${EXEC}: ${OBJECTS}
	 ${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}
	 cp ${EXEC} tests/${EXEC}

# -include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
