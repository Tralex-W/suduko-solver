
#include <iostream>
#include <chrono> //0,002s

using namespace std;

#define n 9

void print_board(int board[n][n]);

bool possible_move(int board[n][n], int i, int j, int k);

bool solve(int board[n][n], int i = 0, int j = 0);

int main() // hauptfunktion handelt den Ablauf des Programms

{

    int board[n][n] = {// definition anfänglichen des Spielfeldes
                       {0, 0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0, 5},
                       {0, 0, 0, 5, 0, 0, 0, 8, 0},
                       {0, 0, 0, 0, 8, 0, 0, 0, 0},
                       {0, 8, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 9, 8},
                       {0, 0, 6, 0, 0, 0, 0, 0, 0}};

    print_board(board);

    auto start = chrono::high_resolution_clock::now();
    solve(board);
    auto end = chrono::high_resolution_clock::now();

    cout << "\n";

    print_board(board);

    chrono::duration<double> duration = end - start;
    cout << "Duration: " << duration.count() << " seconds" << "\n";
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

bool possible_move(int board[n][n], int i, int j, int k) // überprüft ob es valide ist die Zahl k an die position i,j zu setzten

{

    for (int a = 0; a < n; a++)

    {

        if (board[i][a] == k) // überprüfung, ob die selbe Zahl in der gleichn Zeile vorkommt

            return false;

        if (board[a][j] == k) // überprüfung, ob die selbe Zahl in der gleichn Spalte vorkommt

        {

            return false;
        }

        if (board[i - i % 3 + a / 3][j - j % 3 + a % 3] == k) // überprüfung, ob die selbe Zahl in der submatritze vorkommt

            return false;
    }

    return true;
}

// i gibt die Zeile an

// j gibt die Spalte an

bool solve(int board[n][n], int i, int j) // Ist für das lösen des Sudokos durch Bruteforce Backtracking tätig

{

    if (i == n) // wenn alle Zeilen und damit auch alle Splaten erfolgreich ausgefüllt wurden, ist das Sudoku Spiel gelößt

        return true;

    if (j == n) // Wenn die Splatenangabe über die Diemension geht, muss die erste Zelle der nächsten Zeile betrachtet werden

        return solve(board, i + 1, 0);

    if (board[i][j] != 0) // Wenn keine leere Zelle an der aktullen Position vorhanden ist, wird die nächste Position betrachtet

        return solve(board, i, j + 1);

    for (int k = 1; k <= n; ++k) // Alle Validen Zahlen von 1-9 werden linear ausprobiert

    {

        if (!possible_move(board, i, j, k)) // Ist die aktuelle Zahl nicht als Valid in der aktuellen Position zu betrachten, wird die nächste Zahl betrachtet

            continue;

        board[i][j] = k; // Die vermeintlich valide Zahl wird ausprobiert

        if (solve(board, i, j + 1)) // kommen keine weiteren Konfilkte auf, ist dies die richtie lösung

            return true;

        board[i][j] = 0;
    }

    return false; // Falls keine der n Zahlen valide ist, ist ein Konfilkt aufgetreten
}
