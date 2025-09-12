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
        string arg = argv[2];
        try {
            origemD = stoi(arg);
        } catch (...) {
            bool found = false;
            for (int i = 0; i < nVerts; ++i) {
                if (grafo->labelVertice(i) == arg) { origemD = i; found = true; break; }
            }
            if (!found) origemD = 0;
        }
    } else {
        cout << "Informe a origem para Dijkstra (indice 0.." << (nVerts-1)
             << " ou label A..): [padrao 0] ";
        string entrada;
        if (!(cin >> entrada)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            origemD = 0;
        } else {
            try {
                origemD = stoi(entrada);
            } catch (...) {
                bool found = false;
                for (int i = 0; i < nVerts; ++i) {
                    if (grafo->labelVertice(i) == entrada) { origemD = i; found = true; break; }
                }
                
                if (!found) {
                    cout << "Label desconhecida. Usando origem 0." << endl;
                    origemD = 0;
                }
            }
        }
    }

    if (origemD < 0 || origemD >= nVerts) {
        cout << "Origem fora do intervalo. Usando 0." << endl;
        origemD = 0;
    }

    grafo->dijkstra(origemD);

    return 0;
}
