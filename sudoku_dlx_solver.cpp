// Sudoku Solver via Dancing Links (DLX) - Donald Knuth
// Implementado e comentado em C++
#include <iostream>
#include <vector>
using namespace std;

const int N = 9; // Tamanho do Sudoku (9x9)
const int SIZE = N * N * N; // Total de possíveis jogadas (posição + número)
const int CONSTRAINTS = 4 * N * N; // Total de restrições (célula, linha, coluna, bloco)

// Nó básico da estrutura de Dancing Links
struct Node {
    int rowID, colID;
    Node *left, *right, *up, *down;
    Node *colHeader;
};

Node *column[CONSTRAINTS]; // Cabeçalhos de colunas
int colSize[CONSTRAINTS];  // Número de linhas por coluna
Node *root;                // Nó raiz da matriz de cobertura
vector<Node*> solution;    // Caminho de solução atual

// Função auxiliar para criar um novo nó
Node* newNode(int rowID, int colID) {
    Node *n = new Node();
    n->rowID = rowID;
    n->colID = colID;
    n->left = n->right = n->up = n->down = n;
    return n;
}

// Inicializa as colunas com cabeçalhos e ligações horizontais
void initializeDLX() {
    root = newNode(-1, -1);
    Node *last = root;
    for (int c = 0; c < CONSTRAINTS; ++c) {
        column[c] = newNode(-1, c);
        column[c]->colHeader = column[c];
        colSize[c] = 0;
        // Ligação horizontal
        last->right = column[c];
        column[c]->left = last;
        last = column[c];
    }
    last->right = root;
    root->left = last;
    // Inicializa ligações verticais
    for (int c = 0; c < CONSTRAINTS; ++c) {
        column[c]->up = column[c]->down = column[c];
    }
}

// Adiciona uma linha à matriz (uma possível jogada)
void addRow(int rowID, const vector<int>& cols) {
    Node *first = nullptr, *last = nullptr;
    for (int colID : cols) {
        Node *n = newNode(rowID, colID);
        n->colHeader = column[colID];

        // Inserção vertical na coluna
        n->down = column[colID];
        n->up = column[colID]->up;
        column[colID]->up->down = n;
        column[colID]->up = n;

        ++colSize[colID];

        // Ligação horizontal entre os nós da linha
        if (!first) first = n;
        if (last) {
            last->right = n;
            n->left = last;
        }
        last = n;
    }
    if (first && last) {
        first->left = last;
        last->right = first;
    }
}

// Mapeia posição e valor para um ID único
int getRowID(int r, int c, int n) {
    return r * N * N + c * N + n;
}

// Tradução de (r, c, n) para as 4 restrições
vector<int> getConstraints(int r, int c, int n) {
    return {
        r * N + c,                  // Cada célula deve conter um número
        N * N + r * N + n,         // Cada linha deve conter cada número
        2 * N * N + c * N + n,     // Cada coluna deve conter cada número
        3 * N * N + (r / 3 * 3 + c / 3) * N + n // Cada bloco 3x3 deve conter cada número
    };
}

// Remove uma coluna e suas linhas (passo DLX)
void cover(Node *col) {
    col->right->left = col->left;
    col->left->right = col->right;
    for (Node *i = col->down; i != col; i = i->down) {
        for (Node *j = i->right; j != i; j = j->right) {
            j->down->up = j->up;
            j->up->down = j->down;
            --colSize[j->colID];
        }
    }
}

// Restaura uma coluna e suas linhas (passo DLX inverso)
void uncover(Node *col) {
    for (Node *i = col->up; i != col; i = i->up) {
        for (Node *j = i->left; j != i; j = j->left) {
            ++colSize[j->colID];
            j->down->up = j;
            j->up->down = j;
        }
    }
    col->right->left = col;
    col->left->right = col;
}

// Escolhe a melhor coluna (com menos linhas)
Node* chooseColumn() {
    Node *best = root->right;
    for (Node *c = best->right; c != root; c = c->right) {
        if (colSize[c->colID] < colSize[best->colID]) {
            best = c;
        }
    }
    return best;
}

// Backtracking principal do algoritmo DLX
bool solve() {
    if (root->right == root) return true; // Solução encontrada

    Node *col = chooseColumn();
    cover(col);

    for (Node *r = col->down; r != col; r = r->down) {
        solution.push_back(r);
        for (Node *j = r->right; j != r; j = j->right) {
            cover(j->colHeader);
        }
        if (solve()) return true;
        for (Node *j = r->left; j != r; j = j->left) {
            uncover(j->colHeader);
        }
        solution.pop_back();
    }

    uncover(col);
    return false;
}

// Constrói a matriz de cobertura exata para Sudoku
void buildMatrix() {
    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
            for (int n = 0; n < N; ++n) {
                int rowID = getRowID(r, c, n);
                addRow(rowID, getConstraints(r, c, n));
            }
        }
    }
}

// Aplica a grade inicial do Sudoku
void applyInitialGrid(int grid[N][N]) {
    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
            if (grid[r][c] != 0) {
                int n = grid[r][c] - 1;
                int rowID = getRowID(r, c, n);
                for (Node *j = root->right; j != root; j = j->right) {
                    for (Node *i = j->down; i != j; i = i->down) {
                        if (i->rowID == rowID) {
                            for (Node *k = i->right; k != i; k = k->right)
                                cover(k->colHeader);
                            cover(j);
                            solution.push_back(i);
                            goto next;
                        }
                    }
                }
            }
        next:;
        }
    }
}

// Imprime a solução encontrada
void printSolution() {
    int grid[N][N];
    for (auto r : solution) {
        int id = r->rowID;
        int row = id / (N * N);
        int col = (id / N) % N;
        int num = id % N;
        grid[row][col] = num + 1;
    }
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j)
            cout << grid[i][j] << " ";
        cout << "\n";
    }
}

int main() {
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

    initializeDLX();      // Inicializa estrutura
    buildMatrix();        // Constrói matriz de cobertura
    applyInitialGrid(puzzle); // Aplica entradas iniciais

    if (solve()) {
        cout << "\nSolução encontrada:\n";
        printSolution();
    } else {
        cout << "\nNenhuma solução encontrada.\n";
    }
    return 0;
}
