VPATH=libs:src:src/geometry:src/primitive:src/materials
CXX=g++
CXXFLAGS=-std=c++14 -Ilibs -Isrc -Isrc/geometry -Isrc/primitive -Isrc/materials -MMD -Werror=vla -DNDEBUG
OBJECTS=main.o camera.o film.o matrix.o sphere.o transform.o primitive.o integrator.o sampler.o spectrum.o shape.o triangle.o disney.o material.o bvh.o
DEPENDS=${OBJECTS:.o=.d}
EXEC=main

${EXEC}: ${OBJECTS}
	${CXX} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}
.PHONY: clean

clean:
	rm ${OBJECTS} ${DEPENDS} ${EXEC}
