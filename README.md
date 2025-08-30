# Trabalho M1 – Parte 1 (Grafos)

## Descrição
- Objetivo: implementar um grafo e as funções básicas de seu funcionamento nas duas representações pedidas: por lista de adjacência e por matriz de adjacência.
- Escopo: as funções listadas nos slides S02 – Conceitos Básicos e Representação devem existir em AMBAS as representações.

No código atual há uma interface comum (`src/include/Grafos.h`) e duas implementações:
- `GrafoMatriz` (`src/include/GrafoMatriz.h`): estrutura de matriz de adjacência, já com inserção de vértices/arestas e impressão básica da matriz.
- `GrafoLista` (`src/include/GrafoLista.h`): estrutura por lista de adjacência (a serem implementados).

O leitor de arquivo (`src/include/LeitorGrafo.h`) carrega o grafo a partir de um arquivo de texto e informa se o grafo é direcionado e/ou ponderado.

## Como Rodar
Pré‑requisitos: `g++` instalado. O `Makefile` linka `-lGL -lglut` (não utilizados no momento); se não tiver essas libs, remova-as do `Makefile` ou instale as dependências.

1) Compilar:
- `make`

2) Executar:
- `./main`

O programa lê automaticamente o arquivo `filename.txt` na raiz do projeto (caminho está hardcoded em `src/main.cpp`). Para usar outro arquivo, altere o caminho em `src/main.cpp:12`.

## Formato do Arquivo de Entrada
Primeira linha: `V A D P`
- `V`: número de vértices
- `A`: número de arestas
- `D`: 1 se direcionado, 0 caso contrário
- `P`: 1 se ponderado, 0 caso contrário

Demais linhas: uma aresta por linha
- Não ponderado: `origem destino`
- Ponderado: `origem destino peso`

Exemplo (`filename.txt`):
```
3 4 0 1
0 1 5
0 2 2
1 2 7
2 0 1
```

No `main`, os vértices são adicionados com rótulos automáticos `A`, `B`, `C`, …

## Estrutura
- `src/main.cpp`: orquestra leitura do arquivo e popula o `GrafoMatriz`.
- `src/include/Grafos.h`: interface base com as operações exigidas.
- `src/include/GrafoMatriz.h`: implementação por matriz de adjacência.
- `src/include/GrafoLista.h`: implementação por lista de adjacência (a implementar).
- `src/include/LeitorGrafo.h`: leitor do arquivo de entrada.

## TODO
Itens marcados com [x] estão implementados ao menos de forma básica; [ ] pendentes ou apenas com stubs/mocks.

Geral
- [x] Interface comum `Grafos` com operações básicas
- [x] Leitor de arquivo (`LeitorGrafo`) com flags direcionado/ponderado
- [x] População do grafo a partir do arquivo no `main`
- [x] Impressão da matriz de adjacência (com cabeçalhos)

GrafoMatriz (`src/include/GrafoMatriz.h`)
- [x] `adicionarVertice(label)` – ajusta matriz e labels
- [x] `adicionarAresta(origem, destino, peso)` – respeita direcionado/não direcionado
- [x] `imprimirGrafo()` – imprime matriz com labels e largura ajustada
- [x] `pesoAresta(origem, destino)` – retorna valor da matriz
- [x] `removerVertice(index)` – remover linha/coluna e label correspondente
- [x] `removerAresta(origem, destino)` – zerar entrada (e simétrica se não direcionado)
- [x] `labelVertice(index)` – retornar label correto (hoje retorna placeholder)
- [x] `existeAresta(origem, destino)` – verificar > 0 (ou diferente de "ausência"/∞)
- [ ] `retornarVizinhos(vertice)` – índices j com aresta válida a partir de `vertice`
- [ ] `bfs()` – busca em largura usando a matriz
- [ ] `dfs()` – busca em profundidade usando a matriz

GrafoLista (`src/include/GrafoLista.h`)
- [ ] Estrutura interna (e.g., `vector<vector<pair<int,float>>>` + `labels`)
- [ ] `adicionarVertice(label)`
- [ ] `adicionarAresta(origem, destino, peso)` – respeitar direcionado/ponderado
- [ ] `removerVertice(index)` – atualizar lista e reindexações necessárias
- [ ] `removerAresta(origem, destino)`
- [ ] `labelVertice(index)`
- [ ] `imprimirGrafo()` – formato legível (lista por vértice)
- [ ] `existeAresta(origem, destino)`
- [ ] `pesoAresta(origem, destino)`
- [ ] `retornarVizinhos(vertice)`
- [ ] `bfs()`
- [ ] `dfs()`

Sugestões rápidas
- Se não usar OpenGL/GLUT, simplifique o `Makefile` removendo `-lGL -lglut`.
- Parametrize o caminho do arquivo de entrada via argumento CLI (e.g., `./main arquivo.txt`).
