from random import choice
from math import inf

initial_board = [[0, 0, 0], [0, 0, 0], [0, 0, 0]]

def display_board(current_board):
    plays_dict = {
        1 : 'X',
        -1 : 'O',
        0 : ' ' 
    }
    print('---------------')
    for row in current_board:
        for elem in row:
            print(f'| {plays_dict[elem]} |', end='')
        print('\n' + '---------------')

def current_empty_places(current_board):
    list_empties = []

    for i, row in enumerate(current_board):
        for j, cell in enumerate(row):
            if current_board[i][j] == 0:
                list_empties.append((i, j))

    return list_empties

def evaluate_result(current_board):
    player = 1
    computer = -1

    X_evaluation = 1
    O_evaluation = -1

    if player_won(current_board, player):
        return X_evaluation
    elif player_won(current_board, computer):
        return O_evaluation
    else:
        return 0

def maximizer_move(current_board, alpha, beta):
    if is_game_won(current_board) or is_full_board(current_board):
        return evaluate_result(current_board), None

    max_evaluation = -inf
    best_move = None
    for (i, j) in current_empty_places(current_board):
        current_board[i][j] = 1
        curr_evaluation, _ = minimizer_move(current_board, alpha, beta)
        current_board[i][j] = 0

        if curr_evaluation > max_evaluation:
            max_evaluation = curr_evaluation
            best_move = (i, j)

        alpha = max(alpha, curr_evaluation)

        if beta <= alpha:
            break

    return max_evaluation, best_move

def minimizer_move(current_board, alpha, beta):
    if is_game_won(current_board) or is_full_board(current_board):
        return evaluate_result(current_board), None

    min_evaluation = inf
    best_move = None

    for (i, j) in current_empty_places(current_board):
        current_board[i][j] = -1
        curr_evaluation, _ = maximizer_move(current_board, alpha, beta)
        current_board[i][j] = 0

        if curr_evaluation < min_evaluation:
            min_evaluation = curr_evaluation
            best_move = (i, j)

        beta = min(beta, curr_evaluation)

        if beta <= alpha:
            break

    return min_evaluation, best_move

def find_best_move(current_board):
    alpha = -inf
    beta = inf

    _, best_move = minimizer_move(current_board, alpha, beta)

    if best_move != None:
        row_to_fill = best_move[0]
        column_to_fill = best_move[1]
        current_board[row_to_fill][column_to_fill] = -1

def player_turn(current_board):
    input_cell = eval(input('Enter row and column to write on [ex. (1, 2)]: '))
    row = input_cell[0] - 1
    column = input_cell[1] - 1

    current_board[row][column] = 1

def make_move(current_board, current_player):
    if current_player == 1:
        player_turn(current_board)
    else:
        find_best_move(current_board) # computer on row
        display_board(current_board)

def is_full_board(current_board):
    for i, row in enumerate(current_board):
        for j, _ in enumerate(row):
            if current_board[i][j] == 0:
                return False
    return True

def is_empty_board(current_board):
    for i, row in enumerate(current_board):
        for j, _ in enumerate(row):
            if current_board[i][j] == 1 or current_board[i][j] == -1:
                return False
    return True

def player_won(current_board, current_player_on_row):
    winning_combinations = [
        [current_board[0][0], current_board[0][1], current_board[0][2]],
        [current_board[1][0], current_board[1][1], current_board[1][2]],
        [current_board[2][0], current_board[2][1], current_board[2][2]],
        [current_board[0][0], current_board[1][0], current_board[2][0]],
        [current_board[0][1], current_board[1][1], current_board[2][1]],
        [current_board[0][2], current_board[1][2], current_board[2][2]],
        [current_board[0][0], current_board[1][1], current_board[2][2]],
        [current_board[0][2], current_board[1][1], current_board[2][0]]
    ]
    winning_row = [current_player_on_row, current_player_on_row, current_player_on_row]

    if winning_row in winning_combinations:
        return True

    return False

def print_winning_player(current_board):
    player_one = 1
    player_two = -1
    if player_won(current_board, player_one):
        print('X won!')
    elif player_won(current_board, player_two):
        print('O won!')
    else:
        print('Tie!')

def is_game_won(current_board):
    player_one = 1
    player_two = -1
    return player_won(current_board, player_one) or player_won(current_board, player_two)

def play():
    current_board = initial_board

    is_player_first = eval(input('Enter 1 for you first and 2 for you second: '))
    if is_player_first == 1:
        current_player = 1
    elif is_player_first == 2:
        current_player = -1

    while not (is_full_board(current_board) or is_game_won(current_board)):
        make_move(current_board, current_player)
        current_player *= -1

    print_winning_player(current_board)

if __name__ == '__main__':
    play()