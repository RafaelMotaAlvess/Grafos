#include <iostream>
#include <fstream>
#include <sstream>
#include <memory> // <-- ADICIONE isto

#include "./include/GrafoLista.h"
#include "./include/GrafoMatriz.h"
#include "./include/LeitorGrafo.h"

using namespace std;

int main(int argc, char** argv) {
    int opcao;
    string caminho = (argc >= 2 ? argv[1] : string("filename.txt"));
    LeitorGrafo leitor(caminho);

    if (!leitor.carregarArquivo()) {
        cout << "Falha ao carregar o grafo!" << endl;
        return 1;
    }

    cout << "1-Matriz | 2-Lista" << endl;
    cin >> opcao;

    // ===== ESCOLHA DA IMPLEMENTAÇÃO =====
    unique_ptr<Grafos> grafo;
    if (opcao == 1) {
        grafo = make_unique<GrafoMatriz>(leitor.getIsDirecionado(), leitor.getIsPonderado());
    } else if (opcao == 2) {
        grafo = make_unique<GrafoLista>(leitor.getIsDirecionado(), leitor.getIsPonderado());
    } else {
        cout << "Opcao invalida." << endl;
    }
    // ====================================

    // adiciona vértices
    for (int i = 0; i < leitor.getVertices(); i++) {
        char letra = 'A' + i;
        grafo->adicionarVertice(string(1, letra));   // OBS: usa '->' agora
    }

    // pega as linhas das arestas
    vector<vector<string>> dados = leitor.getDados();

    // a nomenclatura é a mesma do arquivo do Lyra (TG02 - Conceitos Básicos)
    for (auto &linha : dados) {
        int Ao = stoi(linha[0]);  // origem
        int Ad = stoi(linha[1]);  // destino

        float Ap = 1.0f;
        if (leitor.getIsPonderado() && linha.size() >= 3) {
            Ap = stof(linha[2]); // peso
        }

        grafo->adicionarAresta(Ao, Ad, Ap);
    }

    // grafo->removerVertice(2);
    // grafo->removerAresta(0, 2);

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

    return 0;
}
