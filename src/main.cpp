#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <limits>

#include "./include/InicializadorGrafo.h"

using namespace std;

int main(int argc, char** argv) {
    int opcao;
    string caminho = (argc >= 2 ? argv[1] : string("filename7.txt"));

    cout << "1-Matriz | 2-Lista" << endl;
    cin >> opcao;

    unique_ptr<Grafos> grafo = inicializarGrafo(caminho, (opcao == 1 ? 1 : 2));
    if (!grafo) {
        cout << "Falha ao carregar o grafo!" << endl;
        return 1;
    }

    cout << "grafo label vertice: " << grafo->labelVertice(2) << endl;

    cout << "Aresta entre 0 e 2 existe? " << (grafo->existeAresta(0, 2) ? " Sim" : " Nao") << endl;
    cout << "Aresta entre 0 e 1 existe? " << (grafo->existeAresta(0, 1) ? " Sim" : " Nao") << endl;

    vector<int> vizinhos = grafo->retornarVizinhos(0);
    cout << "Vizinhos do vertice 0: ";
    for (int v : vizinhos) cout << v << " ";
    cout << endl;

    grafo->bfs();
    grafo->bfs(2);
    grafo->imprimirGrafo();
    grafo->dfs();
    grafo->dfs(2);

    int origemD = 0;
    int nVerts = grafo->numeroVertices();
    if (argc >= 3) {
        try {
            origemD = stoi(argv[2]);
        } catch (...) { origemD = 0; }
    } else {
        cout << "Informe o indice da origem para Dijkstra (0.." << (nVerts-1) << ") [padrao 0]: ";
        if (!(cin >> origemD)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            origemD = 0;
        }
    }
    
    if (origemD < 0 || origemD >= nVerts) {
        cout << "Indice fora do intervalo. Usando origem 0." << endl;
        origemD = 0;
    }

    grafo->dijkstra(origemD);

    return 0;
}
