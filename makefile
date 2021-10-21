all:clean comp run
comp:
	g++ -o app output_vmstat.cpp Config.cpp
run:
	./app
.PHONY : clean
clean:
	-rm app 