all:clean client serve
client:
	g++ -g -o app output_vmstat.cpp config/Config.cpp MemoryInfo.cpp IoInfo.cpp CpuInfo.cpp NetInfo.cpp
serve:
	g++ -g -o serve lrii_serve.cpp
run:
	./app
.PHONY : clean
clean:
	-rm app 