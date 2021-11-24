// Commento a cazzo!
#include <unistd.h>
#include <iostream>
#include <ostream>
#include <tuple>
#include <vector>
#include <string>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <math.h>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "headers/moves.hpp"
#include "headers/utils.hpp"
#include "headers/min_max.hpp"

using namespace std;

#define H_WHITE 15
#define H_BLACK 5
#define H_W_EATEN 15
#define H_B_EATEN 5
#define H_B_NEAR_KING 7
#define B_PAWNS 16
#define W_PAWNS 8

string coordinates [9] [9] = {{{"A1"}, {"B1"}, {"C1"}, {"D1"}, {"E1"}, {"F1"}, {"G1"}, {"H1"}, {"I1"}},
                              {{"A2"}, {"B2"}, {"C2"}, {"D2"}, {"E2"}, {"F2"}, {"G2"}, {"H2"}, {"I2"}},
                              {{"A3"}, {"B3"}, {"C3"}, {"D3"}, {"E3"}, {"F3"}, {"G3"}, {"H3"}, {"I3"}},
                              {{"A4"}, {"B4"}, {"C4"}, {"D4"}, {"E4"}, {"F4"}, {"G4"}, {"H4"}, {"I4"}},
                              {{"A5"}, {"B5"}, {"C5"}, {"D5"}, {"E5"}, {"F5"}, {"G5"}, {"H5"}, {"I5"}},
                              {{"A6"}, {"B6"}, {"C6"}, {"D6"}, {"E6"}, {"F6"}, {"G6"}, {"H6"}, {"I6"}},
                              {{"A7"}, {"B7"}, {"C7"}, {"D7"}, {"E7"}, {"F7"}, {"G7"}, {"H7"}, {"I7"}},
                              {{"A8"}, {"B8"}, {"C8"}, {"D8"}, {"E8"}, {"F8"}, {"G8"}, {"H8"}, {"I8"}},
                              {{"A9"}, {"B9"}, {"C9"}, {"D9"}, {"E9"}, {"F9"}, {"G9"}, {"H9"}, {"I9"}}
                            };

int COUNTER = 0;

void min_max_iterative_depth(vector<vector<char>> state, bool white, string const &move, bool const &die){
  string & move_y = const_cast<string &>(move);
  bool & die_y = const_cast<bool &>(die);
  int depth = 2;
  while (true){ //rimetti a true
    move_y = min_max_player(state, white, depth);
    cout << "fatto " << depth << "(" << die_y << ")" << endl;
    depth++;
    if (die_y){
      cout << "Mi uccido!" << endl;
      cout << "Mi uccido!" << endl;
      cout << "Mi uccido!" << endl;
      cout << "Mi uccido!" << endl;
      cout << "Mi uccido!" << endl;
      cout << "Mi uccido!" << endl;
      cout << "Mi uccido!" << endl;
      cout << "Mi uccido!" << endl;
      cout << "Mi uccido!" << endl;
      cout << "Mi uccido!" << endl;
      cout << "Mi uccido!" << endl;
      cout << "Mi uccido!" << endl;
      cout << "Mi uccido!" << endl;
      cout << "Mi uccido!" << endl;
      cout << "Mi uccido!" << endl;
      cout << "Mi uccido!" << endl;
      cout << "Mi uccido!" << endl;
      cout << "Mi uccido!" << endl;
      cout << "Mi uccido!" << endl;

      exit(-1);
    }
  }
}

string min_max_player_wrapper(vector<vector<char>> state, bool white, int time){
    string retValue = "";
    bool die = false;
    try {
      std::thread t(min_max_iterative_depth, state, white, std::ref(retValue), std::ref(die));

      std::mutex m;
      std::condition_variable cv;

      t.detach();

      {
          std::unique_lock<std::mutex> l(m);
          if(cv.wait_for(l, chrono::duration<int, std::milli>(5*1000)) == std::cv_status::timeout)
              throw std::runtime_error("Timeout");
          die = true;
      }
    } catch(std::runtime_error& e) {
      cout << "Timeout :" << e.what() << endl;
      cout << "Migliore Ottenuto: " << retValue << endl;
      return retValue;
    }
    return retValue;
}

