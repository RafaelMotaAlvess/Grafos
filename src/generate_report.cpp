#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "./include/InicializadorGrafo.h"

using namespace std;

namespace {
string formatarDouble(double valor, int precisao = 4) {
    ostringstream oss;
    oss << fixed << setprecision(precisao) << valor;
    return oss.str();
}

struct OutputPair {
    ostream& console;
    ofstream& file;

    void write(const string& line) {
        console << line << '\n';
        file << line << '\n';
    }

    void blank() { write(""); }
};

void relatarColoracao(OutputPair& out,
                      const string& arquivo,
                      Grafos& grafo) {
    vector<Grafos::ResultadoColoracao> heuristicas;
    heuristicas.push_back(grafo.coloracaoGulosaSimples());
    heuristicas.push_back(grafo.coloracaoWelshPowell());
    heuristicas.push_back(grafo.coloracaoDSatur());

    for (const auto& resultado : heuristicas) {
        out.write("Executando \"" + arquivo + "\" utilizando \"" + resultado.descricao + "\"");
        if (!resultado.sucesso) {
            out.write("  Resultado indisponivel.");
            out.blank();
            continue;
        }

        out.write("  Tempo (ms): " + formatarDouble(resultado.tempoMs));
        out.write("  Numero de cores: " + to_string(resultado.numeroCores));
        out.blank();
    }
}

void relatarArvoreMinima(OutputPair& out,
                         const string& arquivo,
                         Grafos& grafo) {
    vector<Grafos::ResultadoArvoreMinima> algoritmos;
    algoritmos.push_back(grafo.arvoreGeradoraPrim());
    algoritmos.push_back(grafo.arvoreGeradoraKruskal());

    for (const auto& resultado : algoritmos) {
        out.write("Executando \"" + arquivo + "\" utilizando \"" + resultado.descricao + "\"");
        if (!resultado.sucesso) {
            out.write("  Resultado indisponivel.");
            out.blank();
            continue;
        }

        out.write("  Tempo (ms): " + formatarDouble(resultado.tempoMs));
        out.write("  Soma total (AGM): " + formatarDouble(resultado.somaPesos));
        out.blank();
    }
}
} // namespace

int main() {
    const vector<string> instanciasColoracao = {
        "grafos_exemplos/grafos_coloridos/C4000-260-X.txt",
        "grafos_exemplos/grafos_coloridos/k33.txt",
        "grafos_exemplos/grafos_coloridos/k5.txt",
        "grafos_exemplos/grafos_coloridos/kquase5.txt",
        "grafos_exemplos/grafos_coloridos/r1000-234-234.txt",
        "grafos_exemplos/grafos_coloridos/r250-66-65.txt",
    };

    const vector<string> instanciasArvore = {
        "grafos_exemplos/grafos_mst/1000vertices25%Arestas.txt",
        "grafos_exemplos/grafos_mst/500vertices100%Arestas.txt",
        "grafos_exemplos/grafos_mst/500vertices25%Arestas.txt",
        "grafos_exemplos/grafos_mst/500vertices50%Arestas.txt",
        "grafos_exemplos/grafos_mst/50vertices100%Arestas.txt",
        "grafos_exemplos/grafos_mst/50vertices25%Arestas.txt",
        "grafos_exemplos/grafos_mst/50vertices50%Arestas.txt",
    };

    ofstream arquivoSaida("relatorio_execucoes.txt");
    if (!arquivoSaida.is_open()) {
        cerr << "Falha ao criar relatorio_execucoes.txt" << endl;
        return 1;
    }

    OutputPair output{cout, arquivoSaida};

    for (const auto& caminho : instanciasColoracao) {
        unique_ptr<Grafos> grafo = inicializarGrafo(caminho, 2);
        if (!grafo) {
            output.write("Nao foi possivel carregar o grafo de " + caminho);
            output.blank();
            continue;
        }
        relatarColoracao(output, caminho, *grafo);
    }

    for (const auto& caminho : instanciasArvore) {
        unique_ptr<Grafos> grafo = inicializarGrafo(caminho, 2);
        if (!grafo) {
            output.write("Nao foi possivel carregar o grafo de " + caminho);
            output.blank();
            continue;
        }
        relatarArvoreMinima(output, caminho, *grafo);
    }

    output.write("Relatorio salvo em relatorio_execucoes.txt");
    return 0;
}
