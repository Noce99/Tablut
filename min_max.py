import player
from typing import Union
import numpy as np
import random
import copy
from numba import jit

MAX_DEPH = 3

coordinates = np.array([['A1', 'B1', 'C1', 'D1', 'E1', 'F1', 'G1', 'H1', 'I1'],
                        ['A2', 'B2', 'C2', 'D2', 'E2', 'F2', 'G2', 'H2', 'I2'],
                        ['A3', 'B3', 'C3', 'D3', 'E3', 'F3', 'G3', 'H3', 'I3'],
                        ['A4', 'B4', 'C4', 'D4', 'E4', 'F4', 'G4', 'H4', 'I4'],
                        ['A5', 'B5', 'C5', 'D5', 'E5', 'F5', 'G5', 'H5', 'I5'],
                        ['A6', 'B6', 'C6', 'D6', 'E6', 'F6', 'G6', 'H6', 'I6'],
                        ['A7', 'B7', 'C7', 'D7', 'E7', 'F7', 'G7', 'H7', 'I7'],
                        ['A8', 'B8', 'C8', 'D8', 'E8', 'F8', 'G8', 'H8', 'I8'],
                        ['A9', 'B9', 'C9', 'D9', 'E9', 'F9', 'G9', 'H9', 'I9']])
# 5, 6, 7, 8 -> GREY
# 2 -> BLUE
# 3 -> ORANGE
# 4 -> CROWN
color = np.array([[0, 2, 2, 5, 5, 5, 2, 2, 0],
                  [2, 0, 0, 0, 5, 0, 0, 0, 2],
                  [2, 0, 0, 0, 0, 0, 0, 0, 2],
                  [6, 0, 0, 0, 4, 0, 0, 0, 8],
                  [6, 6, 0, 4, 3, 4, 0, 8, 8],
                  [6, 0, 0, 0, 4, 0, 0, 0, 8],
                  [2, 0, 0, 0, 0, 0, 0, 0, 2],
                  [2, 0, 0, 0, 7, 0, 0, 0, 2],
                  [0, 2, 2, 7, 7, 7, 2, 2, 0]], dtype=np.dtype('uint8'))


class NodeNotEvaluatedError(Exception):
    pass


class Node:
    def __init__(self, state: np.array, white: bool, depth: int, parent=None, start=None, end=None):
        self.state = state
        self.child = []
        self.value = None
        self.value_depth = None
        self.pruned = False
        self.parent = parent
        self.start = start
        self.end = end
        if depth != 0:
            for e in find_all_possible_states_2(state, white):
                self.child.append(Node(e[0], not white, depth - 1, self, e[1], e[2]))

    def __eq__(self, other):
        if self.value is None or other.value is None:
            raise NodeNotEvaluatedError
        return self.value == other.value and self.pruned == other.pruned

    def __gt__(self, other):
        if self.value is None or other.value is None:
            raise NodeNotEvaluatedError
        if self.value == other.value:
            return other.pruned
        return self.value > other.value

    def __lt__(self, other):
        if self.value is None or other.value is None:
            raise NodeNotEvaluatedError
        if self.value == other.value and self.pruned != other.pruned:
            return self.pruned
        return self.value < other.value

    def __str__(self):
        return "Valore: " + str(self.value) + " (" + str(self.start) + " to " + str(self.end) + ") " + " (depth = " +\
               str(self.value_depth) + ")"

    def __repr__(self):
        return "Valore: " + str(self.value) + " (" + str(self.start) + " to " + str(self.end) + ") " + " (depth = " + \
                str(self.value_depth) + ")"


more_pessimistic_node_for_white = Node(np.zeros((9, 9)), True, 0)
more_pessimistic_node_for_white.value = -5000
more_pessimistic_node_for_black = Node(np.zeros((9, 9)), True, 0)
more_pessimistic_node_for_black.value = 5000


