.PHONY: main generate_report run run_report clear fresh

CXX := g++
CXXFLAGS := -g -std=c++17

main: ./src/main.cpp
	$(CXX) $(CXXFLAGS) ./src/main.cpp -o main -lGL -lglut

generate_report: ./src/generate_report.cpp
	$(CXX) $(CXXFLAGS) ./src/generate_report.cpp -o generate_report

run: main 
	./main

run_report: generate_report
	./generate_report
	
clear:
	rm -f main generate_report

fresh:
	$(MAKE) clear
	$(MAKE) main
	$(MAKE) run
