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
#include <chrono>
#include <numeric>
#include <unordered_set>

using namespace std;

class Grafos {
public:
    virtual ~Grafos() = default;

    bool isDirecionado = false;
    bool isPonderado = false;

    struct ResultadoColoracao {
        vector<int> cores;
        int numeroCores = 0;
        double tempoMs = 0.0;
        bool sucesso = false;
        string descricao;
    };

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
            int u = q.front();
            q.pop();

            // Imprime a vertice atual
            cout << labelVertice(u) << ' ';

            // Para cada vizinho de u, se ainda não visitado, marca e enfileira
            vector<int> viz = retornarVizinhos(u);
            for (int i = 0; i < viz.size(); ++i) {
                int v = viz[i];
                if (!visitado[v]) {
                    visitado[v] = true;
                    q.push(v);
                }
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

        // verifica se há arestas com peso negativo
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

        cout << "Dijkstra a partir de " << labelVertice(source) << endl;
        for (int v = 0; v < n; ++v) {
            cout << "Destino " << labelVertice(v);
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

    ResultadoColoracao coloracaoForcaBruta() {
        ResultadoColoracao resultado;

        int n = numeroVertices();
        if (n >= 10) {
            resultado.descricao = string("Forca Bruta com mais de 10 vertices (n = ")
                                   + to_string(n) + " vertices)";
            return resultado;
        }

        resultado.descricao = "Forca Bruta";

        if (n == 0) {
            resultado.sucesso = true;
            return resultado;
        }

        auto inicio = chrono::steady_clock::now();
        vector<int> cores(n, -1);
        vector<vector<int>> vizinhos = construirVizinhosColoracao();

        function<bool(int,int)> atribui = [&](int v, int maxCor){
            if (v == n) return true;
            for (int cor = 0; cor < maxCor; ++cor) {
                bool temConflito = false;
                for (int viz : vizinhos[v]) {
                    if (cores[viz] == cor) { temConflito = true; break; }
                }
                if (!temConflito) {
                    cores[v] = cor;
                    if (atribui(v + 1, maxCor)) return true;
                    cores[v] = -1;
                }
            }
            return false;
        };

        int minCoresInicial = (n <= 1 ? 1 : 2);
        for (int k = minCoresInicial; k <= n; ++k) {
            fill(cores.begin(), cores.end(), -1);
            if (atribui(0, k)) {
                resultado.cores = cores;
                resultado.numeroCores = 0;
                for (int cor : cores)
                    resultado.numeroCores = max(resultado.numeroCores, cor + 1);
                resultado.sucesso = true;
                break;
            }
        }

        auto fim = chrono::steady_clock::now();
        resultado.tempoMs = chrono::duration<double, std::milli>(fim - inicio).count();
        return resultado;
    }

    ResultadoColoracao coloracaoGulosaSimples() {
        ResultadoColoracao resultado;
        resultado.descricao = "Heuristica Gulosa Simples";

        int n = numeroVertices();
        vector<int> cores(n, -1);
        vector<vector<int>> vizinhos = construirVizinhosColoracao();

        auto inicio = chrono::steady_clock::now();
        for (int v = 0; v < n; ++v) {
            vector<bool> usadas(n, false);
            for (int viz : vizinhos[v]) {
                int corViz = (viz >= 0 && viz < n ? cores[viz] : -1);
                if (corViz >= 0) usadas[corViz] = true;
            }
            int cor = 0;
            while (cor < n && usadas[cor]) ++cor;
            cores[v] = cor;
        }
        auto fim = chrono::steady_clock::now();

        resultado.cores = cores;
        resultado.numeroCores = 0;
        for (int cor : cores)
            resultado.numeroCores = max(resultado.numeroCores, cor + 1);
        resultado.tempoMs = chrono::duration<double, std::milli>(fim - inicio).count();
        resultado.sucesso = true;
        return resultado;
    }

    ResultadoColoracao coloracaoWelshPowell() {
        ResultadoColoracao resultado;
        resultado.descricao = "Welsh-Powell";

        int n = numeroVertices();
        vector<int> cores(n, -1);
        vector<int> ordem(n);
        vector<int> graus(n, 0);

        vector<vector<int>> vizinhos = construirVizinhosColoracao();
        for (int v = 0; v < n; ++v) {
            graus[v] = static_cast<int>(vizinhos[v].size());
        }

        iota(ordem.begin(), ordem.end(), 0);
        sort(ordem.begin(), ordem.end(), [&](int a, int b){
            if (graus[a] != graus[b]) return graus[a] > graus[b];
            return a < b;
        });

        auto inicio = chrono::steady_clock::now();
        vector<bool> usadas(n, false);

        for (int v : ordem) {
            fill(usadas.begin(), usadas.end(), false);
            for (int viz : vizinhos[v]) {
                int corViz = cores[viz];
                if (corViz >= 0 && corViz < n) usadas[corViz] = true;
            }
            int cor = 0;
            while (cor < n && usadas[cor]) ++cor;
            cores[v] = cor;
        }
        auto fim = chrono::steady_clock::now();

        resultado.cores = cores;
        resultado.numeroCores = 0;
        for (int cor : cores)
            resultado.numeroCores = max(resultado.numeroCores, cor + 1);
        resultado.tempoMs = chrono::duration<double, std::milli>(fim - inicio).count();
        resultado.sucesso = true;
        return resultado;
    }

    ResultadoColoracao coloracaoDSatur() {
        ResultadoColoracao resultado;
        resultado.descricao = "DSATUR";

        int n = numeroVertices();
        vector<int> cores(n, -1);
        vector<int> graus(n, 0);
        vector<vector<int>> vizinhos = construirVizinhosColoracao();

        for (int v = 0; v < n; ++v) {
            graus[v] = static_cast<int>(vizinhos[v].size());
        }

        vector<unordered_set<int>> coresAdj(n);
        vector<int> saturacao(n, 0);
        vector<bool> colorido(n, false);

        auto inicio = chrono::steady_clock::now();

        for (int passo = 0; passo < n; ++passo) {
            int escolhido = -1;
            int melhorSat = -1;
            int melhorGrau = -1;

            for (int v = 0; v < n; ++v) {
                if (colorido[v]) continue;
                int sat = saturacao[v];
                if (sat > melhorSat || (sat == melhorSat && graus[v] > melhorGrau)) {
                    melhorSat = sat;
                    melhorGrau = graus[v];
                    escolhido = v;
                }
            }

            if (escolhido == -1) break;

            vector<bool> usadas(n, false);
            for (int viz : vizinhos[escolhido]) {
                int corViz = cores[viz];
                if (corViz >= 0 && corViz < n) usadas[corViz] = true;
            }
            int cor = 0;
            while (cor < n && usadas[cor]) ++cor;
            cores[escolhido] = cor;
            colorido[escolhido] = true;

            for (int viz : vizinhos[escolhido]) {
                if (colorido[viz]) continue;
                if (coresAdj[viz].insert(cor).second) {
                    saturacao[viz] = static_cast<int>(coresAdj[viz].size());
                }
            }
        }

        auto fim = chrono::steady_clock::now();

        resultado.cores = cores;
        resultado.numeroCores = 0;
        for (int cor : cores)
            resultado.numeroCores = max(resultado.numeroCores, cor + 1);
        resultado.tempoMs = chrono::duration<double, std::milli>(fim - inicio).count();
        resultado.sucesso = true;
        return resultado;
    }

protected:
    vector<vector<int>> construirVizinhosColoracao() {
        int n = numeroVertices();
        vector<vector<int>> viz(n);
        for (int v = 0; v < n; ++v) {
            vector<int> adjacentes = retornarVizinhos(v);
            for (int u : adjacentes) {
                if (u < 0 || u >= n) continue;
                viz[v].push_back(u);
                viz[u].push_back(v);
            }
        }
        for (auto& lista : viz) {
            sort(lista.begin(), lista.end());
            lista.erase(unique(lista.begin(), lista.end()), lista.end());
        }
        return viz;
    }

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
