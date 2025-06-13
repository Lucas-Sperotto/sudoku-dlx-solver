# Sudoku DLX Solver

Este projeto implementa um resolvedor de Sudoku em C++ baseado no algoritmo **Dancing Links (DLX)** proposto por Donald Knuth.

## 🔢 Sobre o algoritmo

O DLX resolve o problema de cobertura exata usando uma estrutura de dados altamente eficiente com listas duplamente encadeadas circulares. O Sudoku é modelado como um problema com 729 possíveis jogadas e 324 restrições.

## 🚀 Como compilar

```bash
g++ -O2 -std=c++11 -o sudoku_dlx sudoku_dlx_solver.cpp
./sudoku_dlx
````

## 🧩 Exemplo de entrada (interna no código)

```cpp
int puzzle[N][N] = {
    {5, 3, 0, 0, 7, 0, 0, 0, 0},
    {6, 0, 0, 1, 9, 5, 0, 0, 0},
    {0, 9, 8, 0, 0, 0, 0, 6, 0},
    {8, 0, 0, 0, 6, 0, 0, 0, 3},
    {4, 0, 0, 8, 0, 3, 0, 0, 1},
    {7, 0, 0, 0, 2, 0, 0, 0, 6},
    {0, 6, 0, 0, 0, 0, 2, 8, 0},
    {0, 0, 0, 4, 1, 9, 0, 0, 5},
    {0, 0, 0, 0, 8, 0, 0, 7, 9}
};
```

## 📚 Referência

* [Knuth, D. (2000). Dancing Links](https://arxiv.org/abs/cs/0011047)

## 📄 Licença

Este projeto está sob a Licença MIT - veja o arquivo [LICENSE](LICENSE) para detalhes.
