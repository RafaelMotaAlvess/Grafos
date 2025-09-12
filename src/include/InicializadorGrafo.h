#ifndef INICIALIZADOR_GRAFO_H
#define INICIALIZADOR_GRAFO_H

#include <memory>
#include <string>
#include <vector>

#include "GrafoLista.h"
#include "GrafoMatriz.h"
#include "LeitorGrafo.h"

using namespace std;

// tipo: 1 = Matriz, 2 = Lista
inline unique_ptr<Grafos> inicializarGrafo(const string& caminhoArquivo, int tipo) {
    LeitorGrafo leitor(caminhoArquivo);
    if (!leitor.carregarArquivo()) {
        return nullptr;
    }

    unique_ptr<Grafos> grafo;
    if (tipo == 1) {
        grafo = make_unique<GrafoMatriz>(leitor.getIsDirecionado(), leitor.getIsPonderado());
    } else {
        grafo = make_unique<GrafoLista>(leitor.getIsDirecionado(), leitor.getIsPonderado());
    }

    // Adiciona vértices com rótulos A, B, C, ...
    for (int i = 0; i < leitor.getVertices(); ++i) {
        char letra = 'A' + i;
        grafo->adicionarVertice(string(1, letra));
    }

    // Adiciona arestas a partir dos dados lidos
    const vector<vector<string>>& dados = leitor.getDados();
    for (const auto& linha : dados) {
        if (linha.size() < 2) continue;
        int origem = stoi(linha[0]);
        int destino = stoi(linha[1]);
        float peso = 1.0f;
        if (leitor.getIsPonderado() && linha.size() >= 3) {
            peso = stof(linha[2]);
        }
        grafo->adicionarAresta(origem, destino, peso);
    }

    return grafo;
}

#endif
