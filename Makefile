main: ./src/main.cpp
	g++ -g ./src/main.cpp -o main -lGL -lglut

clear:
	rm -f main
