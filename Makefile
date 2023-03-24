VPATH=libs:src:src/geometry
CXX=g++
CXXFLAGS=-std=c++14 -Ilibs -Isrc -Isrc/geometry -MMD -Werror=vla -DNDEBUG
OBJECTS=main.o camera.o film.o matrix.o sphere.o transform.o primitive.o integrator.o sampler.o spectrum.o shape.o triangle.o
DEPENDS=${OBJECTS:.o=.d}
EXEC=main

${EXEC}: ${OBJECTS}
	${CXX} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}
.PHONY: clean

clean:
	rm ${OBJECTS} ${DEPENDS} ${EXEC}
