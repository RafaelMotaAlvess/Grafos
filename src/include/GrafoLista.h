#ifndef GRAFOLISTA_H
#define GRAFOLISTA_H

#include "Grafos.h"
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <queue>
#include <iomanip>
#include <limits>

using namespace std;

struct Aresta {
    int destino;
    float peso;
    Aresta(int d, float p) : destino(d), peso(p) {}
};

class GrafoLista : public Grafos {
    vector<vector<Aresta>> adj;
    vector<string> labels;

    bool indiceValido(int i) const {
        return i >= 0 && i < (int)adj.size();
    }
    auto findAresta(int u, int v) {
        return std::find_if(adj[u].begin(), adj[u].end(),
            [&](const Aresta& a){ return a.destino == v; });
    }
    auto findArestaConst(int u, int v) const {
        return std::find_if(adj[u].begin(), adj[u].end(),
            [&](const Aresta& a){ return a.destino == v; });
    }

public:
    GrafoLista(bool dir=false, bool pond=false) {
        this->isDirecionado = dir;
        this->isPonderado   = pond;
    }

    bool adicionarVertice(string label) override {
        labels.push_back(std::move(label));
        adj.emplace_back();
        return true;
    }

    bool removerVertice(int index) override {
        if (!indiceValido(index)) return false;

        for (int u = 0; u < (int)adj.size(); ++u) {
            if (u == index) continue;
            auto& lu = adj[u];
            lu.erase(remove_if(lu.begin(), lu.end(),
                    [&](const Aresta& a){ return a.destino == index; }),
                lu.end());
        }

        adj.erase(adj.begin() + index);

        for (auto& lu : adj) {
            for (auto& a : lu) if (a.destino > index) a.destino -= 1;
        }

        labels.erase(labels.begin() + index);
        return true;
    }

    string labelVertice(int index) override {
        if (!indiceValido(index)) return "";
        return labels[index];
    }

    int numeroVertices() const override { return static_cast<int>(adj.size()); }

    bool adicionarAresta(int origem, int destino, float peso = 1.0) override {
        if (!indiceValido(origem) || !indiceValido(destino)) return false;
        float w = this->isPonderado ? (float)peso : 1.0f;

        {
            auto it = findAresta(origem, destino);
            if (it != adj[origem].end()) it->peso = w;
            else adj[origem].emplace_back(destino, w);
        }

        if (!this->isDirecionado) {
            auto it = findAresta(destino, origem);
            if (it != adj[destino].end()) it->peso = w;
            else adj[destino].emplace_back(origem, w);
        }
        return true;
    }

    bool removerAresta(int origem, int destino) override {
        if (!indiceValido(origem) || !indiceValido(destino)) return false;

        auto& lo = adj[origem];
        lo.erase(remove_if(lo.begin(), lo.end(),
                [&](const Aresta& a){ return a.destino == destino; }),
            lo.end());

        if (!this->isDirecionado) {
            auto& ld = adj[destino];
            ld.erase(remove_if(ld.begin(), ld.end(),
                    [&](const Aresta& a){ return a.destino == origem; }),
                ld.end());
        }
        return true;
    }

    bool existeAresta(int origem, int destino) override {
        if (!indiceValido(origem) || !indiceValido(destino)) return false;
        return findArestaConst(origem, destino) != adj[origem].end();
    }

    float pesoAresta(int origem, int destino) override {
        if (!indiceValido(origem) || !indiceValido(destino)) 
            return std::numeric_limits<float>::quiet_NaN();
        auto it = findArestaConst(origem, destino);
        if (it == adj[origem].end())
            return std::numeric_limits<float>::quiet_NaN();
        return it->peso;
    }

    vector<int> retornarVizinhos(int v) override {
        vector<int> vs;
        if (!indiceValido(v)) return vs;
        vs.reserve(adj[v].size());
        for (auto& a : adj[v]) vs.push_back(a.destino);
        return vs;
    }

    void imprimirGrafo() override {
        cout << (this->isDirecionado ? "GRAFO DIRECIONADO" : "GRAFO NAO-DIRECIONADO")
             << (this->isPonderado ? " PONDERADO\n" : " NAO-PONDERADO\n");
        cout << "Vertices: " << adj.size() << "\n\n";
        for (int u = 0; u < (int)adj.size(); ++u) {
            cout << u << " (" << labels[u] << "): ";
            for (auto& e : adj[u]) {
                if (this->isPonderado) cout << e.destino << "(" << e.peso << ") ";
                else                    cout << e.destino << " ";
            }
            cout << "\n";
        }
        cout.flush();
    }
};

#endif
