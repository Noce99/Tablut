import nut_simple_graphics as nsg

COLOR_MAP = ("011222110",
             "100020001",
             "100000001",
             "200000002",
             "220030022",
             "200000002",
             "100000001",
             "100020001",
             "011222110")
COORD_L = "ABCDEFGHI"
COORD_N = "123456789"
GREY = ["D1", "E1", "F1", "E2", "A4", "A5", "A6", "B5", "D9", "E9", "F9", "E8", "H5", "I4", "I5", "I6"]
BLUE = ["B1", "C1", "G1", "H1", "A2", "I2", "A3", "I3", "A7", "I7", "A8", "I8", "B9", "C9", "G9", "H9"]
ORANGE = ["E5"]
CROWN = ["D4", "E4", "F4", "D5", "F5", "D6", "E6", "F6"]


def initialize(click_event):
    nsg.initialize_graphics()
    for i, e in enumerate(COLOR_MAP):
        for ii, ee in enumerate(e):
            if ee == "0":
                nsg.add(nsg.Square(ii * 100, i * 100, ii * 100 + 100, i * 100 + 100, nsg.get_color(255, 233, 127)))
            elif ee == "1":
                nsg.add(nsg.Square(ii * 100, i * 100, ii * 100 + 100, i * 100 + 100, nsg.get_color(92, 249, 249)))
            elif ee == "2":
                nsg.add(nsg.Square(ii * 100, i * 100, ii * 100 + 100, i * 100 + 100, nsg.get_color(128, 128, 128)))
            elif ee == "3":
                nsg.add(nsg.Square(ii * 100, i * 100, ii * 100 + 100, i * 100 + 100, nsg.get_color(255, 128, 0)))
    nsg.add(nsg.Grid(0, 0, 100, 9, nsg.get_color(0, 0, 0)))
    nsg.print_all()
    nsg.func_biding("<Button-1>", click_event)


def clear():
    nsg.clear_tmp_things()
    positions.clear()
    all_moves.clear()


positions = []
all_moves = []


def print_piece(color, position, moves):
    positions.append(position)
    all_moves.append(moves)
    if color == "W":
        c = nsg.get_color(255, 255, 255)
        s = 45
    elif color == "B":
        c = nsg.get_color(0, 0, 0)
        s = 45
    else:
        c = nsg.get_color(255, 102, 255)
        s = 30
    if len(position) == 2 and position[0] in COORD_L and position[1] in COORD_N:
        nsg.add_tmp(nsg.Circle(COORD_L.find(position[0]) * 100 + 50, COORD_N.find(position[1]) * 100 + 50, s, c))
    nsg.print_all()


def highlight_pos(position):
    if len(position) == 2 and position[0] in COORD_L and position[1] in COORD_N:
        nsg.add_super_tmp(nsg.SquareNotFill(COORD_L.find(position[0]) * 100, COORD_N.find(position[1]) * 100,
                                            COORD_L.find(position[0]) * 100 + 100,
                                            COORD_N.find(position[1]) * 100 + 100, nsg.get_color(128, 255, 0)))
    nsg.print_all()


def print_all():
    nsg.print_all()


def loop():
    nsg.enter_loop()


def highlight_clear():
    nsg.clear_super_tmp_things()


def destroy_everything():
    nsg.destroy_everything()