string min_max_player(vector<vector<char>> state, bool white, int max_depth){
  tuple<vector<vector<char>> , vector<int>, vector<int>> next_state;
  int value;
  if (white){
    tuple<tuple<vector<vector<char>> , vector<int>, vector<int>>, int> result = min_max(make_tuple(state, vector<int>(), vector<int>()), max_depth, max_depth, -10000, 10000, true);
    next_state = get<0>(result);
    int value = get<1>(result);
    // cout << "MY MOVE" << endl;
    // print_state(get<0>(next_state));
    // cout << "Best Value: " << value << '\n';
    // cout << "--------------------------" << endl;
  }else{
    tuple<tuple<vector<vector<char>> , vector<int>, vector<int> >, int> result = min_max(make_tuple(state, vector<int>(), vector<int>()), max_depth, max_depth, -10000, 10000, false);
    next_state = get<0>(result);
    int value = get<1>(result);
    print_state(get<0>(next_state));
    cout << "Best Value: " << value << '\n';
  }
  cout << "COUNTER: " << COUNTER << endl;
  return get_move_from_matrix(next_state);
}

string get_move_from_matrix(tuple<vector<vector<char>> , vector<int>, vector<int>> state){
  vector<int> start = get<1>(state);
  vector<int> end = get<2>(state);
  string str_start = coordinates[start[0]][start[1]];
  string str_end = coordinates[end[0]][end[1]];
  return "{\"from\":\"" + str_start + "\",\"to\":\"" + str_end +
                                    "\",\"turn\":";
}

vector<int> get_king_position_on_board(vector<vector<char>> state){
  for (int i = 0; i < 9; i++){
    for (int j = 0; j < 9; j++){
      if (state[i][j] == 3){
        vector<int> pos = {0, 0};
        pos[0] = i;
        pos[1] = j;
        return pos;
      }
    }
  }
  return vector<int>();
}

int get_num_of_white_peaces(vector<vector<char>> state){
  int counter = 0;
  for (int i = 0; i < 9; i++){
    for (int j = 0; j < 9; j++){
      if (state[i][j] == 1){
        counter ++;
      }
    }
  }
  return counter;
}

int get_num_of_black_peaces(vector<vector<char>> state){
  int counter = 0;
  for (int i = 0; i < 9; i++){
    for (int j = 0; j < 9; j++){
      if (state[i][j] == 2){
        counter ++;
      }
    }
  }
  return counter;
}

bool get_if_state_is_a_finish_game_state(vector<vector<char>> state){
  vector<int> pos = get_king_position_on_board(state);
  if (pos == vector<int>()){
    return true;
  }
  if (color[pos[0]][pos[1]] == 2){
    return true;
  }
  return false;
}

int get_mean_distance_of_blacks_from_king(vector<vector<char>> state, vector<int> king_pos){
  int dist = 0;
  for (int row = 0; row < 9; row++){
    for(int col = 0; col < 9; col++){
      if(state[row][col] == 2){
          dist = dist + sqrt((row - king_pos[0])*(row - king_pos[0]) + (col - king_pos[1])*(col - king_pos[1]));
      }
    }
  }
  return int((dist - 60)/2);
}

int get_black_near_king(vector<vector<char>> state, vector<int> king_pos){
  int count = 0;
  int k_x = king_pos[0], k_y = king_pos[1];
  if(state[k_x][k_y + 1] == 2)
    count++;
  if(state[k_x][k_y - 1] == 2)
    count++;
  if(state[k_x + 1][k_y] == 2)
    count++;
  if(state[k_x - 1][k_y] == 2)
    count++;
  return count;
}

int state_evaluation(vector<vector<char>> state){
  COUNTER++;
  vector<int> pos = get_king_position_on_board(state);
  if (pos == vector<int>()){
    return -1000;
  }
  if (color[pos[0]][pos[1]] == 2){
    return 1000;
  }
  int tot = 0;
  tot += get_num_of_white_peaces(state) * 15;
  tot += get_num_of_black_peaces(state) * -10;
  tot += get_mean_distance_of_blacks_from_king(state, pos);
  return tot;
}

