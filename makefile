all:clean comp run
comp:
	g++ -o app output_vmstat.cpp config/Config.cpp
run:
	./app
.PHONY : clean
clean:
	-rm app 