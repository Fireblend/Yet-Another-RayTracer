LDLIBS= -lm -g
CHECK=-Wall -Wextra

rayTracer: rayTracer.o sphere.o loader.o polygon.o cylinder.o cone.o quadratic.o structs.o disc.o
	cc $(CHECK) $(LDLIBS) -o $@ rayTracer.o sphere.o loader.o polygon.o cylinder.o cone.o quadratic.o structs.o disc.o
	
rayTracer.o: rayTracer.c
	cc -c -g -o $@ $<
	
disc.o: disc.c
	cc -c -g -o $@ $<

sphere.o: sphere.c
	cc -c -g -o $@ $<

polygon.o: polygon.c
	cc -c -g -o $@ $<

cylinder.o: cylinder.c
	cc -c -g -o $@ $<

cone.o: cone.c
	cc -c -g -o $@ $<

quadratic.o: quadratic.c
	cc -c -g -o $@ $<

loader.o: loader.c
	cc -c -g -o $@ $<
	
structs.o: structs.c
	cc -c -g -o $@ $<

clean:
	rm -f rayTracer rayTracer.o sphere.o loader.o polygon.o cylinder.o cone.o quadratic.o structs.o disc.o
	
compile:
	rm -f rayTracer rayTracer.o sphere.o loader.o polygon.o cylinder.o cone.o quadratic.o structs.o disc.o

	cc -c -o rayTracer.o rayTracer.c
	cc -c -o sphere.o sphere.c 
	cc -c -o loader.o loader.c
	cc -c -o polygon.o polygon.c
	cc -c -o cylinder.o cylinder.c
	cc -c -o cone.o cone.c
	cc -c -o quadratic.o quadratic.c
	cc -c -o structs.o structs.c
	cc -c -o disc.o disc.c

	cc $(CHECK) $(LDLIBS) -o $@ rayTracer.o sphere.o loader.o polygon.o cylinder.o cone.o quadratic.o structs.o disc.o

gdb: rayTracer.o sphere.o polygon.o cylinder.o cone.o quadratic.o structs.o disc.o
	cc $(CHECK) $(LDLIBS) -o $@ rayTracer.o sphere.o loader.o polygon.o cylinder.o cone.o quadratic.o structs.o disc.o
	gdb ./rayTracer
