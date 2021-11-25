import player
from typing import Union
import numpy as np
# import copy
from time import time

'''
Modulo che gioca in modo autonomo prendendo decisioni con il famoso min_max.

STATO -> Lo stato è una tupla di 3 elementi

         [1] rappresentazione della scacchiera in quello stato,
         [2] coordinate di inizio dell' ultima mossa,
         [3] coordinate di fine dell' ultima mossa

Sezioni:
    - DEFINIZIONI UTILI
    - MAIN
    - RICERCA MOSSE
    - VALUTAZIONE
'''
'''
██╗███╗   ██╗██╗███████╗██╗ ██████╗
██║████╗  ██║██║╚══███╔╝██║██╔═══██╗
██║██╔██╗ ██║██║  ███╔╝ ██║██║   ██║
██║██║╚██╗██║██║ ███╔╝  ██║██║   ██║
██║██║ ╚████║██║███████╗██║╚██████╔╝
╚═╝╚═╝  ╚═══╝╚═╝╚══════╝╚═╝ ╚═════╝

██████╗ ███████╗███████╗██╗███╗   ██╗██╗███████╗██╗ ██████╗ ███╗   ██╗██╗
██╔══██╗██╔════╝██╔════╝██║████╗  ██║██║╚══███╔╝██║██╔═══██╗████╗  ██║██║
██║  ██║█████╗  █████╗  ██║██╔██╗ ██║██║  ███╔╝ ██║██║   ██║██╔██╗ ██║██║
██║  ██║██╔══╝  ██╔══╝  ██║██║╚██╗██║██║ ███╔╝  ██║██║   ██║██║╚██╗██║██║
██████╔╝███████╗██║     ██║██║ ╚████║██║███████╗██║╚██████╔╝██║ ╚████║██║
╚═════╝ ╚══════╝╚═╝     ╚═╝╚═╝  ╚═══╝╚═╝╚══════╝╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚═╝
'''

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

'''
███████╗██╗███╗   ██╗███████╗
██╔════╝██║████╗  ██║██╔════╝
█████╗  ██║██╔██╗ ██║█████╗
██╔══╝  ██║██║╚██╗██║██╔══╝
██║     ██║██║ ╚████║███████╗
╚═╝     ╚═╝╚═╝  ╚═══╝╚══════╝

██████╗ ███████╗███████╗██╗███╗   ██╗██╗███████╗██╗ ██████╗ ███╗   ██╗██╗
██╔══██╗██╔════╝██╔════╝██║████╗  ██║██║╚══███╔╝██║██╔═══██╗████╗  ██║██║
██║  ██║█████╗  █████╗  ██║██╔██╗ ██║██║  ███╔╝ ██║██║   ██║██╔██╗ ██║██║
██║  ██║██╔══╝  ██╔══╝  ██║██║╚██╗██║██║ ███╔╝  ██║██║   ██║██║╚██╗██║██║
██████╔╝███████╗██║     ██║██║ ╚████║██║███████╗██║╚██████╔╝██║ ╚████║██║
╚═════╝ ╚══════╝╚═╝     ╚═╝╚═╝  ╚═══╝╚═╝╚══════╝╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚═╝
'''
'''
██╗███╗   ██╗██╗███████╗██╗ ██████╗
██║████╗  ██║██║╚══███╔╝██║██╔═══██╗
██║██╔██╗ ██║██║  ███╔╝ ██║██║   ██║
██║██║╚██╗██║██║ ███╔╝  ██║██║   ██║
██║██║ ╚████║██║███████╗██║╚██████╔╝
╚═╝╚═╝  ╚═══╝╚═╝╚══════╝╚═╝ ╚═════╝

███╗   ███╗ █████╗ ██╗███╗   ██╗
████╗ ████║██╔══██╗██║████╗  ██║
██╔████╔██║███████║██║██╔██╗ ██║
██║╚██╔╝██║██╔══██║██║██║╚██╗██║
██║ ╚═╝ ██║██║  ██║██║██║ ╚████║
╚═╝     ╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═══╝
'''


def min_max_player(state: np.array, pl: Union[player.WPlayer, player.BPlayer]):
    MAX_DEPH = 4
    if isinstance(pl, player.WPlayer):
        next_state, value = min_max((state, None, None), MAX_DEPH, MAX_DEPH, -10000, 10000, True)
        print("Best Value:", value)
    else:
        next_state, value = min_max((state, None, None), MAX_DEPH, MAX_DEPH, -10000, 10000, False)
        print("Best Value:", value)
    # --------------------------------------------------------------------------------------------------------
    return get_move_from_matrix(next_state)


