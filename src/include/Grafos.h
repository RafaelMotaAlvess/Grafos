#ifndef GRAFOS_H
#define GRAFOS_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Grafos {
public:
    virtual ~Grafos() = default;

    bool isDirecionado = false;
    bool isPonderado = false;

    virtual bool adicionarVertice(string label) = 0;
    virtual bool removerVertice(int index) = 0;
    virtual string labelVertice(int index) = 0;
    virtual void imprimirGrafo() = 0;

    virtual bool adicionarAresta(int origem, int destino, float peso = 1.0) = 0;
    virtual bool removerAresta(int origem, int destino) = 0;
    virtual bool existeAresta(int origem, int destino) = 0;
    virtual float pesoAresta(int origem, int destino) = 0;

    virtual vector<int> retornarVizinhos(int vertice) = 0;

    virtual void bfs(int source) = 0;
    virtual void dfs() = 0;
};

#endif
