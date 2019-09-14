run:
	g++ -std=c++11 ./source/main.cpp -w -lGL -lglfw -lGLEW -lGLU -o ./bin/p
	./bin/p
clean:
	rm -f ./bin/p