def get_move_from_matrix(state):
    board = state[0]
    start = state[1]
    end = state[2]
    str_start = coordinates[start]
    str_end = coordinates[end]
    print(str_start, "to", str_end)
    if board[end] == 2:
        # Black is moving
        return str_start, None, str_end
    elif board[end] == 1:
        # White soldier is moving
        return "S", str_start, str_end
    else:
        # White king is moving
        return "K", None, str_end


'''
███████╗██╗███╗   ██╗███████╗
██╔════╝██║████╗  ██║██╔════╝
█████╗  ██║██╔██╗ ██║█████╗
██╔══╝  ██║██║╚██╗██║██╔══╝
██║     ██║██║ ╚████║███████╗
╚═╝     ╚═╝╚═╝  ╚═══╝╚══════╝

███╗   ███╗ █████╗ ██╗███╗   ██╗
████╗ ████║██╔══██╗██║████╗  ██║
██╔████╔██║███████║██║██╔██╗ ██║
██║╚██╔╝██║██╔══██║██║██║╚██╗██║
██║ ╚═╝ ██║██║  ██║██║██║ ╚████║
╚═╝     ╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═══╝
'''
'''
██╗███╗   ██╗██╗███████╗██╗ ██████╗
██║████╗  ██║██║╚══███╔╝██║██╔═══██╗
██║██╔██╗ ██║██║  ███╔╝ ██║██║   ██║
██║██║╚██╗██║██║ ███╔╝  ██║██║   ██║
██║██║ ╚████║██║███████╗██║╚██████╔╝
╚═╝╚═╝  ╚═══╝╚═╝╚══════╝╚═╝ ╚═════╝

██████╗ ██╗ ██████╗███████╗██████╗  ██████╗ █████╗     ███╗   ███╗ ██████╗ ███████╗███████╗███████╗
██╔══██╗██║██╔════╝██╔════╝██╔══██╗██╔════╝██╔══██╗    ████╗ ████║██╔═══██╗██╔════╝██╔════╝██╔════╝
██████╔╝██║██║     █████╗  ██████╔╝██║     ███████║    ██╔████╔██║██║   ██║███████╗███████╗█████╗
██╔══██╗██║██║     ██╔══╝  ██╔══██╗██║     ██╔══██║    ██║╚██╔╝██║██║   ██║╚════██║╚════██║██╔══╝
██║  ██║██║╚██████╗███████╗██║  ██║╚██████╗██║  ██║    ██║ ╚═╝ ██║╚██████╔╝███████║███████║███████╗
╚═╝  ╚═╝╚═╝ ╚═════╝╚══════╝╚═╝  ╚═╝ ╚═════╝╚═╝  ╚═╝    ╚═╝     ╚═╝ ╚═════╝ ╚══════╝╚══════╝╚══════╝
'''


def find_all_possible_children(board: np.array, white: bool) -> list:
    """
    Find all possible move from a state
    :param board:
    :param white: color that have to move (True -> White) (False -> Black)
    :return: a list of all the possibile states
    """
    if white:
        ai, aj = np.where(board == 1)
        if np.sum(board == 3) == 1:
            ai = np.append(ai, np.where(board == 3)[0][0])
            aj = np.append(aj, np.where(board == 3)[1][0])
        aa = np.stack((ai, aj), axis=0)
        aa = np.rot90(aa)
        np.random.shuffle(aa)
        for k in range(len(ai)):
            for move in get_moves_for_peaces((board, (aa[k, 0], aa[k, 1]), None)):
                yield move
    else:
        ai, aj = np.where(board == 2)
        aa = np.stack((ai, aj), axis=0)
        aa = np.rot90(aa)
        np.random.shuffle(aa)
        for k in range(len(ai)):
            for move in get_moves_for_peaces((board, (aa[k, 0], aa[k, 1]), None)):
                yield move


def get_moves_for_peaces(state: tuple) -> list:
    """
    :param state:
    :return:
    """
    board = state[0]
    i, j = state[1]
    for ii in range(i - 1, -1, -1):
        if check_move(board, (i, j), (ii, j)):
            yield get_new_state(board, (i, j), (ii, j))
        else:
            break
    for ii in range(i + 1, 9, 1):
        if check_move(board, (i, j), (ii, j)):
            yield get_new_state(board, (i, j), (ii, j))
        else:
            break
    for jj in range(j - 1, -1, -1):
        if check_move(board, (i, j), (i, jj)):
            yield get_new_state(board, (i, j), (i, jj))
        else:
            break
    for jj in range(j + 1, 9, 1):
        if check_move(board, (i, j), (i, jj)):
            yield get_new_state(board, (i, j), (i, jj))
        else:
            break


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
    return new_state, start, end


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


