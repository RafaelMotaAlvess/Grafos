#include <iostream>
#include <string>
#include <memory>
#include <iomanip>

#include "./include/InicializadorGrafo.h"

using namespace std;

int main(int argc, char** argv) {
    int opcao;
    string caminho = (argc >= 2 ? argv[1] : string("grafo_teste_grande.txt"));

    cout << "1-Matriz | 2-Lista" << endl;
    cin >> opcao;

    unique_ptr<Grafos> grafo = inicializarGrafo(caminho, (opcao == 1 ? 1 : 2));
    if (!grafo) {
        cout << "Falha ao carregar o grafo!" << endl;
        return 1;
    }

    // cout << "==========================" << endl;
    // grafo->imprimirGrafo();

    cout << "==========================" << endl;
    cout << "Coloracao de Grafos" << endl;

    auto imprimirResultado = [&](const Grafos::ResultadoColoracao& r) {
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

    // auto resForca = grafo->coloracaoForcaBruta();
    // imprimirResultado(resForca);
    // auto resGulosa = grafo->coloracaoGulosaSimples();
    // imprimirResultado(resGulosa);
    auto resWelsh = grafo->coloracaoWelshPowell();
    imprimirResultado(resWelsh);
    auto resDSatur = grafo->coloracaoDSatur();
    imprimirResultado(resDSatur);

    return 0;
}
