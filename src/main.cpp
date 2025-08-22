#include <iostream>

#include "./include/GrafoLista.h"
#include "./include/GrafoMatriz.h"

using namespace std;


int main(int argc, char** argv) {
	GrafoLista grafoLista;
	GrafoMatriz grafoMatriz;

	grafoLista.adicionarVertice("A");
	grafoLista.adicionarAresta(0, 1);

	grafoMatriz.adicionarVertice("B");
	grafoMatriz.adicionarAresta(0, 1);

	return 0;
}