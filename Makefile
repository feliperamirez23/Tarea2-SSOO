#makefile tarea2

all:
	g++ -o run tarea.cpp -lpthread
	./run archivo_listado_ips.txt 4
build:
	g++ -o run tarea.cpp -lpthread
run:
	./run archivo_listado_ips.txt 4
clean:
	rm run
