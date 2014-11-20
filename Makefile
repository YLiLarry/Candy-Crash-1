ss5k: main.o game.o board.o textview.o cellview.o
	g++ main.o game.o board.o textview.o cellview.o -o ss5k

main.o: main.cc
	g++ -c main.cc

game.o: control/game.cc control/game.h
	g++ -c control/game.cc

board.o: model/board.cc model/board.h
	g++ -c model/board.cc

textview.o: view/textview/textview.cc view/textview/textview.h
	g++ -c view/textview/textview.cc

cellview.o: view/cellview.cc view/cellview.h
	g++ -c view/cellview.cc
	
