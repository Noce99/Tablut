import tkinter as tk
from math import sqrt
import sys

master: tk.Tk = None
canvas: tk.Canvas = None
zoom = 1
zoom_rate = 1.3
X = 0
Y = 0
things = []
tmp_things = []
super_tmp_things = []
last_mouse_x = 0
last_mouse_y = 0


class Grid:
    def __init__(self, x, y, cell_size, cell_num, color, line_width=1):
        self.x = x
        self.y = y
        self.cell_size = cell_size
        self.cell_num = cell_num
        self.color = color
        self.line_width = line_width

    def print(self):
        print_grid(change_x(self.x), change_y(self.y), self.cell_size * zoom, self.cell_num, self.color,
                   self.line_width)


class Circle:
    def __init__(self, x, y, radius, color):
        self.x = x
        self.y = y
        self.radius = radius
        self.color = color

    def print(self):
        canvas.create_oval(change_x(self.x - self.radius), change_y(self.y - self.radius),
                           change_x(self.x + self.radius), change_y(self.y + self.radius), fill=self.color)


class Line:
    def __init__(self, x0, y0, x1, y1, color):
        self.x0 = x0
        self.y0 = y0
        self.x1 = x1
        self.y1 = y1
        self.color = color

    def print(self):
        canvas.create_line(change_x(self.x0), change_y(self.y0), change_x(self.x1), change_y(self.y1), fill=self.color,
                           activewidth=10, width=4)


class Square:
    def __init__(self, x0, y0, x1, y1, color):
        self.x0 = x0
        self.y0 = y0
        self.x1 = x1
        self.y1 = y1
        self.color = color

    def print(self):
        canvas.create_rectangle(change_x(self.x0), change_y(self.y0), change_x(self.x1), change_y(self.y1),
                                fill=self.color, width=0)


class SquareNotFill:
    def __init__(self, x0, y0, x1, y1, color):
        self.x0 = x0
        self.y0 = y0
        self.x1 = x1
        self.y1 = y1
        self.color = color

    def print(self):
        canvas.create_rectangle(change_x(self.x0), change_y(self.y0), change_x(self.x1), change_y(self.y1), width=6)


def get_color(r, g, b):
    return "#%02x%02x%02x" % (int(r), int(g), int(b))


def dot(x, y, color):
    canvas.create_oval(x, y, x, y, fill=color)


def print_grid(x, y, cell_size, cell_num, color, line_width):
    global_size = cell_size * cell_num
    canvas.create_line(x, y, x + global_size, y, x + global_size, y + global_size, x, y + global_size, x, y, fill=color, width=line_width)
    for i in range(1, cell_num):
        xx = cell_size * i
        canvas.create_line(x + xx, y, x + xx, y + global_size, fill=color, width=line_width)
    for i in range(1, cell_num):
        yy = cell_size * i
        canvas.create_line(x, y + yy, x + global_size, y + yy, fill=color, width=line_width)


def mouse_dragged(event):
    # global last_mouse_x
    # global last_mouse_y
    # global X
    # global Y
    # difference_x = last_mouse_x - event.x
    # difference_y = last_mouse_y - event.y
    # if sqrt(difference_x * difference_x + difference_y * difference_y) < 20:
    #     X = X - difference_x / zoom
    #     Y = Y - difference_y / zoom
    # last_mouse_x = event.x
    # last_mouse_y = event.y
    # print_all()
    pass


def mouse_wheel_up(_):
    # global zoom
    # if zoom < 100:
    #     zoom = zoom * zoom_rate
    # print_all()
    pass


def mouse_wheel_down(_):
    # global zoom
    # zoom = zoom / zoom_rate
    # print_all()
    pass


def add(item):
    things.append(item)


def add_tmp(item):
    tmp_things.append(item)


def add_super_tmp(item):
    super_tmp_things.append(item)


def print_all():
    global canvas
    canvas.delete('all')
    for item in things:
        item.print()
    for item in tmp_things:
        item.print()
    for item in super_tmp_things:
        item.print()
    canvas.update()


def clear_things():
    global things
    things.clear()


def clear_tmp_things():
    global tmp_things
    tmp_things.clear()


def clear_super_tmp_things():
    global super_tmp_things
    super_tmp_things.clear()


def change_x(x):
    return (X + x) * zoom


def change_y(y):
    return (Y + y) * zoom


def add_loop_bind(loop_function):
    master.bind('<Key>', lambda i: loop_function(i))


def initialize_graphics():
    global master
    global canvas
    master = tk.Tk()
    master.title("Tablut")
    # must be flat, groove, raised, ridge, solid, or sunken
    canvas = tk.Canvas(master, width=900, height=900, bg="white", bd=0)
    canvas.pack()
    master.bind("<B1-Motion>", mouse_dragged)
    master.bind("<Button-4>", mouse_wheel_up)
    master.bind("<Button-5>", mouse_wheel_down)
    master.protocol("WM_DELETE_WINDOW", destroy_everything)


def enter_loop():
    tk.mainloop()


def func_biding(event, func):
    global canvas
    canvas.bind(event, func)


def destroy_everything():
    print("Quitting!")
    master.destroy()
    sys.exit(0)

# ALPHA
# from tkinter import *
# from PIL import Image, ImageTk
#
# root = Tk()
#
# images = []  # to hold the newly created image
#
# def create_rectangle(x1, y1, x2, y2, **kwargs):
#     if 'alpha' in kwargs:
#         alpha = int(kwargs.pop('alpha') * 255)
#         fill = kwargs.pop('fill')
#         fill = root.winfo_rgb(fill) + (alpha,)
#         image = Image.new('RGBA', (x2-x1, y2-y1), fill)
#         images.append(ImageTk.PhotoImage(image))
#         canvas.create_image(x1, y1, image=images[-1], anchor='nw')
#     canvas.create_rectangle(x1, y1, x2, y2, **kwargs)
#
# canvas = Canvas(width=300, height=200)
# canvas.pack()
#
# create_rectangle(10, 10, 200, 100, fill='blue')
# create_rectangle(50, 50, 250, 150, fill='green', alpha=.5)
# create_rectangle(80, 80, 150, 120, fill='#800000', alpha=.8)
#
# root.mainloop()
