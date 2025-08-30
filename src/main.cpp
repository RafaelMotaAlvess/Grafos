#include <iostream>
#include <fstream>
#include <sstream>

#include "./include/GrafoLista.h"
#include "./include/GrafoMatriz.h"
#include "./include/LeitorGrafo.h"

using namespace std;

int main(int argc, char** argv) {
    LeitorGrafo leitor("filename.txt");

    if (!leitor.carregarArquivo()) {
        cout << "Falha ao carregar o grafo!" << endl;
        return 1;
    }

    GrafoMatriz grafo(leitor.getIsDirecionado(), leitor.getIsPonderado());

    // adiciona vértices
    for (int i = 0; i < leitor.getVertices(); i++) {
				char letra = 'A' + i;
        grafo.adicionarVertice(string(1, letra));
    }

    // pega as linhas das arestas
    vector<vector<string>> dados = leitor.getDados();


		// a nomeclaruta é a mesma do arquivo do Lyra (TG02 - Conceitos Básicos)
    for (auto &linha : dados) {
        int Ao = stoi(linha[0]);  // origem
        int Ad = stoi(linha[1]);  // destino

        float Ap = 1.0;
        if (leitor.getIsPonderado() && linha.size() >= 3) {
            Ap = stof(linha[2]); // peso
        }

        grafo.adicionarAresta(Ao, Ad, Ap);
    }

    // grafo.removerVertice(2); // remove o vertice C (índice 2) a
    grafo.removerAresta(0, 2); // remove a aresta A-D
    cout << "grafo label vertice: " <<  grafo.labelVertice(2) << endl;

    grafo.imprimirGrafo();

    return 0;
}
