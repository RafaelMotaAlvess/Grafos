#ifndef GRAFOMATRIZ_H
#define GRAFOMATRIZ_H

#include "Grafos.h"

class GrafoMatriz : public Grafos {
public:
    bool adicionarVertice(string label) override {
        cout << "Adicionando vertice no GrafoMatriz" << endl;
        return true;
    }

    bool adicionarAresta(int origem, int destino, float peso = 1.0) override {
        cout << "Adicionando aresta no GrafoMatriz" << endl;
        return true;
    }

    bool removerVertice(int index) override {
        cout << "Removendo vertice no GrafoMatriz" << endl;
        return true;
    }

    bool removerAresta(int origem, int destino) override {
        cout << "Removendo aresta no GrafoMatriz" << endl;
        return true;
    }

    string labelVertice(int index) override {
        cout << "Retornando label do vertice no GrafoMatriz" << endl;
        return "A";
    }

    void imprimirGrafo() override {
        cout << "Imprimindo GrafoMatriz" << endl;
    }

    bool existeAresta(int origem, int destino) override {
        cout << "Verificando se existe aresta no GrafoMatriz" << endl;
        return true;
    }

    float pesoAresta(int origem, int destino) override {
        cout << "Retornando peso da aresta no GrafoMatriz" << endl;
        return 1.0;
    }

    vector<int> retornarVizinhos(int vertice) override {
        cout << "Retornando vizinhos do vertice no GrafoMatriz" << endl;
        return vector<int>{1, 2, 3};
    }

    void bfs() override {
        cout << "Executando BFS no GrafoMatriz" << endl;
    }

    void dfs() override {
        cout << "Executando DFS no GrafoMatriz" << endl;
    }

private:
    // futura estrutura de matriz de adjacência
};

#endif
