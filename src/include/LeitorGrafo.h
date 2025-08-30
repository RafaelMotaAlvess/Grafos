#ifndef LEITORGRAFO_H
#define LEITORGRAFO_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

class LeitorGrafo {
private:
    string caminhoArquivo;
    int vertices;
    int arestas;
    bool isDirecionado;
    bool isPonderado;
    vector<vector<string>> dados;

public:
    LeitorGrafo(string caminho)
        : caminhoArquivo(caminho), vertices(0), arestas(0), 
          isDirecionado(false), isPonderado(false) {}

    bool carregarArquivo() {
        ifstream MyReadFile(caminhoArquivo);
        if (!MyReadFile.is_open()) {
            cerr << "Erro ao abrir arquivo: " << caminhoArquivo << endl;
            return false;
        }

        string text;
        int linhaAtual = 0;

        while (getline(MyReadFile, text)) {
            stringstream ss(text);
            string elemento;

            if (linhaAtual == 0) {
                ss >> elemento;
                vertices = stoi(elemento);

                ss >> elemento;
                arestas = stoi(elemento);

                ss >> elemento;
                isDirecionado = (stoi(elemento) != 0);

                ss >> elemento;
                isPonderado = (stoi(elemento) != 0);
            } else {
                vector<string> linha;
                while (ss >> elemento) {
                    linha.push_back(elemento);
                }
                if (!linha.empty()) {
                    dados.push_back(linha);
                }
            }
            linhaAtual++;
        }

        MyReadFile.close();
        return true;
    }

    // Getters
    int getVertices() const { return vertices; }
    int getArestas() const { return arestas; }
    bool getIsDirecionado() const { return isDirecionado; }
    bool getIsPonderado() const { return isPonderado; }
    const vector<vector<string>>& getDados() const { return dados; }

    void imprimirInfo() const {
        cout << "Vertices: " << vertices << endl;
        cout << "Arestas: " << arestas << endl;
        cout << "Direcionado: " << (isDirecionado ? "sim" : "nao") << endl;
        cout << "Ponderado: " << (isPonderado ? "sim" : "nao") << endl;

        cout << "Dados do arquivo:" << endl;
        for (auto &linha : dados) {
            for (auto &el : linha) {
                cout << "[" << el << "] ";
            }
            cout << endl;
        }
    }
};

#endif