def min_max_player(state: np.array, pl: Union[player.WPlayer, player.BPlayer]):
    # Implementare di seguito la vera funzione min_max che ritorni next_state invece di calcolarlo casualmente
    total_num_of_pawns = np.sum(state == 1) + np.sum(state == 2)
    global MAX_DEPH
    if isinstance(pl, player.WPlayer):
        # next_states = find_all_possible_states_2(state, True)
        # next_state = random_evaluation(next_states, "W")
        if total_num_of_pawns < 10:
            MAX_DEPH = 4
        N = Node(state, True, MAX_DEPH)
        min_max(N, MAX_DEPH, -10000, 10000, True)
        possible_move = []
        for n in N.child:
            possible_move.append(n)
        if len(possible_move) != 1:
            possible_move.sort(reverse=True)
            i = 0
            for i, n in enumerate(possible_move):
                if color[state == 3] == 2:
                    return get_move_from_matrix_2(n.state, n.start, n.end)
                if n.value != possible_move[0].value or n.pruned is True:
                    break
            bests_move = possible_move[:i]
            next_state = bests_move[random.randrange(0, len(bests_move))]
        else:
            next_state = possible_move[0]
        print("Value:", next_state.value)
        print("State:", next_state.state)
    else:
        next_states = find_all_possible_states_2(state, False)
        next_state = random_evaluation(next_states)
    # --------------------------------------------------------------------------------------------------------
    return get_move_from_matrix_2(next_state.state, next_state.start, next_state.end)


def get_value_depth(move: Node):
    return move.value_depth


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


@jit(nopython=True)
def find_all_possible_states_2(state: np.array, white: bool) -> list:
    """
    Find all possible move from a state
    :param white: color that have to move (True -> White) (False -> Black)
    :param state: actual state
    :return: a list of all the possibile states
    """
    L = []
    if white:
        if np.sum(state == 3) == 1:
            ai = np.where(state == 3)[0][0]
            aj = np.where(state == 3)[1][0]
            L.extend(get_moves_for_peaces(state, ai, aj))
        ai, aj = np.where(state == 1)
        for k in range(len(ai)):
            L.extend(get_moves_for_peaces(state, ai[k], aj[k]))
    else:
        ai, aj = np.where(state == 2)
        for k in range(len(ai)):
            L.extend(get_moves_for_peaces(state, ai[k], aj[k]))
    return L


@jit(nopython=True)
def get_moves_for_peaces(state: np.array, i: int, j: int) -> list:
    """

    :param state:
    :param i: raw
    :param j: column
    :return:
    """
    L = []
    for ii in range(i - 1, -1, -1):
        if check_move(state, (i, j), (ii, j)):
            new_state = get_new_state(state, (i, j), (ii, j))
            L.append((new_state, (i, j), (ii, j)))
        else:
            break
    for ii in range(i + 1, 9, 1):
        if check_move(state, (i, j), (ii, j)):
            new_state = get_new_state(state, (i, j), (ii, j))
            L.append((new_state, (i, j), (ii, j)))
        else:
            break
    for jj in range(j - 1, -1, -1):
        if check_move(state, (i, j), (i, jj)):
            new_state = get_new_state(state, (i, j), (i, jj))
            L.append((new_state, (i, j), (i, jj)))
        else:
            break
    for jj in range(j + 1, 9, 1):
        if check_move(state, (i, j), (i, jj)):
            new_state = get_new_state(state, (i, j), (i, jj))
            L.append((new_state, (i, j), (i, jj)))
        else:
            break
    return L


@jit(nopython=True)
def check_move(state: np.array, start: tuple, end: tuple):
    white = state[start] == 1
    if state[end] != 0:
        return False
    # Impediamo a chiunque di andare sulle arancioni
    if color[end] == 3:
        return False
    if color[end] in (5, 6, 7, 8):
        if white:
            return False
        if color[start] == color[end]:
            return True
        else:
            return False
    return True


