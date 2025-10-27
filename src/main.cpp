#include <iostream>
#include <string>
#include <memory>
#include <iomanip>
#include <vector>
#include <limits>
#include <algorithm>
#include <functional>
#include <dirent.h>
#include <sys/stat.h>

#include "./include/InicializadorGrafo.h"

using namespace std;

auto listarArquivosTxt = [](const string& diretorioRaiz) {
    vector<string> arquivos;
    function<void(const string&)> explorar = [&](const string& dirAtual) {
        DIR* dir = opendir(dirAtual.c_str());
        if (!dir) return;
        struct dirent* entry = nullptr;
        while ((entry = readdir(dir)) != nullptr) {
            string nome = entry->d_name;
            if (nome == "." || nome == "..") continue;
            string caminho = dirAtual + "/" + nome;

            struct stat info {};
            if (stat(caminho.c_str(), &info) != 0) {
                continue;
            }

            if (S_ISDIR(info.st_mode)) {
                explorar(caminho);
            } else if (S_ISREG(info.st_mode)) {
                if (nome.size() >= 4 &&
                    nome.substr(nome.size() - 4) == ".txt") {
                    arquivos.push_back(caminho);
                }
            }
        }
        closedir(dir);
    };

    explorar(diretorioRaiz);
    sort(arquivos.begin(), arquivos.end());
    return arquivos;
};

int main(int argc, char** argv) {
    string caminho;
    const string caminhoPadrao = "grafo_teste_grande.txt";

    if (argc >= 2) {
        caminho = argv[1];
    } else {
        const string pastaExemplos = "grafos_exemplos";
        vector<string> arquivosTxt = listarArquivosTxt(pastaExemplos);

        cout << "==========================" << endl;
        cout << "Selecione o arquivo de grafo" << endl;
        if (arquivosTxt.empty()) {
            cout << "(Nenhum arquivo .txt encontrado em " << pastaExemplos << ")" << endl;
        } else {
            for (size_t i = 0; i < arquivosTxt.size(); ++i) {
                cout << (i + 1) << " - " << arquivosTxt[i] << endl;
            }
        }
        cout << "0 - Informar caminho manualmente" << endl;
        cout << "==========================" << endl;

        int escolhaArquivo = -1;
        cin >> escolhaArquivo;

        if (!cin) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            escolhaArquivo = -1;
        }

        if (escolhaArquivo == 0) {
            cout << "Digite o caminho do arquivo: ";
            cin >> caminho;
        } else if (escolhaArquivo >= 1 &&
                   static_cast<size_t>(escolhaArquivo) <= arquivosTxt.size()) {
            caminho = arquivosTxt[escolhaArquivo - 1];
        } else {
            cout << "Opcao invalida. Usando arquivo padrao: "
                 << caminhoPadrao << endl;
            caminho = caminhoPadrao;
        }
    }

    if (caminho.empty()) {
        caminho = caminhoPadrao;
    }
    cout << "Arquivo selecionado: " << caminho << endl;

    cout << "1-Matriz | 2-Lista" << endl;
    int opcao = 0;
    cin >> opcao;

    unique_ptr<Grafos> grafo = inicializarGrafo(caminho, (opcao == 1 ? 1 : 2));
    if (!grafo) {
        cout << "Falha ao carregar o grafo!" << endl;
        return 1;
    }

    auto imprimirColoracao = [&](const Grafos::ResultadoColoracao& r) {
        cout << r.descricao << endl;
        if (!r.sucesso) {
            cout << "  Resultado indisponivel." << endl;
            return;
        }
        cout << fixed << setprecision(4);
        cout << "  Tempo: " << r.tempoMs << " ms" << endl;
        cout << defaultfloat << setprecision(6);
        cout << "  Cores utilizadas: " << r.numeroCores << endl;

        int n = grafo->numeroVertices();
        if (n < 10 && r.cores.size() == static_cast<size_t>(n)) {
            for (int v = 0; v < n; ++v) {
                cout << "    " << grafo->labelVertice(v)
                     << " -> cor " << (r.cores[v] + 1) << endl;
            }
        }
    };

    auto imprimirArvore = [&](const Grafos::ResultadoArvoreMinima& r) {
        cout << r.descricao << endl;
        if (!r.sucesso) {
            cout << "  Resultado indisponivel." << endl;
            return;
        }
        cout << fixed << setprecision(4);
        cout << "  Tempo: " << r.tempoMs << " ms" << endl;
        cout << "  Soma dos pesos: " << r.somaPesos << endl;
        cout << defaultfloat << setprecision(6);

        int n = grafo->numeroVertices();
        if (n > 0 && n < 10 && r.arestas.size() == static_cast<size_t>(n - 1)) {
            for (const auto& aresta : r.arestas) {
                int u = aresta.first;
                int v = aresta.second;
                cout << "    " << grafo->labelVertice(u)
                     << " - " << grafo->labelVertice(v) << endl;
            }
        }
    };

    cout << "==========================" << endl;
    cout << "Selecione o tipo de algoritmo" << endl;
    cout << "1 - Coloracao de Grafos" << endl;
    cout << "2 - Arvore Geradora Minima" << endl;
    int modo = 0;
    cin >> modo;
    cout << endl;

    switch (modo) {
    case 1: {
        cout << "==========================" << endl;
        cout << "Coloracao de Grafos" << endl;
        cout << "==========================" << endl;
        cout << endl;

        auto resForca = grafo->coloracaoForcaBruta();
        imprimirColoracao(resForca);
        auto resGulosa = grafo->coloracaoGulosaSimples();
        imprimirColoracao(resGulosa);
        auto resWelsh = grafo->coloracaoWelshPowell();
        imprimirColoracao(resWelsh);
        auto resDSatur = grafo->coloracaoDSatur();
        imprimirColoracao(resDSatur);
        break;
    }
    case 2: {
        cout << "==========================" << endl;
        cout << "Arvore Geradora Minima" << endl;
        cout << "==========================" << endl;
        cout << endl;

        auto resPrim = grafo->arvoreGeradoraPrim();
        imprimirArvore(resPrim);
        auto resKruskal = grafo->arvoreGeradoraKruskal();
        imprimirArvore(resKruskal);
        break;
    }
    default:
        cout << "Opcao invalida." << endl;
        break;
    }

    return 0;
}
