bin/ExamenPar2: src/ExamenPar2.cpp src/AnimationController.hpp src/Characters.hpp
	@mkdir -p bin
	c++ -I/usr/local/include -L/usr/local/lib src/ExamenPar2.cpp -o bin/ExamenPar2 -lftxui-screen -lftxui-dom -lftxui-component

runExamenPar2: bin/ExamenPar2
	./bin/ExamenPar2
