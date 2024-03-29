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
#include <mutex>
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

const int color_heuristic[9][9] = {{0, 2, 2, 1, 1, 1, 2, 2, 0},
                                  {2, 0, 0, 0, 1, 0, 0, 0, 2},
                                  {2, 0, 0, 0, 0, 0, 0, 0, 2},
                                  {1, 0, 0, 0, 4, 0, 0, 0, 1},
                                  {1, 1, 0, 4, 1, 4, 0, 1, 1},
                                  {1, 0, 0, 0, 4, 0, 0, 0, 1},
                                  {2, 0, 0, 0, 0, 0, 0, 0, 2},
                                  {2, 0, 0, 0, 1, 0, 0, 0, 2},
                                  {0, 2, 2, 1, 1, 1, 2, 2, 0}};

int val_pos[9][9] = {{2, 1, 2, 6, 6, 6, 2, 1, 2},
                    {1, 4, 4,-3, 6,-3, 4, 4, 1},
                    {2, 4, 6, 4, 2, 4, 6, 4, 2},
                    {6,-3, 4, 6, 0, 6, 4,-3, 6},
                    {6, 6, 2, 0, 4, 0, 2, 6, 6},
                    {6,-3, 4, 6, 0, 6, 4,-3, 6},
                    {2, 4, 6, 4, 2, 4, 6, 4, 2},
                    {1, 4, 4,-3, 6,-3, 4, 4, 1},
                    {2, 1, 2, 6, 6, 6, 2, 1, 2}};
int COUNTER = 0;
time_t START_TIME = 0;
time_t MAX_TIME = 0;


string min_max_player_wrapper(vector<vector<char>> state, bool white, int time_needed, time_t start_time){
    string move;
    START_TIME = start_time;
    MAX_TIME = time_needed;
    int depth = 3;
    try {
      while (true){ //rimetti a true
        move = min_max_player(state, white, depth);
        cout << "Done depth " << depth << "!" << endl;
        depth++;
      }
    } catch(int e) {}
    return move;
}

