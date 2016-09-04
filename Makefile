CXXFLAGS  = -Wall  -std=c++11

all: Tracer

test: Test/Test_Math

Tracer: Math.o Ray.o Sphere.o Triangle.o TriangleMesh.o 

Test/Test_Math: Math.o

clean: ;rm Tracer Test/Test_Math *.o *.ppm
