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
#include <utility>

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

    struct ResultadoArvoreMinima {
        vector<pair<int,int>> arestas;
        double somaPesos = 0.0;
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
        int totalVertices = numeroVertices();
        if (totalVertices >= 10) {
            resultado.descricao = string("Forca Bruta com mais de 10 vertices (n = ")
                                   + to_string(totalVertices) + " vertices)";
            return resultado;
        }

        resultado.descricao = "Forca Bruta";

        if (totalVertices == 0) {
            resultado.sucesso = true;
            return resultado;
        }

        auto inicio = chrono::steady_clock::now();

        vector<int> corPorVertice(totalVertices, -1);
        vector<vector<int>> vizinhosPorVertice = construirVizinhosColoracao();

        function<bool(int,int)> tentarAtribuirCor = [&](int indiceVertice, int limiteCores){
            if (indiceVertice == totalVertices) return true;
            for (int corCandidata = 0; corCandidata < limiteCores; ++corCandidata) {
                bool existeConflito = false;
                for (int vizinho : vizinhosPorVertice[indiceVertice]) {
                    if (corPorVertice[vizinho] == corCandidata) { existeConflito = true; break; }
                }
                if (!existeConflito) {
                    corPorVertice[indiceVertice] = corCandidata;
                    if (tentarAtribuirCor(indiceVertice + 1, limiteCores)) return true;
                    corPorVertice[indiceVertice] = -1;
                }
            }
            return false;
        };

        int quantidadeCoresMinima = (totalVertices <= 1 ? 1 : 2);
        for (int totalCores = quantidadeCoresMinima; totalCores <= totalVertices; ++totalCores) {
            fill(corPorVertice.begin(), corPorVertice.end(), -1);
            if (tentarAtribuirCor(0, totalCores)) {
                resultado.cores = corPorVertice;
                resultado.numeroCores = 0;
                for (int cor : corPorVertice)
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

        int totalVertices = numeroVertices();
        vector<int> corPorVertice(totalVertices, -1);
        vector<vector<int>> vizinhosPorVertice = construirVizinhosColoracao();

        auto inicio = chrono::steady_clock::now();
        for (int indiceVertice = 0; indiceVertice < totalVertices; ++indiceVertice) {
            vector<bool> corUtilizada(totalVertices, false);
            for (int vizinho : vizinhosPorVertice[indiceVertice]) {
                int corVizinho = (vizinho >= 0 && vizinho < totalVertices ? corPorVertice[vizinho] : -1);
                if (corVizinho >= 0) corUtilizada[corVizinho] = true;
            }
            int corCandidata = 0;
            while (corCandidata < totalVertices && corUtilizada[corCandidata]) ++corCandidata;
            corPorVertice[indiceVertice] = corCandidata;
        }
        auto fim = chrono::steady_clock::now();

        resultado.cores = corPorVertice;
        resultado.numeroCores = 0;
        for (int cor : corPorVertice)
            resultado.numeroCores = max(resultado.numeroCores, cor + 1);
        resultado.tempoMs = chrono::duration<double, std::milli>(fim - inicio).count();
        resultado.sucesso = true;
        return resultado;
    }

    ResultadoColoracao coloracaoWelshPowell() {
        ResultadoColoracao resultado;
        resultado.descricao = "Welsh-Powell";

        int totalVertices = numeroVertices();
        vector<int> corPorVertice(totalVertices, -1);
        vector<int> ordemVertices(totalVertices);
        vector<int> grauPorVertice(totalVertices, 0);

        vector<vector<int>> vizinhosPorVertice = construirVizinhosColoracao();
        for (int indiceVertice = 0; indiceVertice < totalVertices; ++indiceVertice) {
            grauPorVertice[indiceVertice] = static_cast<int>(vizinhosPorVertice[indiceVertice].size());
        }

        iota(ordemVertices.begin(), ordemVertices.end(), 0);
        sort(ordemVertices.begin(), ordemVertices.end(), [&](int a, int b){
            if (grauPorVertice[a] != grauPorVertice[b]) return grauPorVertice[a] > grauPorVertice[b];
            return a < b;
        });

        auto inicio = chrono::steady_clock::now();
        vector<bool> corUtilizada(totalVertices, false);

        for (int indiceVertice : ordemVertices) {
            fill(corUtilizada.begin(), corUtilizada.end(), false);
            for (int vizinho : vizinhosPorVertice[indiceVertice]) {
                int corVizinho = corPorVertice[vizinho];
                if (corVizinho >= 0 && corVizinho < totalVertices) corUtilizada[corVizinho] = true;
            }
            int corCandidata = 0;
            while (corCandidata < totalVertices && corUtilizada[corCandidata]) ++corCandidata;
            corPorVertice[indiceVertice] = corCandidata;
        }
        auto fim = chrono::steady_clock::now();

        resultado.cores = corPorVertice;
        resultado.numeroCores = 0;
        for (int cor : corPorVertice)
            resultado.numeroCores = max(resultado.numeroCores, cor + 1);
        resultado.tempoMs = chrono::duration<double, std::milli>(fim - inicio).count();
        resultado.sucesso = true;
        return resultado;
    }

    ResultadoColoracao coloracaoDSatur() {
        ResultadoColoracao resultado;
        resultado.descricao = "DSATUR";

        int totalVertices = numeroVertices();
        vector<int> corPorVertice(totalVertices, -1);
        vector<int> grauPorVertice(totalVertices, 0);
        vector<vector<int>> vizinhosPorVertice = construirVizinhosColoracao();

        for (int indiceVertice = 0; indiceVertice < totalVertices; ++indiceVertice) {
            grauPorVertice[indiceVertice] = static_cast<int>(vizinhosPorVertice[indiceVertice].size());
        }

        vector<unordered_set<int>> coresAdjacentes(totalVertices);
        vector<int> grauSaturacao(totalVertices, 0);
        vector<bool> verticeColorido(totalVertices, false);

        auto inicio = chrono::steady_clock::now();

        for (int passo = 0; passo < totalVertices; ++passo) {
            int verticeEscolhido = -1;
            int melhorSaturacao = -1;
            int melhorGrau = -1;

            for (int indiceVertice = 0; indiceVertice < totalVertices; ++indiceVertice) {
                if (verticeColorido[indiceVertice]) continue;
                int saturacaoAtual = grauSaturacao[indiceVertice];
                if (saturacaoAtual > melhorSaturacao ||
                    (saturacaoAtual == melhorSaturacao && grauPorVertice[indiceVertice] > melhorGrau)) {
                    melhorSaturacao = saturacaoAtual;
                    melhorGrau = grauPorVertice[indiceVertice];
                    verticeEscolhido = indiceVertice;
                }
            }

            if (verticeEscolhido == -1) break;

            vector<bool> corUtilizada(totalVertices, false);
            for (int vizinho : vizinhosPorVertice[verticeEscolhido]) {
                int corVizinho = corPorVertice[vizinho];
                if (corVizinho >= 0 && corVizinho < totalVertices) corUtilizada[corVizinho] = true;
            }
            int corCandidata = 0;
            while (corCandidata < totalVertices && corUtilizada[corCandidata]) ++corCandidata;
            corPorVertice[verticeEscolhido] = corCandidata;
            verticeColorido[verticeEscolhido] = true;

            for (int vizinho : vizinhosPorVertice[verticeEscolhido]) {
                if (verticeColorido[vizinho]) continue;
                if (coresAdjacentes[vizinho].insert(corCandidata).second) {
                    grauSaturacao[vizinho] = static_cast<int>(coresAdjacentes[vizinho].size());
                }
            }
        }

        auto fim = chrono::steady_clock::now();

        resultado.cores = corPorVertice;
        resultado.numeroCores = 0;
        for (int cor : corPorVertice)
            resultado.numeroCores = max(resultado.numeroCores, cor + 1);
        resultado.tempoMs = chrono::duration<double, std::milli>(fim - inicio).count();
        resultado.sucesso = true;
        return resultado;
    }

    ResultadoArvoreMinima arvoreGeradoraPrim() {
        ResultadoArvoreMinima resultado;
        resultado.descricao = "Prim";

        if (isDirecionado) {
            resultado.descricao += " (nao suportado para grafos direcionados)";
            return resultado;
        }

        int totalVertices = numeroVertices();
        if (totalVertices == 0) {
            resultado.sucesso = true;
            return resultado;
        }

        auto inicio = chrono::steady_clock::now();
        vector<bool> verticeJaInserido(totalVertices, false);
        verticeJaInserido[0] = true;
        int quantidadeInserida = 1;

        while (quantidadeInserida < totalVertices) {
            // Busca a menor aresta que liga a árvore atual a qualquer vértice fora dela.
            float melhorPeso = numeric_limits<float>::infinity();
            int melhorU = -1;
            int melhorV = -1;

            for (int u = 0; u < totalVertices; ++u) {
                if (!verticeJaInserido[u]) continue;
                vector<int> vizinhos = retornarVizinhos(u);
                for (int v : vizinhos) {
                    if (verticeJaInserido[v]) continue;
                    float peso = pesoAresta(u, v);
                    if (std::isnan(peso)) peso = 1.0f;
                    if (peso < melhorPeso) {
                        melhorPeso = peso;
                        melhorU = u;
                        melhorV = v;
                    }
                }
            }

            if (melhorU == -1 || melhorV == -1) break;

            verticeJaInserido[melhorV] = true;
            quantidadeInserida += 1;

            resultado.arestas.emplace_back(melhorU, melhorV);
            resultado.somaPesos += static_cast<double>(melhorPeso);
        }
        auto fim = chrono::steady_clock::now();

        resultado.tempoMs = chrono::duration<double, std::milli>(fim - inicio).count();
        bool todos = (quantidadeInserida == totalVertices);
        resultado.sucesso = todos;
        if (!todos) {
            resultado.descricao += " (falhou: grafo desconexo)";
        }
        return resultado;
    }

    ResultadoArvoreMinima arvoreGeradoraKruskal() {
        ResultadoArvoreMinima resultado;
        resultado.descricao = "Kruskal";

        if (isDirecionado) {
            resultado.descricao += " (nao suportado para grafos direcionados)";
            return resultado;
        }

        int totalVertices = numeroVertices();
        if (totalVertices == 0) {
            resultado.sucesso = true;
            return resultado;
        }

        struct ArestaKruskal {
            int u;
            int v;
            float peso;
        };

        vector<ArestaKruskal> arestas;
        arestas.reserve(totalVertices * 2);
        for (int origem = 0; origem < totalVertices; ++origem) {
            vector<int> vizinhos = retornarVizinhos(origem);
            for (int destino : vizinhos) {
                if (!isDirecionado && destino <= origem) continue;
                float pesoArestaAtual = pesoAresta(origem, destino);
                if (std::isnan(pesoArestaAtual)) pesoArestaAtual = 1.0f;
                arestas.push_back({origem, destino, pesoArestaAtual});
            }
        }

        sort(arestas.begin(), arestas.end(), [](const ArestaKruskal& a, const ArestaKruskal& b){
            if (a.peso != b.peso) return a.peso < b.peso;
            if (a.u != b.u) return a.u < b.u;
            return a.v < b.v;
        });

        vector<int> representante(totalVertices);
        iota(representante.begin(), representante.end(), 0);

        function<int(int)> encontrarRepresentante = [&](int vertice) {
            while (representante[vertice] != vertice) {
                vertice = representante[vertice];
            }
            return vertice;
        };

        auto unirComponentes = [&](int verticeA, int verticeB) {
            int raizA = encontrarRepresentante(verticeA);
            int raizB = encontrarRepresentante(verticeB);
            if (raizA == raizB) return false;
            representante[raizB] = raizA;
            return true;
        };

        auto inicio = chrono::steady_clock::now();
        for (const auto& aresta : arestas) {
            if (unirComponentes(aresta.u, aresta.v)) {
                resultado.arestas.emplace_back(aresta.u, aresta.v);
                resultado.somaPesos += static_cast<double>(aresta.peso);
                if (resultado.arestas.size() == static_cast<size_t>(totalVertices - 1))
                    break;
            }
        }
        auto fim = chrono::steady_clock::now();

        resultado.tempoMs = chrono::duration<double, std::milli>(fim - inicio).count();
        resultado.sucesso = (resultado.arestas.size() == static_cast<size_t>(totalVertices - 1));
        if (!resultado.sucesso) {
            resultado.descricao += " (falhou: grafo desconexo)";
        }
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