'''
███████╗██╗███╗   ██╗███████╗
██╔════╝██║████╗  ██║██╔════╝
█████╗  ██║██╔██╗ ██║█████╗
██╔══╝  ██║██║╚██╗██║██╔══╝
██║     ██║██║ ╚████║███████╗
╚═╝     ╚═╝╚═╝  ╚═══╝╚══════╝

██████╗ ██╗ ██████╗███████╗██████╗  ██████╗ █████╗     ███╗   ███╗ ██████╗ ███████╗███████╗███████╗
██╔══██╗██║██╔════╝██╔════╝██╔══██╗██╔════╝██╔══██╗    ████╗ ████║██╔═══██╗██╔════╝██╔════╝██╔════╝
██████╔╝██║██║     █████╗  ██████╔╝██║     ███████║    ██╔████╔██║██║   ██║███████╗███████╗█████╗
██╔══██╗██║██║     ██╔══╝  ██╔══██╗██║     ██╔══██║    ██║╚██╔╝██║██║   ██║╚════██║╚════██║██╔══╝
██║  ██║██║╚██████╗███████╗██║  ██║╚██████╗██║  ██║    ██║ ╚═╝ ██║╚██████╔╝███████║███████║███████╗
╚═╝  ╚═╝╚═╝ ╚═════╝╚══════╝╚═╝  ╚═╝ ╚═════╝╚═╝  ╚═╝    ╚═╝     ╚═╝ ╚═════╝ ╚══════╝╚══════╝╚══════╝
'''
'''
██╗███╗   ██╗██╗███████╗██╗ ██████╗
██║████╗  ██║██║╚══███╔╝██║██╔═══██╗
██║██╔██╗ ██║██║  ███╔╝ ██║██║   ██║
██║██║╚██╗██║██║ ███╔╝  ██║██║   ██║
██║██║ ╚████║██║███████╗██║╚██████╔╝
╚═╝╚═╝  ╚═══╝╚═╝╚══════╝╚═╝ ╚═════╝

██╗   ██╗ █████╗ ██╗     ██╗   ██╗████████╗ █████╗ ███████╗██╗ ██████╗ ███╗   ██╗███████╗
██║   ██║██╔══██╗██║     ██║   ██║╚══██╔══╝██╔══██╗╚══███╔╝██║██╔═══██╗████╗  ██║██╔════╝
██║   ██║███████║██║     ██║   ██║   ██║   ███████║  ███╔╝ ██║██║   ██║██╔██╗ ██║█████╗
╚██╗ ██╔╝██╔══██║██║     ██║   ██║   ██║   ██╔══██║ ███╔╝  ██║██║   ██║██║╚██╗██║██╔══╝
 ╚████╔╝ ██║  ██║███████╗╚██████╔╝   ██║   ██║  ██║███████╗██║╚██████╔╝██║ ╚████║███████╗
  ╚═══╝  ╚═╝  ╚═╝╚══════╝ ╚═════╝    ╚═╝   ╚═╝  ╚═╝╚══════╝╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
'''


def min_max(state: tuple, depth: int, max_depth: int, alpha: int, beta: int, maximize: bool):
    """
    Core function of min max algorithm
    :param max_depth:
    :param state: Actual state
    :param depth: How many move to calculate
    :param alpha:
    :param beta:
    :param maximize: True -> maximize (White best), False -> minimize (Black best)
    :return: evaluation of the position
    """
    board = state[0]
    if depth == 0 or get_if_state_is_a_finish_game_state(board):
        return state_evaluation(board)

    if maximize:
        maxEval = -10000
        for e in find_all_possible_children(board, True):
            evaluation = min_max(e, depth - 1, max_depth, alpha, beta, False)
            if evaluation > maxEval:
                maxEval = evaluation
                if depth == max_depth:
                    maxChild = e
            alpha = max(alpha, evaluation)
            if beta <= alpha:
                break
        if depth == max_depth:
            return maxChild, maxEval
        return maxEval
    else:
        minEval = 10000
        for e in find_all_possible_children(board, False):
            evaluation = min_max(e, depth - 1, max_depth, alpha, beta, True)
            if evaluation < minEval:
                minEval = evaluation
                if depth == max_depth:
                    minChild = e
            beta = min(beta, evaluation)
            if beta <= alpha:
                break
        if depth == max_depth:
            return minChild, minEval
        return minEval


