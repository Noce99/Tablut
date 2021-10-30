import player
from typing import Union
import numpy as np
import random
import copy

coordinates = np.array([['A1', 'B1', 'C1', 'D1', 'E1', 'F1', 'G1', 'H1', 'I1'],
                        ['A2', 'B2', 'C2', 'D2', 'E2', 'F2', 'G2', 'H2', 'I2'],
                        ['A3', 'B3', 'C3', 'D3', 'E3', 'F3', 'G3', 'H3', 'I3'],
                        ['A4', 'B4', 'C4', 'D4', 'E4', 'F4', 'G4', 'H4', 'I4'],
                        ['A5', 'B5', 'C5', 'D5', 'E5', 'F5', 'G5', 'H5', 'I5'],
                        ['A6', 'B6', 'C6', 'D6', 'E6', 'F6', 'G6', 'H6', 'I6'],
                        ['A7', 'B7', 'C7', 'D7', 'E7', 'F7', 'G7', 'H7', 'I7'],
                        ['A8', 'B8', 'C8', 'D8', 'E8', 'F8', 'G8', 'H8', 'I8'],
                        ['A9', 'B9', 'C9', 'D9', 'E9', 'F9', 'G9', 'H9', 'I9']])


# state -> matrice 9x9 con lo stato della partita
# player -> instance of Player (self.B or self.W where self is Game)

# return move
#   black move -> random_soldier, None, random_move
#   king move  -> return "K", None, random_move
#   white move -> return "S", random_soldier, random_move
def min_max_player(state: np.array, pl: Union[player.WPlayer, player.BPlayer]):
    next_states = find_all_possible_states(state, pl)
    # Implementare di seguito la vera funzione min_max che ritorni next_state invece di calcolarlo casualmente
    if isinstance(pl, player.WPlayer):
        next_state = min_max_evaluation(next_states, "W")
    else:
        next_state = min_max_evaluation(next_states, "B")
    # --------------------------------------------------------------------------------------------------------
    return get_move_from_matrix(next_state, state)


def find_all_possible_states(state: np.array, pl: Union[player.WPlayer, player.BPlayer]):
    next_states = []
    for i, e in enumerate(pl.soldiers):
        moves = pl.moves[i]
        for m in moves:
            next_states.append(get_matrix_from_moves(copy.deepcopy(state), (e, m)))
    if isinstance(pl, player.WPlayer):
        for m in pl.king_moves:
            next_states.append(get_matrix_from_moves(copy.deepcopy(state), (pl.king, m)))
    return next_states


# Return the next move matrix
# state -> actual state
# moves -> (actual position, next position)
# return next_state
def get_matrix_from_moves(state: np.array, move: (str, str)):
    start = player.Player.pos_in_numbers_from_string(move[0])
    finish = player.Player.pos_in_numbers_from_string(move[1])
    state[finish[1], finish[0]] = state[start[1], start[0]]
    state[start[1], start[0]] = 0
    return state


# sbagliatissimo, da cambiare
def get_move_from_matrix(new_state: np.array, old_state: np.array):
    diff = new_state != old_state
    new_num = new_state[diff]
    str_pos = coordinates[diff]
    if new_num[0] == 0:
        start = str_pos[0]
        finish = str_pos[1]
    else:
        start = str_pos[1]
        finish = str_pos[0]
    # print(start, "to", finish)
    if np.sum(new_num) == 2:
        # Black is moving
        return start, None, finish
    elif np.sum(new_num) == 1:
        # White soldier is moving
        return "S", start, finish
    else:
        # White king is moving
        return "K", None, finish


# Funzione che decide che mossa fare
# Parametro ricevuto:
# states_list -> lista dei possibili futuri stati (mosse che possono essere fatte)
#                si tratta di una lista di np.array (9x9) dove 0 è una cella vuota,
#                1 è una pedina bianca, 2 è una pedina nera, 3 è il re bianco
# Ritorna:
# selected_state -> stato che ha deciso essere la prossima mossa da fare
def min_max_evaluation(states_list: np.array, color: str) -> np.array:
    selected_state = states_list[random.randrange(0, len(states_list))]
    return selected_state
