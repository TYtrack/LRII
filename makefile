all:clean comp
comp:
	g++ -g -o app output_vmstat.cpp config/Config.cpp MemoryInfo.cpp IoInfo.cpp CpuInfo.cpp
run:
	./app
.PHONY : clean
clean:
	-rm app 