def get_if_state_is_a_finish_game_state(board: np.array) -> bool:
    if np.sum(board == 3) == 0:
        return True
    if color[np.where(board == 3)] == 2:
        return True
    return False


def state_evaluation(board: np.array) -> int:
    tot = 0
    tot += np.sum(np.where(board == 1, 1, 0)) * 15 + (1 - np.sum(board == 3)) * (-1000)
    tot += np.sum(np.where(board == 2, -1, 0)) * 10
    if color[np.where(board == 3)] == 2:
        tot += 1000
    return tot


'''
███████╗██╗███╗   ██╗███████╗
██╔════╝██║████╗  ██║██╔════╝
█████╗  ██║██╔██╗ ██║█████╗
██╔══╝  ██║██║╚██╗██║██╔══╝
██║     ██║██║ ╚████║███████╗
╚═╝     ╚═╝╚═╝  ╚═══╝╚══════╝

██╗   ██╗ █████╗ ██╗     ██╗   ██╗████████╗ █████╗ ███████╗██╗ ██████╗ ███╗   ██╗███████╗
██║   ██║██╔══██╗██║     ██║   ██║╚══██╔══╝██╔══██╗╚══███╔╝██║██╔═══██╗████╗  ██║██╔════╝
██║   ██║███████║██║     ██║   ██║   ██║   ███████║  ███╔╝ ██║██║   ██║██╔██╗ ██║█████╗
╚██╗ ██╔╝██╔══██║██║     ██║   ██║   ██║   ██╔══██║ ███╔╝  ██║██║   ██║██║╚██╗██║██╔══╝
 ╚████╔╝ ██║  ██║███████╗╚██████╔╝   ██║   ██║  ██║███████╗██║╚██████╔╝██║ ╚████║███████╗
  ╚═══╝  ╚═╝  ╚═╝╚══════╝ ╚═════╝    ╚═╝   ╚═╝  ╚═╝╚══════╝╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
'''

initial_state = np.array([[0, 0, 0, 2, 2, 2, 0, 0, 0],
                          [0, 0, 0, 0, 2, 0, 0, 0, 0],
                          [0, 0, 0, 0, 1, 0, 0, 0, 0],
                          [2, 0, 0, 0, 1, 0, 0, 0, 2],
                          [2, 2, 1, 1, 3, 1, 1, 2, 2],
                          [2, 0, 0, 0, 1, 0, 0, 0, 2],
                          [0, 0, 0, 0, 1, 0, 0, 0, 0],
                          [0, 0, 0, 0, 2, 0, 0, 0, 0],
                          [0, 0, 0, 2, 2, 2, 0, 0, 0]])
# particular_state = np.array([[0, 0, 0, 2, 2, 2, 0, 0, 0],
#                              [0, 0, 0, 0, 2, 0, 0, 0, 0],
#                              [0, 0, 0, 0, 1, 0, 0, 0, 0],
#                              [2, 0, 0, 0, 0, 0, 0, 3, 2],
#                              [2, 2, 1, 1, 0, 1, 1, 2, 2],
#                              [2, 0, 0, 0, 1, 0, 0, 0, 2],
#                              [0, 0, 0, 0, 1, 0, 0, 0, 0],
#                              [0, 0, 0, 0, 2, 0, 0, 0, 0],
#                              [0, 0, 0, 2, 2, 2, 0, 0, 0]])
# particular_state = np.array([[0, 0, 0, 2, 2, 2, 0, 0, 0],
#                              [0, 0, 0, 0, 0, 0, 0, 0, 0],
#                              [0, 1, 0, 0, 0, 0, 0, 0, 0],
#                              [2, 0, 0, 0, 3, 0, 0, 0, 2],
#                              [2, 2, 1, 1, 0, 1, 1, 2, 2],
#                              [2, 0, 0, 0, 1, 0, 0, 0, 2],
#                              [0, 0, 0, 0, 1, 0, 0, 0, 0],
#                              [0, 0, 0, 0, 2, 0, 0, 0, 0],
#                              [0, 0, 0, 2, 2, 2, 0, 0, 0]])
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

# L = []
# start = time()
# for e in find_all_possible_states(initial_state, True):
#     L.append(e)
# end = time()
# print(len(L))
# print("Tempo:", end-start)

# start = time()
# min_max_player(initial_state, player.WPlayer())
# end = time()
# print("Tempo:", end-start)