string min_max_player(vector<vector<char>> state, bool white, int max_depth){
  tuple<vector<vector<char>> , vector<int>, vector<int>> next_state;
  int value;
  COUNTER = 0;
  if (white){
    tuple<tuple<vector<vector<char>> , vector<int>, vector<int>>, int> result = min_max(make_tuple(state, vector<int>(), vector<int>()), max_depth, max_depth, -10000, 10000, true);
    next_state = get<0>(result);
    value = get<1>(result);
    // cout << "MY MOVE" << endl;
    // print_state(get<0>(next_state));
    // cout << "Best Value: " << value << '\n';
    // cout << "--------------------------" << endl;
  }else{
    tuple<tuple<vector<vector<char>> , vector<int>, vector<int> >, int> result = min_max(make_tuple(state, vector<int>(), vector<int>()), max_depth, max_depth, -10000, 10000, false);
    next_state = get<0>(result);
    value = get<1>(result);
    // print_state(get<0>(next_state));
    // cout << "Best Value: " << value << '\n';
  }
  cout << "Move Counter: " << COUNTER << "\nBest Value: " << value << endl;
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

vector<int> get_num_of_peaces(vector<vector<char>> state){
  vector<int> counter;
  counter.push_back(0);
  counter.push_back(0);
  for (int i = 0; i < 9; i++){
    for (int j = 0; j < 9; j++){
      if (state[i][j] == 1){
        counter[0] ++;
      }else if (state[i][j] == 2){
        counter[1] ++;
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

// int get_mean_distance_of_blacks_from_king(vector<vector<char>> state, vector<int> king_pos){
//   int dist = 0;
//   for (int row = 0; row < 9; row++){
//     for(int col = 0; col < 9; col++){
//       if(state[row][col] == 2){
//           dist = dist + sqrt((row - king_pos[0])*(row - king_pos[0]) + (col - king_pos[1])*(col - king_pos[1]));
//       }
//     }
//   }
//   return int((dist - 60)/2);
// }

int get_num_of_blacks_around_king(vector<vector<char>> state, vector<int> king_pos){
  int start_raw, end_raw, start_column, end_column;
  int counter = 0;
  if (king_pos[0] > 0){
    start_raw = king_pos[0] - 1;
  }else{
    start_raw = king_pos[0];
  }
  if (king_pos[0] < 8){
    end_raw = king_pos[0] + 1;
  }else{
    end_raw = king_pos[0];
  }
  if (king_pos[1] > 0){
    start_column = king_pos[1] - 1;
  }else{
    start_column = king_pos[1];
  }
  if (king_pos[1] < 8){
    end_column = king_pos[1] + 1;
  }else{
    end_column = king_pos[1];
  }
  for (int i = start_raw; i <= end_raw; i++){
    for (int ii = start_column; ii <= end_column; ii++){
      if (state[i][ii] == 2){
        counter++;
      }
    }
  }
  return counter * 20;
}

int get_black_that_close_passage(vector<vector<char>> state){
  int counter = 0;
  if (state[2][1] == 2){
	  counter += 4;
  }
  if (state[1][2] == 2){
	  counter += 4;
  }
  if (state[1][6] == 2){
	  counter += 4;
  }
  if (state[2][7] == 2){
	  counter += 4;
  }
  if (state[6][1] == 2){
	  counter += 4;
  }
  if (state[7][2] == 2){
	  counter += 4;
  }
  if (state[6][7] == 2){
	  counter += 4;
  }
  if (state[7][6] == 2){
	  counter += 4;
  }
  
  if (state[2][2] == 2){
  	  counter += 10;
    }
  if (state[2][6] == 2){
  	  counter += 10;
    }
  if (state[6][2] == 2){
  	  counter += 10;
    }
  if (state[6][6] == 2){
  	  counter += 10;
    }
  
  
  if (state[2][2] == 3){
	  if (state[2][1] == 1 ^ state[1][2] == 1){
		  counter -= 20;
      }
  }
  if (state[6][2] == 3){
  	  if (state[6][1] == 1 ^ state[7][2] == 1){
  		  counter -= 20;
        }
    }
  if (state[2][6] == 3){
  	  if (state[1][6] == 1 ^ state[2][7] == 1){
  		  counter -= 20;
        }
    }
  if (state[6][6] == 3){
  	  if (state[7][6] == 1 ^ state[6][7] == 1){
  		  counter -= 20;
        }
    }
  return counter;
}

int get_mean_distance_of_blacks_from_king(vector<vector<char>> state, vector<int> king_pos){
  int dist = 0;
  for (int row = 0; row < 9; row++){
    for(int col = 0; col < 9; col++){
      if(state[row][col] == 2){
          dist = dist + (row - king_pos[0]) + (col - king_pos[1]);
      }
    }
  }
  return int((dist - 60)/2);
}

int get_value_of_positions(vector<vector<char>> state){
  int val = 0;
  for (int row = 0; row < 9; row++){
    for(int col = 0; col < 9; col++){
      if(state[row][col] == 1 || state[row][col] == 3){
          val = val + val_pos[row][col];
      }
      if(state[row][col] == 2){
          val = val - val_pos[row][col];
      }
    }
  }
  return int((val + 20)/4);
}

int get_rhombus(vector<vector<char>> state){
  int val = 0;
  for (int row = 0; row < 7; row++){
    for(int col = 1; col < 8; col++){
      if(state[row][col] == 1 || state[row][col] == 3 || color_heuristic[row][col] == 1){
          if((state[row+1][col-1] == 1 || state[row+1][col-1] == 3 || color_heuristic[row+1][col-1] == 1) &&
                  (state[row+1][col+1] == 1 || state[row+1][col+1] == 3 || color_heuristic[row+1][col+1] == 1) &&
                  (state[row+2][col] == 1 || state[row+2][col] == 3 || color_heuristic[row+2][col] == 1)){
              val = val + 10;
          }
      }
      if(state[row][col] == 2 || color_heuristic[row][col] == 1){
          if((state[row+1][col-1] == 2 || color_heuristic[row+1][col-1] == 1) && (state[row+1][col+1] == 2 || color_heuristic[row+1][col+1] == 1) &&
                  (state[row+2][col] == 2 || color_heuristic[row+2][col] == 1)){
              val = val - 10;
          }
      }
    }
  }
  return val;
}

int state_evaluation(vector<vector<char>> state, int depth){
  COUNTER++;
  if (COUNTER % 50000 == 0){
    if (time(NULL)-START_TIME > MAX_TIME-1){
      throw 10;
    }
  }
  vector<int> pos = get_king_position_on_board(state);
  if (pos == vector<int>()){
    return -1000+50-depth;
  }
  if (color[pos[0]][pos[1]] == 2){
    return 1000-50+depth;
  }
  int tot = 0;
  vector<int> peaces = get_num_of_peaces(state);
  tot += peaces[0] * 90;
  tot += peaces[1] * -60;
  tot += get_value_of_positions(state);
  //tot += get_rhombus(state);
  tot += get_mean_distance_of_blacks_from_king(state, pos);
  tot -= get_num_of_blacks_around_king(state, pos);
  tot -= get_black_that_close_passage(state);
  return tot + 300;
}

tuple<tuple<vector<vector<char>> , vector<int>, vector<int>>, int> min_max(tuple<vector<vector<char>>, vector<int>, vector<int>> state, int depth, int max_depth, int alpha, int beta, bool maximize){
  vector<vector<char>> board = get<0>(state);
  if (depth == 0 || get_if_state_is_a_finish_game_state(board))
    return make_tuple(make_tuple(vector<vector<char>>(), vector<int>(), vector<int>()), state_evaluation(board, depth));
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