@jit(nopython=True)
def get_new_state(state: np.array, start: tuple, end: tuple):
    new_state = state.copy()
    i, j = start
    ii, jj = end
    new_state[ii, jj] = new_state[i, j]
    new_state[i, j] = 0
    to_check = get_around(end)
    for e in to_check:
        around = get_around(e)
        opposites_1 = [around[0], around[2]]
        opposites_2 = [around[1], around[3]]
        if new_state[e] == 3 and new_state[ii, jj] == 2:
            if color[e] in [3, 4]:
                if check_if_killer(new_state, around[0], e) and check_if_killer(new_state, around[2], e) and \
                        check_if_killer(new_state, around[1], e) and check_if_killer(new_state, around[3], e):
                    new_state[e] = 0
            elif end in opposites_1 and check_if_killer(new_state, around[0], e) and check_if_killer(new_state,
                                                                                                   around[2], e):
                new_state[e] = 0
            elif end in opposites_2 and check_if_killer(new_state, around[1], e) and check_if_killer(new_state,
                                                                                                     around[3], e):
                new_state[e] = 0
        else:
            if end in opposites_1 and check_if_killer(new_state, around[0], e) and check_if_killer(new_state,
                                                                                                   around[2], e):
                new_state[e] = 0
            elif end in opposites_2 and check_if_killer(new_state, around[1], e) and check_if_killer(new_state,
                                                                                                     around[3], e):
                new_state[e] = 0
    return new_state


@jit(nopython=True)
def get_around(pos: tuple):
    to_check = []
    ii, jj = pos
    if ii != 0:
        to_check.append((ii - 1, jj))
    else:
        to_check.append((-1, -1))
    if jj != 8:
        to_check.append((ii, jj + 1))
    else:
        to_check.append((-1, -1))
    if ii != 8:
        to_check.append((ii + 1, jj))
    else:
        to_check.append((-1, -1))
    if jj != 0:
        to_check.append((ii, jj - 1))
    else:
        to_check.append((-1, -1))
    return to_check


@jit(nopython=True)
def check_if_killer(state: np.array, pos: tuple, vittima: tuple):
    if pos == (-1, -1):
        return False
    if state[vittima] == 0:
        return False
    if color[pos] in [5, 6, 7, 8, 3] and color[vittima] not in [5, 6, 7, 8, 3]:
        return True
    if state[pos] == 0:
        return False
    if state[vittima] == 2:
        if state[pos] in [1, 3]:
            return True
    else:
        if state[pos] == 2:
            return True
    return False


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


def get_move_from_matrix_2(status: np.array, start: tuple, end: tuple):
    str_start = coordinates[start]
    str_end = coordinates[end]
    print(str_start, "to", str_end)
    if status[end] == 2:
        # Black is moving
        return str_start, None, str_end
    elif status[end] == 1:
        # White soldier is moving
        return "S", str_start, str_end
    else:
        # White king is moving
        return "K", None, str_end


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


def get_if_state_is_a_finish_game_state(state: np.array) -> bool:
    if np.sum(state == 3) == 0:
        return True
    if color[np.where(state == 3)] == 2:
        return True
    return False


def state_evaluation(state: np.array) -> int:
    tot = 0
    tot += np.sum(np.where(state == 1, 1, 0))*15 + (1 - np.sum(state == 3)) * (-1000)
    tot += np.sum(np.where(state == 2, -1, 0))*10
    if color[np.where(state == 3)] == 2:
        tot += 1000
    return tot


def random_evaluation(states_list: np.array) -> np.array:
    selected_state = states_list[random.randrange(0, len(states_list))]
    return selected_state


def min_max(node: Node, depth: int, alpha: int, beta: int, maximize: bool):
    """
    Core function of min max algorithm
    :param node: Start state
    :param depth: How many move to calculate
    :param alpha:
    :param beta:
    :param maximize: True -> maximize (White best), False -> minimize (Black best)
    :return: evaluation of the position
    """
    if depth == 0 or get_if_state_is_a_finish_game_state(node.state):
        node.value = state_evaluation(node.state)
        node.value_depth = depth
        return node

    if maximize:
        maxEval = more_pessimistic_node_for_white
        for e in node.child:
            evaluation = min_max(e, depth - 1, alpha, beta, False)
            if not evaluation.pruned:
                maxEval = max(maxEval, evaluation)
                alpha = max(alpha, evaluation.value)
                if beta <= alpha:
                    maxEval.pruned = True
                    break
        maxEval.parent.pruned = maxEval.pruned
        maxEval.parent.value = maxEval.value
        maxEval.parent.value_depth = maxEval.value_depth
        return maxEval.parent
    else:
        minEval = more_pessimistic_node_for_black
        for e in node.child:
            evaluation = min_max(e, depth - 1, alpha, beta, True)
            if not evaluation.pruned:
                minEval = min(minEval, evaluation)
                beta = min(beta, evaluation.value)
                if beta <= alpha:
                    minEval.pruned = True
                    break
        minEval.parent.pruned = minEval.pruned
        minEval.parent.value = minEval.value
        minEval.parent.value_depth = minEval.value_depth
        return minEval.parent


