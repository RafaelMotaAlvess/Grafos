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
    vector<vector<Aresta>> adj;   // lista de adjacência
    vector<string> labels;        // rótulos/nomes dos vértices

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
    using Grafos::bfs;
    using Grafos::dfs;

    GrafoLista(bool dir=false, bool pond=false) {
        this->isDirecionado = dir;
        this->isPonderado   = pond;
    }

    // -------- VÉRTICES --------
    bool adicionarVertice(string label) override {
        labels.push_back(std::move(label));
        adj.emplace_back(); // nova lista para o novo vértice
        return true;
    }

    bool removerVertice(int index) override {
        if (!indiceValido(index)) return false;

        // remove arestas que chegam em 'index'
        for (int u = 0; u < (int)adj.size(); ++u) {
            if (u == index) continue;
            auto& lu = adj[u];
            lu.erase(remove_if(lu.begin(), lu.end(),
                    [&](const Aresta& a){ return a.destino == index; }),
                lu.end());
        }

        // remove as arestas que saem de 'index' e o próprio vértice
        adj.erase(adj.begin() + index);

        // reindexa destinos > index
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

    // -------- ARESTAS --------
    bool adicionarAresta(int origem, int destino, float peso = 1.0) override {
        if (!indiceValido(origem) || !indiceValido(destino)) return false;
        float w = this->isPonderado ? (float)peso : 1.0f;

        // cria/atualiza origem->destino
        {
            auto it = findAresta(origem, destino);
            if (it != adj[origem].end()) it->peso = w;
            else adj[origem].emplace_back(destino, w);
        }

        // se não-direcionado, espelha
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

    // -------- IMPRESSÃO --------
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

    // -------- BFS/DFS --------
    void bfs(int source) override {
        int n = (int)adj.size();
        if (source < 0 || source >= n) { cout << "BFS: (fonte invalida)\n"; return; }
        vector<bool> vis(n,false);
        queue<int> q;
        cout << "BFS: ";
        vis[source]=true; q.push(source);
        while(!q.empty()){
            int u=q.front(); q.pop();
            cout << labelVertice(u) << " ";
            for (auto& a: adj[u]) if(!vis[a.destino]){ vis[a.destino]=true; q.push(a.destino); }
        }
        cout << "\n";
    }

    void dfs(int source) override {
        int n = (int)adj.size();
        if (source < 0 || source >= n) { cout << "DFS: (fonte invalida)\n"; return; }
        vector<bool> vis(n,false);
        cout << "DFS: ";
        dfsVisit(source, vis);
        cout << "\n";
    }

private:
    void dfsVisit(int u, vector<bool>& vis){
        vis[u]=true;
        cout << labelVertice(u) << " ";
        for (auto& a : adj[u]) if(!vis[a.destino]) dfsVisit(a.destino, vis);
    }
};

#endif
