import player
import sys
import time
import visualizer as vis
from visualizer import COORD_L
from visualizer import COORD_N
from visualizer import BLUE
from visualizer import GREY
from visualizer import ORANGE
from visualizer import CROWN
import random_player


class Game:

    def __init__(self, visualize=False, white_player="human", black_player="human"):
        self.W = player.WPlayer()
        self.B = player.BPlayer()
        self.visualize = visualize
        if self.visualize:
            vis.initialize(self.click_event)
        self.color_playing = None
        self.highlight = None
        self.white_player = white_player
        self.black_player = black_player

    def print_all(self):
        vis.clear()
        for i, e in enumerate(self.W.soldiers):
            vis.print_piece("W", e, self.W.moves[i])
        vis.print_piece("K", self.W.king, [])
        for i, e in enumerate(self.B.soldiers):
            vis.print_piece("B", e, [])

    def click_event(self, event):
        vis.print_all()
        xx = int(event.x / 100)
        yy = int(event.y / 100)
        e = COORD_L[xx] + COORD_N[yy]
        if e in self.W.soldiers:
            moves = self.W.moves[self.W.soldiers.index(e)]
            vis.highlight_clear()
            for m in moves:
                vis.highlight_pos(m)
            self.highlight = e
        elif e in self.B.soldiers:
            moves = self.B.moves[self.B.soldiers.index(e)]
            vis.highlight_clear()
            for m in moves:
                vis.highlight_pos(m)
            self.highlight = e
        elif e == self.W.king:
            vis.highlight_clear()
            for m in self.W.king_moves:
                vis.highlight_pos(m)
            self.highlight = e
        else:
            if self.color_playing == "W" and self.highlight in self.W.soldiers:
                if e in self.W.moves[self.W.soldiers.index(self.highlight)]:
                    self.W.soldiers[self.W.soldiers.index(self.highlight)] = e
                    self.check_things(e, "W")
                    vis.highlight_clear()
                    self.print_all()
                    self.color_playing = "B"
                    self.W.find_possible_moves(self.B.soldiers)
                    self.B.find_possible_moves(self.W.soldiers+[self.W.king])
                    if self.black_player != "human":
                        self.non_human_move()
            elif self.color_playing == "W" and self.highlight == self.W.king:
                if e in self.W.king_moves:
                    self.W.king = e
                    self.check_things(e, "W")
                    vis.highlight_clear()
                    self.print_all()
                    self.color_playing = "B"
                    self.W.find_possible_moves(self.B.soldiers)
                    self.B.find_possible_moves(self.W.soldiers+[self.W.king])
                    if self.black_player != "human":
                        self.non_human_move()
            elif self.color_playing == "B" and self.highlight in self.B.soldiers:
                if e in self.B.moves[self.B.soldiers.index(self.highlight)]:
                    self.B.soldiers[self.B.soldiers.index(self.highlight)] = e
                    self.check_things(e, "B")
                    vis.highlight_clear()
                    self.print_all()
                    self.color_playing = "W"
                    self.W.find_possible_moves(self.B.soldiers)
                    self.B.find_possible_moves(self.W.soldiers+[self.W.king])
                    if self.white_player != "human":
                        self.non_human_move()

    def start_game(self):
        self.W.find_possible_moves(self.B.soldiers)
        self.B.find_possible_moves(self.W.soldiers+[self.W.king])
        if self.visualize:
            self.print_all()
        self.color_playing = "W"
        if self.white_player == "random":
            self.non_human_move()
        vis.loop()

    def non_human_move(self):
        again_non_human = False
        if self.color_playing == "W":
            if self.white_player == "random":
                move = random_player.get_white_random_move(self.W.soldiers, self.W.moves, self.W.king_moves)
            elif self.white_player == "otherCPU":
                move = None
            else:
                move = random_player.get_white_random_move(self.W.soldiers, self.W.moves, self.W.king_moves)
            if move[0] == "K":
                self.W.king = move[2]
            else:
                index = self.W.soldiers.index(move[1])
                self.W.soldiers[index] = move[2]
            self.check_things(move[2], "W")
            self.color_playing = "B"
            if self.black_player != "human":
                again_non_human = True
        else:
            if self.black_player == "random":
                move = random_player.get_black_random_move(self.B.soldiers, self.B.moves)
            elif self.black_player == "otherCPU":
                move = None
            else:
                move = random_player.get_black_random_move(self.B.soldiers, self.B.moves)
            index = self.B.soldiers.index(move[0])
            self.B.soldiers[index] = move[2]
            self.check_things(move[2], "B")
            self.color_playing = "W"
            if self.white_player != "human":
                again_non_human = True
        if self.visualize:
            vis.highlight_clear()
            self.print_all()
        self.W.find_possible_moves(self.B.soldiers)
        self.B.find_possible_moves(self.W.soldiers + [self.W.king])
        if again_non_human:
            self.non_human_move()

    def check_things(self, last_move, last_move_color):
        if self.W.king in BLUE:
            print("White WON!")
            if self.visualize:
                vis.destroy_everything()
            else:
                sys.exit(0)
        around = self.W.get_around(last_move)
        for pos in around:
            if pos is not None:
                if last_move_color == "W" and pos in self.B.soldiers:
                    if self.check_if_dead(pos, "B"):
                        self.B.soldiers.remove(pos)
                elif last_move_color == "B" and pos in self.W.soldiers:
                    if self.check_if_dead(pos, "W"):
                        self.W.soldiers.remove(pos)
                elif last_move_color == "B" and pos == self.W.king:
                    if self.check_if_king_dead(pos):
                        print("Black WON!")
                        if self.visualize:
                            vis.destroy_everything()
                        else:
                            sys.exit(0)

    def check_if_dead(self, pos, color):
        around = self.W.get_around(pos)
        around_bad = []
        for el in around:
            if el is None:
                around_bad.append(0)
            elif el in GREY+ORANGE:
                if pos not in GREY:
                    around_bad.append(1)
                else:
                    around_bad.append(0)
            elif el in self.W.soldiers + [self.W.king]:
                if color == "W":
                    around_bad.append(0)
                else:
                    around_bad.append(1)
            elif el in self.B.soldiers:
                if color == "W":
                    around_bad.append(1)
                else:
                    around_bad.append(0)
            else:
                around_bad.append(0)
        num_of_bad = around_bad.count(1)
        if num_of_bad >= 3:
            return True
        if num_of_bad <= 1:
            return False
        if around_bad[0]+around_bad[2] == 2 or around_bad[1]+around_bad[3] == 2:
            return True
        else:
            return False

    def check_if_king_dead(self, pos):
        around = self.W.get_around(pos)
        around_bad = []
        for el in around:
            if el is None:
                around_bad.append(0)
            elif el in GREY + ORANGE:
                around_bad.append(1)
            elif el in self.W.soldiers + [self.W.king]:
                around_bad.append(0)
            elif el in self.B.soldiers:
                around_bad.append(1)
            else:
                around_bad.append(0)
        num_of_bad = around_bad.count(1)
        if pos in ["E5"] + CROWN:
            if num_of_bad < 4:
                return False
            else:
                return True
        if num_of_bad >= 3:
            return True
        if num_of_bad <= 1:
            return False
        if around_bad[0] + around_bad[2] == 2 or around_bad[1] + around_bad[3] == 2:
            return True
        else:
            return False


G = Game(visualize=True, white_player="human", black_player="random")
G.start_game()
# G.W.find_possible_moves(G.B.soldiers)
# G.B.find_possible_moves(G.W.soldiers+[G.W.king])
# G.print_all()
# time.sleep(2)
# G.W.soldiers[0] = "C9"
# G.W.find_possible_moves(G.B.soldiers)
# G.B.find_possible_moves(G.W.soldiers+[G.W.king])
# G.print_all()
# vis.loop()
# Modifica!