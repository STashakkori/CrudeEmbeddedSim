make:
	g++ -w -o sim sim.cpp cmd.cpp global.cpp

clean:
	rm sim
