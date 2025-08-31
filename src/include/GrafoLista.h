#ifndef GRAFOLISTA_H
#define GRAFOLISTA_H

#include "Grafos.h"

class GrafoLista : public Grafos {
public:
    bool adicionarVertice(string label) override {
        cout << "Adicionando vertice no GrafoLista" << endl;
        return true;
    }

    bool adicionarAresta(int origem, int destino, float peso = 1.0) override {
        cout << "Adicionando aresta no GrafoLista" << endl;
        return true;
    }

    bool removerVertice(int index) override {
        cout << "Removendo vertice no GrafoLista" << endl;
        return true;
    }

    bool removerAresta(int origem, int destino) override {
        cout << "Removendo aresta no GrafoLista" << endl;
        return true;
    }

    string labelVertice(int index) override {
        cout << "Retornando label do vertice no GrafoLista" << endl;
        return "A";
    }

    void imprimirGrafo() override {
        cout << "Imprimindo GrafoLista" << endl;
    }

    bool existeAresta(int origem, int destino) override {
        cout << "Verificando se existe aresta no GrafoLista" << endl;
        return true;
    }

    float pesoAresta(int origem, int destino) override {
        cout << "Retornando peso da aresta no GrafoLista" << endl;
        return 1.0;
    }

    vector<int> retornarVizinhos(int vertice) override {
        cout << "Retornando vizinhos do vertice no GrafoLista" << endl;
        return vector<int>{1, 2, 3};
    }

    void bfs(int source) override {
        cout << "Executando BFS no GrafoLista" << endl;
    }

    void dfs() override {
        cout << "Executando DFS no GrafoLista" << endl;
    }

private:
    // futura estrutura de lista de adjacência
};

#endif
