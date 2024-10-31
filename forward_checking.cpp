#include <iostream>
#include <bitset>
#include <array>
#include <chrono> //0,0009s  122,22% schneller
using namespace std;

#define n 9

void print_board(int board[n][n]);
inline bool solve(array<bitset<n>, n * n> &domains, int board[n][n], int i = 0, int j = 0);
void initialize_domains(array<bitset<n>, n * n> &domains, int board[n][n]);
inline bool update_domains(array<bitset<n>, n * n> &domains, int board[n][n], int i, int j, int k);

int main() // hauptfunktion handelt den Ablauf des Programms
{
    int board[n][n] = {// definition anfänglichen des Spielfeldes
                       {5, 0, 0, 0, 0, 8, 0, 0, 0},
                       {0, 0, 0, 1, 0, 0, 6, 7, 0},
                       {0, 0, 0, 0, 6, 0, 0, 0, 5},
                       {0, 0, 0, 5, 0, 0, 0, 8, 0},
                       {0, 0, 0, 0, 8, 0, 0, 0, 0},
                       {0, 8, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 9, 8},
                       {0, 0, 6, 0, 0, 0, 0, 0, 0}};

    array<bitset<n>, n * n> domains;

    print_board(board);

    auto start = chrono::high_resolution_clock::now();
    initialize_domains(domains, board);
    solve(domains, board);
    auto end = chrono::high_resolution_clock::now();

    cout << "\n";
    print_board(board);

    chrono::duration<double> duration = end - start;
    cout << "Duration: " << duration.count() << " seconds" << endl;
}

void print_board(int board[n][n]) // zeigt das Sudoku Feld an
{
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            cout << board[i][j] << " ";
        }
        cout << "\n";
    }
}

// ausgehend von der Zelle einer Zelle werden die Domänmen der beeinflussten Zellen aktualisiert
// 1 =  möglich die Zahl zu setzten
// 0 =  nicht möglich die Zahl zu setzten
inline bool update_domains(array<bitset<n>, n * n> &domains, int board[n][n], int i, int j, int k)
{
    for (int a = 0; a < n; ++a)
    {
        domains[i * n + a].reset(k - 1); // reihe
        domains[a * n + j].reset(k - 1); // splate

        // Wenn es nicht mehr möglich ist eine Zahl an die Stelle zu setzten
        if (domains[i * n + a].none() && board[i][a] == 0)
        {
            return false;
        }

        if (domains[a * n + j].none() && board[a][j] == 0)
        {
            return false;
        }
    }

    for (int row = (i / 3) * 3; row < ((i / 3) * 3) + 3; ++row)
    {
        for (int col = (j / 3) * 3; col < ((j / 3) * 3) + 3; ++col)
        {
            domains[row * n + col].reset(k - 1); // Subgrid

            if (domains[row * n + col].none() && board[row][col] == 0)
            {
                return false;
            }
        }
    }

    return true;
}

// 1 =  möglich die Zahl zu setzten
// 0 =  nicht möglich die Zahl zu setzten
// Initialisiert die Bitsets für die schon vorhanden Zellen mit Werten
void initialize_domains(array<bitset<n>, n * n> &domains, int board[n][n])
{
    // Alle Zahlen pro Reihe, Spalte und Submatrize
    array<bitset<n>, n> row_constraints, col_constraints, subgrid_constraints;

    // Initialisierung der eben deklarierten Bitsets
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            if (board[i][j] != 0)
            {
                int num = board[i][j] - 1;
                row_constraints[i].set(num);
                col_constraints[j].set(num);
                subgrid_constraints[(i / 3) * 3 + (j / 3)].set(num);
            }
        }
    }

    // Initialisierung der Domänen für jede Zelle
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            if (board[i][j] == 0)
            {
                // Berechnet die Domain für die Zelle, indem alle belegten Zahlen in der entsprechenden Zeile, Spalte und dem Subgrid ausgeschlossen werden. Die übrig geliebenen Zahlen werden durch Verknüpfen und dann invertiert, sodass alle möglichen Werte für die Zelle erhalten bleiben
                domains[i * n + j] = ~(row_constraints[i] | col_constraints[j] | subgrid_constraints[(i / 3) * 3 + (j / 3)]);
            }
            else
            {
                // Schon ausgefüllte Zellen haben
                domains[i * n + j].reset();
            }
        }
    }
}

// i gibt die Zeile an
// j gibt die Spalte an
inline bool solve(array<bitset<n>, n * n> &domains, int board[n][n], int i, int j) // Ist für das lösen des Sudokos durch Bruteforce Backtracking tätig
{
    if (i == n) // wenn alle Zeilen und damit auch alle Splaten erfolgreich ausgefüllt wurden, ist das Sudoku Spiel gelößt
        return true;
    if (j == n) // Wenn die Splatenangabe über die Diemension geht, muss die erste Zelle der nächsten Zeile betrachtet werden
        return solve(domains, board, i + 1, 0);
    if (board[i][j] != 0) // Wenn keine leere Zelle an der aktullen Position vorhanden ist, wird die nächste Position betrachtet
        return solve(domains, board, i, j + 1);

    for (int k = 1; k <= n; ++k) // Alle Validen Zahlen von 1-9 werden linear ausprobiert
    {
        if (!domains[i * n + j].test(k - 1)) // Wenn die Zahl nicht in der Domäne der aktuellen Zelle vorhanden ist, kann sie übersprungen werden
            continue;

        array<bitset<n>, n * n> pre_updated_domain = domains;

        board[i][j] = k; // Die vermeintlich valide Zahl wird ausprobiert

        if (update_domains(domains, board, i, j, k)) // Kein Konfilkt
        {
            if (solve(domains, board, i, j + 1)) // kommen keine weiteren Konfilkte auf, ist dies die richtie lösung
                return true;
        }

        // Wenn die Zahl zu einem Konfilkt führt (in einer beeinflussten Zelle, kann keine Zahl mehr eingesetzt werden) wird diese nicht verwendet
        board[i][j] = 0;

        domains = pre_updated_domain;
    }

    return false; // Falls keine der n Zahlen valide ist, ist ein Konfilkt aufgetreten
}