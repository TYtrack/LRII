all:clean comp run
comp:
	g++ -o app output_vmstat.c
run:
	./app
.PHONY : clean
clean:
	-rm app 