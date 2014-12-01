CXX = g++
CXXFLAGS = -lX11 -L/usr/X11/lib -I/usr/X11/include -Wall -std=c++0x -pthread -MMD #-DDEBUG_VIEW #-DDEBUG_GRAPHIC -DDEBUG #-DDEBUG_BOARD #-DMATCH_TEST #-DDEBUG_VIEW
EXEC = ss5k
MODEL = model/board.o model/square.o model/generator.o
VIEW = view/view.o view/viewabstract.o view/textview/textview.o view/graphicview/graphicview.o view/graphicview/window.o view/graphicview/animation.o 
CONTROL = controller/game.o
OTHERS = public/global.o main.o
OBJECTS = ${MODEL} ${VIEW} ${CONTROL} ${OTHERS}
DEPENDS = ${OBJECTS:.o=.d}


${EXEC}: ${OBJECTS}
	 ${CXX} ${OBJECTS} ${CXXFLAGS} -o ${EXEC}
	 cp ${EXEC} tests/${EXEC}

# -include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}

re:
	make clean
	make
