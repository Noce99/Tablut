import player


# state -> matrice 9x9 con lo stato della partita
# player -> instance of Player (self.B or self.W where self is Game)

# return move
#   black move -> random_soldier, None, random_move
#   king move  -> return "K", None, random_move
#   white move -> return "S", random_soldier, random_move
def min_max_player(state, pl: player.Player):
    print("Sono il min_max e ti ucciderò!")
    find_all_possible_moves(state, pl)


def find_all_possible_moves(state, pl: player.WPlayer):
    next_status = []
    for i, e in enumerate(pl.soldiers):
        moves = pl.moves[i]
