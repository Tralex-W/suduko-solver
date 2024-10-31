from pulp import * #Bibilothe um das BILP zu lösen
import math #für sqrt (Wurzel)

N = 9
input_board = [
                       [0, 3, 0, 0, 7, 0, 0, 0, 0],
                       [6, 0, 0, 1, 9, 5, 0, 0, 0],
                       [0, 9, 8, 0, 0, 2, 0, 6, 0],
                       [8, 0, 0, 0, 6, 0, 0, 0, 3],
                       [4, 0, 0, 8, 0, 3, 0, 0, 1],
                       [7, 0, 0, 0, 2, 0, 0, 0, 6],
                       [0, 6, 0, 0, 0, 0, 2, 8, 0],
                       [0, 0, 0, 4, 1, 9, 0, 0, 5],
                       [0, 0, 0, 0, 8, 0, 0, 7, 9]]


def generate_string_value_sequence(): #Alle Zahlen von 1 bis N
    global N

    return [number for number in range(1, N+1)]

def add_exisiting_values_to_problem_set(problem_set, input_board, board): #Fügt die bereits gesetzten Zellen hinzu
    for row in range(N):
        for col in range(N):
            val = input_board[row][col]

            if (val > 0):
                problem_set += board[val][row+1][col+1] == 1
    
    return problem_set

def get_subgrid_indexes(sequence): #List einer Liste, aller Positionen einer Submatritze 
    global N

    subgrid_dimension = int(math.sqrt(N))
    subgrid_indexes = []
    
    for row in range(subgrid_dimension):
        for col in range(subgrid_dimension):
            subgrid_indexes += [[(sequence[subgrid_dimension * row + a], sequence[ subgrid_dimension * col + b]) for a in range(subgrid_dimension) for b in range(subgrid_dimension)]]

    return subgrid_indexes

def print_solution(sequence, board): #Ausgabe der Lösung 
    global N

    for row in sequence:
        for col in sequence:
            for val in sequence:
                if value(board[val][row][col]) == 1:    
                    print(val, end=" ")
        print()   

def add_constrains(sequence, subgrids, problem_set, board): #Nebenbedingungen (Summen)
    for row in sequence:
        for col in sequence:
            problem_set += lpSum([board[val][row][col] for val in sequence]) == 1

    for val in sequence:
        for row in sequence:
            problem_set += lpSum([board[val][row][col] for col in sequence]) == 1
            
        for col in sequence:
            problem_set += lpSum([board[val][row][col] for row in sequence]) == 1

        for sub in subgrids:
            problem_set += lpSum([board[val][row][col] for (row, col) in sub]) == 1

    return problem_set


def main():
    sequence = generate_string_value_sequence()

    problem_set = LpProblem("Sudoku Solver", LpMinimize)
    board = LpVariable.dicts("Board", (sequence, sequence, sequence), 0, 1, LpBinary) #Drei dimensionales Dictionary board[value][row][col] ist binär. 1 = v in (row, coloum) gestzt, 0 nicht

    subgrids = get_subgrid_indexes(sequence)

    problem_set = add_constrains(sequence, subgrids, problem_set, board)
    problem_set = add_exisiting_values_to_problem_set(problem_set, input_board, board)

    problem_set.solve()

    print_solution(sequence, board)

main()