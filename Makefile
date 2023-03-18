CXX=g++
CXXFLAGS=-std=c++14 -MMD -Werror=vla
OBJECTS=main.o camera.o film.o matrix.o sphere.o transform.o primitive.o integrator.o sampler.o spectrum.o
DEPENDS=${OBJECTS:.o=.d}
EXEC=main

${EXEC}: ${OBJECTS}
	${CXX} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}
.PHONY: clean

clean:
	rm ${OBJECTS} ${DEPENDS} ${EXEC}