# initial_state = np.array([[0, 0, 0, 2, 2, 2, 0, 0, 0],
#                           [0, 0, 0, 0, 2, 0, 0, 0, 0],
#                           [0, 0, 0, 0, 1, 0, 0, 0, 0],
#                           [2, 0, 0, 0, 1, 0, 0, 0, 2],
#                           [2, 2, 1, 1, 3, 1, 1, 2, 2],
#                           [2, 0, 0, 0, 1, 0, 0, 0, 2],
#                           [0, 0, 0, 0, 1, 0, 0, 0, 0],
#                           [0, 0, 0, 0, 2, 0, 0, 0, 0],
#                           [0, 0, 0, 2, 2, 2, 0, 0, 0]])
# # particular_state = np.array([[0, 0, 0, 2, 2, 2, 0, 0, 0],
# #                              [0, 0, 0, 0, 2, 0, 0, 0, 0],
# #                              [0, 0, 0, 0, 1, 0, 0, 0, 0],
# #                              [2, 0, 0, 0, 0, 0, 0, 3, 2],
# #                              [2, 2, 1, 1, 0, 1, 1, 2, 2],
# #                              [2, 0, 0, 0, 1, 0, 0, 0, 2],
# #                              [0, 0, 0, 0, 1, 0, 0, 0, 0],
# #                              [0, 0, 0, 0, 2, 0, 0, 0, 0],
# #                              [0, 0, 0, 2, 2, 2, 0, 0, 0]])
# # particular_state = np.array([[0, 0, 0, 2, 2, 2, 0, 0, 0],
# #                              [0, 0, 0, 0, 0, 0, 0, 0, 0],
# #                              [0, 1, 0, 0, 0, 0, 0, 0, 0],
# #                              [2, 0, 0, 0, 3, 0, 0, 0, 2],
# #                              [2, 2, 1, 1, 0, 1, 1, 2, 2],
# #                              [2, 0, 0, 0, 1, 0, 0, 0, 2],
# #                              [0, 0, 0, 0, 1, 0, 0, 0, 0],
# #                              [0, 0, 0, 0, 2, 0, 0, 0, 0],
# #                              [0, 0, 0, 2, 2, 2, 0, 0, 0]])
# A = np.array([[1, 1, 1, 1, 1, 1, 1, 0, 0],
#               [2, 2, 2, 2, 2, 2, 2, 0, 0],
#               [0, 0, 0, 0, 0, 0, 0, 0, 0],
#               [0, 0, 0, 0, 0, 0, 0, 0, 0],
#               [0, 0, 0, 0, 0, 0, 0, 0, 0],
#               [0, 0, 0, 0, 0, 0, 0, 0, 0],
#               [0, 0, 0, 0, 0, 0, 0, 0, 0],
#               [0, 0, 0, 0, 0, 0, 0, 0, 0],
#               [0, 0, 0, 0, 0, 0, 0, 0, 0]])
# B = np.array([[1, 1, 1, 1, 1, 1, 0, 0, 0],
#               [2, 2, 2, 2, 2, 2, 0, 0, 0],
#               [0, 0, 0, 0, 0, 0, 0, 0, 0],
#               [0, 0, 0, 0, 0, 0, 0, 0, 0],
#               [0, 0, 0, 0, 0, 0, 0, 0, 0],
#               [0, 0, 0, 0, 0, 0, 0, 0, 0],
#               [0, 0, 0, 0, 0, 0, 0, 0, 0],
#               [0, 0, 0, 0, 0, 0, 0, 0, 0],
#               [0, 0, 0, 0, 0, 0, 0, 0, 0]])
# C = np.array([[1, 0, 0, 0, 0, 0, 0, 0, 0],
#               [2, 2, 2, 2, 2, 2, 0, 0, 0],
#               [0, 0, 0, 0, 0, 0, 0, 0, 0],
#               [0, 0, 0, 0, 0, 0, 0, 0, 0],
#               [0, 0, 0, 0, 0, 0, 0, 0, 0],
#               [0, 0, 0, 0, 0, 0, 0, 0, 0],
#               [0, 0, 0, 0, 0, 0, 0, 0, 0],
#               [0, 0, 0, 0, 0, 0, 0, 0, 0],
#               [0, 0, 0, 0, 0, 0, 0, 0, 0]])
# D = np.array([[1, 1, 1, 0, 0, 0, 0, 0, 0],
#               [2, 2, 2, 0, 0, 0, 0, 0, 0],
#               [0, 0, 0, 0, 0, 0, 0, 0, 0],
#               [0, 0, 0, 0, 0, 0, 0, 0, 0],
#               [0, 0, 0, 0, 0, 0, 0, 0, 0],
#               [0, 0, 0, 0, 0, 0, 0, 0, 0],
#               [0, 0, 0, 0, 0, 0, 0, 0, 0],
#               [0, 0, 0, 0, 0, 0, 0, 0, 0],
#               [0, 0, 0, 0, 0, 0, 0, 0, 0]])
# E = np.array([[0, 0, 0, 0, 0, 0, 0, 0, 0],
#               [2, 2, 2, 2, 2, 2, 0, 0, 0],
#               [0, 0, 0, 0, 0, 0, 0, 0, 0],
#               [0, 0, 0, 0, 0, 0, 0, 0, 0],
#               [0, 0, 0, 0, 0, 0, 0, 0, 0],
#               [0, 0, 0, 0, 0, 0, 0, 0, 0],
#               [0, 0, 0, 0, 0, 0, 0, 0, 0],
#               [0, 0, 0, 0, 0, 0, 0, 0, 0],
#               [0, 0, 0, 0, 0, 0, 0, 0, 0]])
# F = np.array([[1, 1, 1, 1, 1, 0, 0, 0, 0],
#               [0, 0, 0, 0, 0, 0, 0, 0, 0],
#               [0, 0, 0, 0, 0, 0, 0, 0, 0],
#               [0, 0, 0, 0, 0, 0, 0, 0, 0],
#               [0, 0, 0, 0, 0, 0, 0, 0, 0],
#               [0, 0, 0, 0, 0, 0, 0, 0, 0],
#               [0, 0, 0, 0, 0, 0, 0, 0, 0],
#               [0, 0, 0, 0, 0, 0, 0, 0, 0],
#               [0, 0, 0, 0, 0, 0, 0, 0, 0]])
# G = np.array([[1, 1, 1, 1, 1, 0, 0, 0, 0],
#               [2, 0, 0, 0, 0, 0, 0, 0, 0],
#               [0, 0, 0, 0, 0, 0, 0, 0, 0],
#               [0, 0, 0, 0, 0, 0, 0, 0, 0],
#               [0, 0, 0, 0, 0, 0, 0, 0, 0],
#               [0, 0, 0, 0, 0, 0, 0, 0, 0],
#               [0, 0, 0, 0, 0, 0, 0, 0, 0],
#               [0, 0, 0, 0, 0, 0, 0, 0, 0],
#               [0, 0, 0, 0, 0, 0, 0, 0, 0]])
# N = Node(A, True, MAX_DEPH)
# N.child[0].state = B
# N.child[1].state = C
# N.child[0].child[0].state = D
# N.child[0].child[1].state = E
# N.child[1].child[0].state = F
# N.child[1].child[1].state = G
# next_state = min_max(N, MAX_DEPH, -1000, 1000, True)
# print(next_state)
# print(next_state.value)
# print(next_state.state)
# print("BHO!")
# state_evaluation(initial_state)
# print(len(find_all_possible_states_2(initial_state, True)))
# get_moves_for_peaces(initial_state, 0, 5)

# print(get_new_state(particular_state, (4, 2), (3, 2)))
# get_new_state(particular_state, (0, 4), (2, 4))
