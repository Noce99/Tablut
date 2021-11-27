import random
'''
def get_move(soldiers, moves):
'''

def get_black_random_move(soldiers, moves):
    while True:
        soldier_index = random.randrange(0, len(soldiers))
        if len(moves[soldier_index]) != 0:
            break
    random_soldier = soldiers[soldier_index]
    move_index = random.randrange(0, len(moves[soldier_index]))
    random_move = moves[soldier_index][move_index]
    return random_soldier, None, random_move


def get_white_random_move(soldiers, s_moves, k_moves):
    if len(k_moves) == 0:
        while True:
            soldier_index = random.randrange(0, len(soldiers))
            if len(s_moves[soldier_index]) != 0:
                break
    else:
        while True:
            soldier_index = random.randrange(0, len(soldiers) + 1)
            if soldier_index == len(soldiers):
                break
            else:
                if len(s_moves[soldier_index]) != 0:
                    break
    if soldier_index == len(soldiers):
        move_index = random.randrange(0, len(k_moves))
        random_move = k_moves[move_index]
        return "K", None, random_move
    else:
        random_soldier = soldiers[soldier_index]
        move_index = random.randrange(0, len(s_moves[soldier_index]))
        random_move = s_moves[soldier_index][move_index]
        return "S", random_soldier, random_move
