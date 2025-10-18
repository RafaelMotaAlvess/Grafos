.PHONY: main run clear fresh

main: ./src/main.cpp
	g++ -g ./src/main.cpp -o main -lGL -lglut

run: main 
	./main
	
clear:
	rm -f main

fresh:
	$(MAKE) clear
	$(MAKE) main
	$(MAKE) run
