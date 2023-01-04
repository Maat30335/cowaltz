CXX=g++
CXXFLAGS=-std=c++14 -MMD -Werror=vla
OBJECTS=main.o Board.o King.o Knight.o Pawn.o Piece.o SlidingPieces.o Game.o Player.o Human.o Move.o Computer.o
DEPENDS=${OBJECTS:.o=.d}
EXEC=chess

${EXEC}: ${OBJECTS}
	${CXX} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}
.PHONY: clean

clean:
	rm ${OBJECTS} ${DEPENDS} ${EXEC}
