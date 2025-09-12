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

    cout << "==========================" << endl;
    grafo->imprimirGrafo();

    int nVerts = grafo->numeroVertices();
    int origem = 0;

    if (argc >= 3) {
        string arg = argv[2];
        try {
            origem = stoi(arg);
        } catch (...) {
            bool found = false;
            for (int i = 0; i < nVerts; ++i) {
                if (grafo->labelVertice(i) == arg) { origem = i; found = true; break; }
            }
            if (!found) origem = 0;
        }
    } else {
        cout << "Informe a origem (indice 0.." << (nVerts-1)
             << " ou label A..): [padrao 0] ";
        string entrada;
        if (!(cin >> entrada)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            origem = 0;
        } else {
            try {
                origem = stoi(entrada);
            } catch (...) {
                bool found = false;
                for (int i = 0; i < nVerts; ++i) {
                    if (grafo->labelVertice(i) == entrada) { origem = i; found = true; break; }
                }
                if (!found) {
                    cout << "Label desconhecida. Usando origem 0." << endl;
                    origem = 0;
                }
            }
        }
    }

    if (origem < 0 || origem >= nVerts) {
        cout << "Origem fora do intervalo. Usando 0." << endl;
        origem = 0;
    }

    cout << "==========================" << endl;

    grafo->bfs(origem);

    cout << "==========================" << endl;

    grafo->dfs(origem);

    cout << "==========================" << endl;

    grafo->dijkstra(origem);

    return 0;
}