int heuristic_wrapper(vector<vector<char>> state, bool white){
  if(white){
    return white_heuristic(state);
  } else{
     return black_heuristic(state);
  }
}

int white_heuristic(vector<vector<char>> state){
  vector<int> k_pos = get_king_position_on_board(state);
  if (k_pos == vector<int>()){
    return -1000;
  }
  if (color[k_pos[0]][k_pos[1]] == 2){
    return 1000;
  }
  int tot = 0;
  tot += get_num_of_white_peaces(state) * H_WHITE;
  tot += (B_PAWNS - get_num_of_black_peaces(state)) * H_B_EATEN;
  //tot += get_mean_distance_of_blacks_from_king(state, k_pos);
  tot -= get_black_near_king(state, k_pos) * H_B_NEAR_KING;
  return tot;
}

int black_heuristic(vector<vector<char>> state){
  vector<int> k_pos = get_king_position_on_board(state);
  if (k_pos == vector<int>()){
    return -1000;
  }
  if (color[k_pos[0]][k_pos[1]] == 2){
    return 1000;
  }
  int tot = 0;
  tot -= get_num_of_white_peaces(state) * H_WHITE;
  tot -= (W_PAWNS - get_num_of_white_peaces(state)) * H_W_EATEN;
  //tot -= get_mean_distance_of_blacks_from_king(state, k_pos);
  tot -= get_black_near_king(state, k_pos) * H_B_NEAR_KING;
  return tot;
}

tuple<tuple<vector<vector<char>> , vector<int>, vector<int>>, int> min_max(tuple<vector<vector<char>>, vector<int>, vector<int>> state, int depth, int max_depth, int alpha, int beta, bool maximize){
  vector<vector<char>> board = get<0>(state);
  if (depth == 0 || get_if_state_is_a_finish_game_state(board))
    return make_tuple(make_tuple(vector<vector<char>>(), vector<int>(), vector<int>()), state_evaluation(board));
  // fai un while true infinito e poi fai conto di avere la prossima mossa figlio in una variabile tipo child poi lo finisco io che è ciò che sto facendo ora

  if (maximize){
    // EVALUATION FOR MAXIMIZER
    int maxEval = -10000;
    tuple<vector<vector<char>>, vector<int>, vector<int>> child;
    tuple<vector<vector<char>>, vector<int>, vector<int>> maxChild;
    Moves M (get<0>(state), true);
    while (true){
      child = M.get_next_move();
      if (get<0>(child) == vector<vector<char>>()){
        break;
      }
      // cout << "-----------" << COUNTER << "-----------" << endl;
      // print_state(get<0>(child));
      // cout << "----------------------" << endl;
      int evaluation = get<1>(min_max(child, depth-1, max_depth, alpha, beta, false));
      if (evaluation > maxEval){
        maxEval = evaluation;
        if (depth == max_depth)
          maxChild = child;
      }
      if (evaluation > alpha){
        alpha = evaluation;
      }
      if (beta <= alpha){
        break;
      }
    }
    if (depth == max_depth)
      return make_tuple(maxChild, maxEval);
    return make_tuple(make_tuple(vector<vector<char>>(), vector<int>(), vector<int>()), maxEval);
  } else {
    // EVALUATION FOR MINIMIZER
    int minEval = 10000;
    tuple<vector<vector<char>>, vector<int>, vector<int>> child;
    tuple<vector<vector<char>>, vector<int>, vector<int>> minChild;
    Moves M(get<0>(state), false);
    while (true){
      child = M.get_next_move();
      if (get<0>(child) == vector<vector<char>>()){
        break;
      }
      int evaluation = get<1>(min_max(child, depth-1, max_depth, alpha, beta, true));
      if (evaluation < minEval){
        minEval = evaluation;
        if (depth == max_depth)
          minChild = child;
      }
      if (evaluation < beta){
        beta = evaluation;
      }
      if (beta <= alpha){
        break;
      }
    }
    if (depth == max_depth)
      return make_tuple(minChild, minEval);
    return make_tuple(make_tuple(vector<vector<char>>(), vector<int>(), vector<int>()), minEval);
  }
}
