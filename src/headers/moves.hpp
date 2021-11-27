#ifndef MOVES_H
#define MOVES_H

#include <vector>
#include <tuple>

class Moves {
    
    private:
        std::vector<char> ai;
        std::vector<char> aj;
        std::vector<int> actual_peace = {0, 0};
        std::vector<std::vector<char>> actual_board;
        char actual_direction;
        bool done_actual_peace;
        int ii;
        int jj;
        
    public:
        Moves();
        Moves(std::vector<std::vector<char>>, bool);
        std::tuple<std::vector<std::vector<char>> , std::vector<int>, std::vector<int>> get_next_move();
        std::tuple<std::vector<std::vector<char>> , std::vector<int>, std::vector<int>> get_next_move_for_peace();
        bool check_move(std::vector<std::vector<char>>, std::vector<int>, std::vector<int>);
        std::tuple<std::vector<std::vector<char>> , std::vector<int>, std::vector<int>> get_new_state(std::vector<std::vector<char>>, std::vector<int>, std::vector<int>);
        bool check_if_deadth(std::vector<std::vector<char>>, int, int, int, int, int, int, int);
        std::vector<std::vector<int>> get_around(std::vector<int>);
        bool check_if_killer(std::vector<std::vector<char>>, std::vector<int>, std::vector<int>);
        std::vector<int> get_king_position_on_board(std::vector<std::vector<char>>);
};

#endif