#ifndef GRAFOMATRIZ_H
#define GRAFOMATRIZ_H

#include "Grafos.h"

#include <iomanip>
#include <sstream>

class GrafoMatriz : public Grafos {
    vector<vector<float>> matrizAdjacencia;
    vector<string> labels;
public:
    GrafoMatriz(bool isDirecionado = false, bool isPonderado = false) {
        this->isDirecionado = isDirecionado;
        this->isPonderado = isPonderado;
    }

    bool adicionarVertice(string label) override {
        labels.push_back(label);
        int novoTamanho = matrizAdjacencia.size() + 1;

        // Adiciona uma nova linha com zeros
        matrizAdjacencia.push_back(vector<float>(novoTamanho, 0.0));

        // Adiciona uma nova coluna com zeros em todas as linhas existentes
        for (int i = 0; i < novoTamanho - 1; i++) {
            matrizAdjacencia[i].push_back(0.0);
        }

        return true;
    }

    bool adicionarAresta(int origem, int destino, float peso = 1.0) override {
        if(this->isDirecionado){
            matrizAdjacencia[origem][destino] = peso;
        } else {
            matrizAdjacencia[origem][destino] = peso;
            matrizAdjacencia[destino][origem] = peso;
        }
        return true;
    }

    bool removerVertice(int index) override {
        // Remove a linha de um vertice
        matrizAdjacencia.erase(matrizAdjacencia.begin() + index);

        // Remove a coluna de um vertice
        for (auto& row : matrizAdjacencia) {
            row.erase(row.begin() + index);
        }

        // Remove o label do vertice
        labels.erase(labels.begin() + index);

        return true;
    }

    bool removerAresta(int origem, int destino) override {
        if(this->isDirecionado){
            matrizAdjacencia[origem][destino] = 0.0;
        } else {
            matrizAdjacencia[origem][destino] = 0.0;
            matrizAdjacencia[destino][origem] = 0.0;
        }
        return true;
    }

    string labelVertice(int index) override {
        return this->labels.at(index);
    }

    void imprimirGrafo() override {
        cout << "Matriz de Adjacência:" << endl;

        int n = matrizAdjacencia.size();

        size_t largura = 0;
        for (int i = 0; i < n; i++) {
            largura = max(largura, labels[i].size());
            for (int j = 0; j < n; j++) {
                std::stringstream ss;
                ss << matrizAdjacencia[i][j];
                largura = max(largura, ss.str().size());
            }
        }

        largura += 2;

        cout << setw(largura) << " ";
        for (int j = 0; j < n; j++) {
            cout << setw(largura) << labels[j];
        }
        cout << endl;

        for (int i = 0; i < n; i++) {
            cout << setw(largura) << labels[i];
            for (int j = 0; j < n; j++) {
                cout << setw(largura) << matrizAdjacencia[i][j];
            }
            cout << endl;
        }
    }

    bool existeAresta(int origem, int destino) override {
        if(matrizAdjacencia[origem][destino] != 0.0){
            return true;
        }

        return false;
    }

    float pesoAresta(int origem, int destino) override {
        return matrizAdjacencia[origem][destino];
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
};

#endif
