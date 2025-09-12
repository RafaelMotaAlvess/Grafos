#ifndef GRAFOS_H
#define GRAFOS_H

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <limits>
#include <functional>
#include <algorithm>
#include <cmath>

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
    virtual int numeroVertices() const = 0;

    virtual bool adicionarAresta(int origem, int destino, float peso = 1.0) = 0;
    virtual bool removerAresta(int origem, int destino) = 0;
    virtual bool existeAresta(int origem, int destino) = 0;
    virtual float pesoAresta(int origem, int destino) = 0;

    virtual vector<int> retornarVizinhos(int vertice) = 0;

    void bfs(int source) {
        int n = numeroVertices();
        if (source < 0 || source >= n) {
            cout << "BFS: (fonte invalida)" << endl;
            return;
        }
        vector<bool> visitado(n, false);
        queue<int> q;
        cout << "BFS: ";
        visitado[source] = true;
        q.push(source);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            cout << labelVertice(u) << ' ';
            vector<int> viz = retornarVizinhos(u);
            for (size_t i = 0; i < viz.size(); ++i) {
                int v = viz[i];
                if (!visitado[v]) { visitado[v] = true; q.push(v); }
            }
        }
        cout << '\n';
    }

    void bfs() { bfs(0); }

    void dfs(int source) {
        int n = numeroVertices();
        if (source < 0 || source >= n) {
            cout << "DFS: (fonte invalida)" << endl;
            return;
        }
        vector<bool> visitado(n, false);
        cout << "DFS: ";
        dfsVisita(source, visitado);
        cout << '\n';
    }

    void dfs() { dfs(0); }

    vector<float> dijkstra(int source) {
        int n = numeroVertices();

        const float INF = std::numeric_limits<float>::infinity();
        vector<float> dist(n, INF);
        vector<int>   pai(n, -1);
        vector<bool>  visitado(n, false);

        if (source < 0 || source >= n) {
            cout << "Dijkstra: fonte inválida" << endl;
            return dist;
        }

        for (int u = 0; u < n; ++u) {
            vector<int> viz = retornarVizinhos(u);
            for (size_t i = 0; i < viz.size(); ++i) {
                int v = viz[i];
                float w = pesoAresta(u, v);
                if (!std::isnan(w) && w < 0) {
                    cout << "Dijkstra: grafo possui peso negativo; não suportado" << endl;
                    return vector<float>();
                }
            }
        }

        dist[source] = 0.0f;

        for (int iter = 0; iter < n; ++iter) {
            int u = -1;
            float melhor = INF;
            for (int i = 0; i < n; ++i) {
                if (!visitado[i] && dist[i] < melhor) {
                    melhor = dist[i];
                    u = i;
                }
            }
            if (u == -1) break;
            visitado[u] = true;

            vector<int> viz = retornarVizinhos(u);
            for (size_t i = 0; i < viz.size(); ++i) {
                int v = viz[i];
                float w = pesoAresta(u, v);
                if (std::isnan(w)) continue;
                float nd = dist[u] + w;
                if (nd < dist[v]) {
                    dist[v] = nd;
                    pai[v] = u;
                }
            }
        }

        cout << "Dijkstra a partir de " << labelVertice(source) << " (" << source << ")" << endl;
        for (int v = 0; v < n; ++v) {
            cout << "Destino " << labelVertice(v) << " (" << v << ")";
            if (dist[v] == INF) {
                cout << ": dist = INF; caminho: inexistente" << endl;
                continue;
            }
            cout << ": dist = " << dist[v] << "; caminho: ";
            vector<int> caminho;
            int x = v;
            while (x != -1) { caminho.push_back(x); x = pai[x]; }
            for (int i = (int)caminho.size() - 1; i >= 0; --i) {
                cout << labelVertice(caminho[i]);
                if (i > 0) cout << " -> ";
            }
            cout << endl;
        }

        return dist;
    }

    vector<float> dijkstra() { return dijkstra(0); }

private:
    void dfsVisita(int u, vector<bool>& visitado) {
        visitado[u] = true;
        cout << labelVertice(u) << ' ';
        vector<int> viz = retornarVizinhos(u);
        for (size_t i = 0; i < viz.size(); ++i) {
            int v = viz[i];
            if (!visitado[v]) dfsVisita(v, visitado);
        }
    }
};

#endif
