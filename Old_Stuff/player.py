from visualizer import COLOR_MAP as CM
from visualizer import COORD_L as LETTERS
from visualizer import COORD_N as NUMBERS
from visualizer import GREY
from visualizer import BLUE
from visualizer import ORANGE


class Player:
    soldiers = []
    color = ""

    @staticmethod
    def get_up(pos):
        if pos[1] != "1":
            return pos[0] + str(int(pos[1]) - 1)
        else:
            return None

    @staticmethod
    def get_down(pos):
        if pos[1] != "9":
            return pos[0] + str(int(pos[1]) + 1)
        else:
            return None

    @staticmethod
    def get_right(pos):
        if pos[0] != "I":
            return LETTERS[LETTERS.find(pos[0]) + 1] + pos[1]
        else:
            return None

    @staticmethod
    def get_left(pos):
        if pos[0] != "A":
            return LETTERS[LETTERS.find(pos[0]) - 1] + pos[1]
        else:
            return None

    @staticmethod
    def get_around(pos):
        return [Player.get_up(pos), Player.get_right(pos), Player.get_down(pos), Player.get_left(pos)]

    @staticmethod
    def pos_in_numbers_from_string(pos: str):
        return LETTERS.index(pos[0]), NUMBERS.index(pos[1])


class WPlayer(Player):

    def __init__(self):
        self.color = "W"
        self.soldiers = ["C5", "D5", "E3", "E4", "F5", "G5", "E6", "E7"]
        self.king = "E5"
        self.moves = []
        self.king_moves = []

    def find_possible_moves(self, enemy):
        self.moves.clear()
        self.king_moves.clear()
        for s in self.soldiers:
            moves = []
            actual = s
            while True:
                upper = self.get_up(actual)
                if upper in GREY + ORANGE + self.soldiers + enemy + [self.king] or upper is None:
                    break
                moves.append(upper)
                actual = upper
            actual = s
            while True:
                righter = self.get_right(actual)
                if righter in GREY + ORANGE + self.soldiers + enemy + [self.king] or righter is None:
                    break
                moves.append(righter)
                actual = righter
            actual = s
            while True:
                downer = self.get_down(actual)
                if downer in GREY + ORANGE + self.soldiers + enemy + [self.king] or downer is None:
                    break
                moves.append(downer)
                actual = downer
            actual = s
            while True:
                lefter = self.get_left(actual)
                if lefter in GREY + ORANGE + self.soldiers + enemy + [self.king] or lefter is None:
                    break
                moves.append(lefter)
                actual = lefter
            self.moves.append(moves)
        actual = self.king
        while True:
            upper = self.get_up(actual)
            if upper in GREY + ORANGE + self.soldiers + enemy or upper is None:
                break
            self.king_moves.append(upper)
            actual = upper
        actual = self.king
        while True:
            righter = self.get_right(actual)
            if righter in GREY + ORANGE + self.soldiers + enemy or righter is None:
                break
            self.king_moves.append(righter)
            actual = righter
        actual = self.king
        while True:
            downer = self.get_down(actual)
            if downer in GREY + ORANGE + self.soldiers + enemy or downer is None:
                break
            self.king_moves.append(downer)
            actual = downer
        actual = self.king
        while True:
            lefter = self.get_left(actual)
            if lefter in GREY + ORANGE + self.soldiers + enemy or lefter is None:
                break
            self.king_moves.append(lefter)
            actual = lefter


class BPlayer(Player):

    def __init__(self):
        self.color = "B"
        self.soldiers = ["A4", "A5", "A6", "B5", "D1", "E1", "F1", "E2", "H5", "I4", "I5", "I6", "E8", "D9", "E9", "F9"]
        self.moves = []

    def find_possible_moves(self, enemy):
        self.moves.clear()
        for s in self.soldiers:
            moves = []
            actual = s
            while True:
                upper = self.get_up(actual)
                if upper in ORANGE + self.soldiers + enemy or upper is None:
                    break
                if upper in GREY and s not in GREY:
                    break
                moves.append(upper)
                actual = upper
            actual = s
            while True:
                righter = self.get_right(actual)
                if righter in ORANGE + self.soldiers + enemy or righter is None:
                    break
                if righter in GREY and s not in GREY:
                    break
                moves.append(righter)
                actual = righter
            actual = s
            while True:
                downer = self.get_down(actual)
                if downer in ORANGE + self.soldiers + enemy or downer is None:
                    break
                if downer in GREY and s not in GREY:
                    break
                moves.append(downer)
                actual = downer
            actual = s
            while True:
                lefter = self.get_left(actual)
                if lefter in ORANGE + self.soldiers + enemy or lefter is None:
                    break
                if lefter in GREY and s not in GREY:
                    break
                moves.append(lefter)
                actual = lefter
            self.moves.append(moves)
