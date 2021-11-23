#ifndef MIN_MAX_H
#define MIN_MAX_H

#include <tuple>
#include <vector>
#include <string>

std::string min_max_player_wrapper(std::vector<std::vector<char>>, bool, int);
void print_state(std::vector<std::vector<char>>);
bool get_if_state_is_a_finish_game_state(std::vector<std::vector<char>>);
int state_evaluation(std::vector<std::vector<char>>);
std::vector<int> get_king_position_on_board(std::vector<std::vector<char>>);
std::tuple<std::tuple<std::vector<std::vector<char>> , std::vector<int>, std::vector<int>>, int> min_max(std::tuple<std::vector<std::vector<char>>, std::vector<int>, std::vector<int>>, int, int, int, int, bool);
std::string get_move_from_matrix(std::tuple<std::vector<std::vector<char>> , std::vector<int>, std::vector<int>>);
std::string min_max_player(std::vector<std::vector<char>>, bool, int);
int heuristic_wrapper(std::vector<std::vector<char>>, bool);
int black_heuristic(std::vector<std::vector<char>>);
int white_heuristic(std::vector<std::vector<char>>);

#endif