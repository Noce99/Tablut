#include <iostream>
#include <ostream>
#include <tuple>
#include <vector>
#include <string>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

string min_max_player_wrapper(vector<vector<char>>, bool, int);
bool check_if_deadth(vector<vector<char>>, int, int, int, int, int, int, int);
void print_state(vector<vector<char>>);
bool get_if_state_is_a_finish_game_state(vector<vector<char>>);
int state_evaluation(vector<vector<char>>);
vector<int> get_king_position_on_board(vector<vector<char>>);
vector<vector<int>> get_around(vector<int>);
tuple<tuple<vector<vector<char>> , vector<int>, vector<int>>, int> min_max(tuple<vector<vector<char>>, vector<int>, vector<int>>, int, int, int, int, bool);
bool check_if_killer(vector<vector<char>>, vector<int>, vector<int>);
tuple<vector<vector<char>>, vector<int>, vector<int>> get_new_state(vector<vector<char>>, vector<int>, vector<int>);
bool check_move(vector<vector<char>>, vector<int>, vector<int>);
string get_move_from_matrix(tuple<vector<vector<char>> , vector<int>, vector<int>>);
string min_max_player(vector<vector<char>>, bool);

#define PORT_WHITE 5800
#define PORT_BLACK 5801
int SOCK = 0;
string coordinates [9] [9] = {{{"A1"}, {"B1"}, {"C1"}, {"D1"}, {"E1"}, {"F1"}, {"G1"}, {"H1"}, {"I1"}},
                              {{"A2"}, {"B2"}, {"C2"}, {"D2"}, {"E2"}, {"F2"}, {"G2"}, {"H2"}, {"I2"}},
                              {{"A3"}, {"B3"}, {"C3"}, {"D3"}, {"E3"}, {"F3"}, {"G3"}, {"H3"}, {"I3"}},
                              {{"A4"}, {"B4"}, {"C4"}, {"D4"}, {"E4"}, {"F4"}, {"G4"}, {"H4"}, {"I4"}},
                              {{"A5"}, {"B5"}, {"C5"}, {"D5"}, {"E5"}, {"F5"}, {"G5"}, {"H5"}, {"I5"}},
                              {{"A6"}, {"B6"}, {"C6"}, {"D6"}, {"E6"}, {"F6"}, {"G6"}, {"H6"}, {"I6"}},
                              {{"A7"}, {"B7"}, {"C7"}, {"D7"}, {"E7"}, {"F7"}, {"G7"}, {"H7"}, {"I7"}},
                              {{"A8"}, {"B8"}, {"C8"}, {"D8"}, {"E8"}, {"F8"}, {"G8"}, {"H8"}, {"I8"}},
                              {{"A9"}, {"B9"}, {"C9"}, {"D9"}, {"E9"}, {"F9"}, {"G9"}, {"H9"}, {"I9"}}};

const int color [9] [9] = {{0, 2, 2, 5, 5, 5, 2, 2, 0},
                            {2, 0, 0, 0, 5, 0, 0, 0, 2},
                            {2, 0, 0, 0, 0, 0, 0, 0, 2},
                            {6, 0, 0, 0, 4, 0, 0, 0, 8},
                            {6, 6, 0, 4, 3, 4, 0, 8, 8},
                            {6, 0, 0, 0, 4, 0, 0, 0, 8},
                            {2, 0, 0, 0, 0, 0, 0, 0, 2},
                            {2, 0, 0, 0, 7, 0, 0, 0, 2},
                            {0, 2, 2, 7, 7, 7, 2, 2, 0}};

vector<vector<char>> initial_state = {{0, 0, 0, 2, 2, 2, 0, 0, 0},
                                      {0, 0, 0, 0, 2, 0, 0, 0, 0},
                                      {0, 1, 0, 0, 0, 0, 0, 0, 0},
                                      {2, 0, 0, 0, 1, 0, 0, 0, 2},
                                      {2, 2, 1, 1, 3, 1, 1, 2, 2},
                                      {2, 0, 0, 0, 1, 0, 0, 0, 2},
                                      {0, 0, 0, 0, 1, 0, 0, 0, 0},
                                      {0, 0, 0, 0, 2, 0, 0, 0, 0},
                                      {0, 0, 0, 2, 2, 2, 0, 0, 0}};

int COUNTER = 0;

string min_max_player_wrapper(vector<vector<char>> state, bool white, int time){
    string retValue = " ";
    try {
      std::mutex m;
      std::condition_variable cv;

      std::thread t([&](){
          while (true){
            retValue = min_max_player(state, white);
          }
          cv.notify_one();
      });

      t.detach();

      {
          std::unique_lock<std::mutex> l(m);
          if(cv.wait_for(l, chrono::duration<int, std::milli>(time)) == std::cv_status::timeout)
              throw std::runtime_error("Timeout");
      }
    } catch(std::runtime_error& e) {
      cout << "Timeout :" << e.what() << endl;
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
    cout << "MY MOVE" << endl;
    print_state(get<0>(next_state));
    cout << "Best Value: " << value << '\n';
    cout << "--------------------------" << endl;
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

class moves{
  public:
  vector<char> ai;
  vector<char> aj;
  vector<int> actual_peace = {0, 0};
  vector<vector<char>> actual_board;
  char actual_direction;
  bool done_actual_peace;
  int ii;
  int jj;

  moves(vector<vector<char>> board, bool white){
    actual_direction = 'u';
    done_actual_peace = true;
    actual_board = board;
    if (white){
      for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
          if (board[i][j] == 1){
            ai.push_back(i);
            aj.push_back(j);
          }
        }
      }
      vector<int> king_pos = get_king_position_on_board(board);
      if (king_pos != vector<int>()){
        ai.push_back(king_pos[0]);
        aj.push_back(king_pos[1]);
      }
    }else{
      for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
          if (board[i][j] == 2){
            ai.push_back(i);
            aj.push_back(j);
          }
        }
      }
    }
  }

  ~moves(){

  }

  tuple<vector<vector<char>> , vector<int>, vector<int>> get_next_move(){
    if (done_actual_peace == false){
      return get_next_move_for_peace();
    }else{
      if (ai.size() == 0){
        return make_tuple(vector<vector<char>>(), vector<int>(), vector<int>());
      }
      // Qui si può aggiungere la randomicità!
      int pi = ai[ai.size()-1];
      int pj = aj[aj.size()-1];
      // Nel caso qui non pop_back ma pop di quello che hai messo
      ai.pop_back();
      aj.pop_back();
      actual_peace[0] = pi;
      actual_peace[1] = pj;
      ii = pi;
      jj = pj;
      return get_next_move_for_peace();
    }
  }

  tuple<vector<vector<char>> , vector<int>, vector<int>> get_next_move_for_peace(){
    if (done_actual_peace){
      done_actual_peace = false;
      actual_direction = 'u';
    }
    if (actual_direction == 'u'){
      ii -= 1;
      vector<int> start = {0, 0};
      vector<int> end = {0, 0};
      int pi = actual_peace[0];
      int pj = actual_peace[1];
      start[0] = pi;
      start[1] = pj;
      end[0] = ii;
      end[1] = jj;
      if (ii >= 0 && check_move(actual_board, start, end)){
        return get_new_state(actual_board, start, end);
      }else{
        ii = pi;
        jj = pj;
        actual_direction = 'd';
        return get_next_move_for_peace();
      }
    }else if (actual_direction == 'd'){
      ii += 1;
      vector<int> start = {0, 0};
      vector<int> end = {0, 0};
      int pi = actual_peace[0];
      int pj = actual_peace[1];
      start[0] = pi;
      start[1] = pj;
      end[0] = ii;
      end[1] = jj;
      if (ii < 9 && check_move(actual_board, start, end)){
        return get_new_state(actual_board, start, end);
      }else{
        ii = pi;
        jj = pj;
        actual_direction = 'l';
        return get_next_move_for_peace();
      }
    }else if (actual_direction == 'l'){
      jj -= 1;
      vector<int> start = {0, 0};
      vector<int> end = {0, 0};
      int pi = actual_peace[0];
      int pj = actual_peace[1];
      start[0] = pi;
      start[1] = pj;
      end[0] = ii;
      end[1] = jj;
      if (jj >= 0 && check_move(actual_board, start, end)){
        return get_new_state(actual_board, start, end);
      }else{
        ii = pi;
        jj = pj;
        actual_direction = 'r';
        return get_next_move_for_peace();
      }
    }else if (actual_direction == 'r'){
      jj += 1;
      vector<int> start = {0, 0};
      vector<int> end = {0, 0};
      int pi = actual_peace[0];
      int pj = actual_peace[1];
      start[0] = pi;
      start[1] = pj;
      end[0] = ii;
      end[1] = jj;
      if (jj < 9 && check_move(actual_board, start, end)){
        return get_new_state(actual_board, start, end);
      }else{
        ii = pi;
        jj = pj;
        actual_direction = 'u';
        done_actual_peace = true;
        return get_next_move();
      }
    }
    return make_tuple(vector<vector<char>>(), vector<int>(), vector<int>());
  }
};

bool check_move(vector<vector<char>> state, vector<int> start, vector<int> end){
  bool white = false;
  if (state[start[0]][start[1]] == 1){
    white = true;
  }
  if (state[end[0]][end[1]] != 0){
    return false;
  }
  if (color[end[0]][end[1]] == 3){
    return false;
  }
  if (color[end[0]][end[1]] == 5 ||
      color[end[0]][end[1]] == 6 ||
      color[end[0]][end[1]] == 7 ||
      color[end[0]][end[1]] == 8){
    if (white){
      return false;
    }
    if (color[start[0]][start[1]] == color[end[0]][end[1]]){
      return true;
    }else{
      return false;
    }
  }
  return true;
}

tuple<vector<vector<char>> , vector<int>, vector<int>> get_new_state(vector<vector<char>> state, vector<int> start, vector<int> end){
  vector<vector<char>> new_state;
  for (int i = 0; i < 9; i++){
    new_state.push_back(vector<char>());
    for (int j = 0; j < 9; j++){
      new_state[i].push_back(state[i][j]);
    }
  }
  int i = start[0];
  int j = start[1];
  int ii = end[0];
  int jj = end[1];
  int attack_color = color[ii][jj];
  new_state[ii][jj] = new_state[i][j];
  new_state[i][j] = 0;

  int i_check = 0;
  int j_check = 0;
  int i_check_2 = 0;
  int j_check_2 = 0;
  //SOPRA
  if (end[0] >= 2){
    i_check = ii - 1;
    j_check = jj;
    i_check_2 = ii - 2;
    j_check_2 = jj;
    if (check_if_deadth(new_state, attack_color, ii, jj,  i_check, j_check, i_check_2, j_check_2)){
      new_state[i_check][j_check] = 0;
    }
  }
  //SOTTO
  if (end[0] <= 6){
    i_check = ii + 1;
    j_check = jj;
    i_check_2 = ii + 2;
    j_check_2 = jj;
    if (check_if_deadth(new_state, attack_color, ii, jj, i_check, j_check, i_check_2, j_check_2)){
      new_state[i_check][j_check] = 0;
    }
  }
  //SINISTRA
  if (end[1] >= 2){
    i_check = ii;
    j_check = jj - 1;
    i_check_2 = ii;
    j_check_2 = jj - 2;
    if (check_if_deadth(new_state, attack_color, ii, jj, i_check, j_check, i_check_2, j_check_2)){
      new_state[i_check][j_check] = 0;
    }
  }
  //DESTRA
  if (end[1] <= 6){
    i_check = ii;
    j_check = jj + 1;
    i_check_2 = ii;
    j_check_2 = jj + 2;
    if (check_if_deadth(new_state, attack_color, ii, jj, i_check, j_check, i_check_2, j_check_2)){
      new_state[i_check][j_check] = 0;
    }
  }
  return make_tuple(new_state, start, end);
  }

bool check_if_deadth(vector<vector<char>> new_state,int attack_color, int ii, int jj, int i_check, int j_check, int i_check_2, int j_check_2){
  int defense_color = color[i_check][j_check];
  int attack_type = new_state[ii][jj];
  int defence_type = new_state[i_check][j_check];
  if (new_state[i_check][j_check] != -1 && new_state[i_check][j_check] != 0){
    defense_color = color[i_check][j_check];
    if ((attack_type == 1 || attack_type==3) && defence_type == 2){
      if (new_state[i_check_2][j_check_2] == 1 || new_state[i_check_2][j_check_2] == 3){
        return true;
      }else if (color[i_check_2][j_check_2] == 3){
        return true;
      }else if ((color[i_check_2][j_check_2] == 5 ||
                  color[i_check_2][j_check_2] == 6 ||
                  color[i_check_2][j_check_2] == 7 ||
                  color[i_check_2][j_check_2] == 8 ||
                  color[i_check_2][j_check_2] == 3) &&
                (color[i_check][j_check] != 5 &&
                  color[i_check][j_check] != 6 &&
                  color[i_check][j_check] != 7 &&
                  color[i_check][j_check] != 8 &&
                  color[i_check][j_check] != 3)){
        return true;
      }
    }else if (attack_type == 2 && defence_type == 1){
      if (new_state[i_check_2][j_check_2] == 2){
        return true;
      }else if (color[i_check_2][j_check_2] == 3){
        return true;
      }else if ((color[i_check_2][j_check_2] == 5 ||
                  color[i_check_2][j_check_2] == 6 ||
                  color[i_check_2][j_check_2] == 7 ||
                  color[i_check_2][j_check_2] == 8) &&
                (color[i_check][j_check] != 5 &&
                  color[i_check][j_check] != 6 &&
                  color[i_check][j_check] != 7 &&
                  color[i_check][j_check] != 8)){
        return true;
      }
    }else if (attack_type == 2 && defence_type == 3){
      int sum = new_state[i_check+1][j_check] + new_state[i_check-1][j_check] + new_state[i_check][j_check+1] + new_state[i_check][j_check-1];
      if (defense_color == 3){
        if (sum == 8){
          return true;
        }
      }else if (defense_color == 4){
        if (sum == 6){
          return true;
        }
      }else if (new_state[i_check_2][j_check_2] == 2){
        return true;
      }else if ((color[i_check_2][j_check_2] == 5 ||
                  color[i_check_2][j_check_2] == 6 ||
                  color[i_check_2][j_check_2] == 7 ||
                  color[i_check_2][j_check_2] == 8) &&
                (color[i_check][j_check] != 5 &&
                  color[i_check][j_check] != 6 &&
                  color[i_check][j_check] != 7 &&
                  color[i_check][j_check] != 8)){
        return true;
      }
    }
  }
  return false;
}

vector<vector<int>> get_around(vector<int> pos){
    vector<vector<int>> to_check;
    for (int k = 0;k < 4; k++){
      vector<int> pos = {-1, -1};
      to_check.push_back(pos);
    }
    int i = 0;
    int ii = pos[0];
    int jj = pos[1];
    if (ii != 0){
      to_check[i][0] = ii - 1;
      to_check[i][1] = jj;
      i++;
    }
    if (jj != 8){
      to_check[i][0] = ii;
      to_check[i][1] = jj + 1;
      i++;
    }
    if (ii != 8){
      to_check[i][0] = ii + 1;
      to_check[i][1] = jj;
      i++;
    }
    if (jj != 0){
      to_check[i][0] = ii;
      to_check[i][1] = jj - 1;
      i++;
    }
    return to_check;
}

bool check_if_killer(vector<vector<char>> state, vector<int> pos, vector<int> vittima){
  if (pos[0] == -1 && pos[1] == -1){
    return false;
  }
  if (state[vittima[0]][vittima[1]] == 0){
    return false;
  }
  if ((color[pos[0]][pos[1]] == 5 ||
              color[pos[0]][pos[1]] == 6 ||
              color[pos[0]][pos[1]] == 7 ||
              color[pos[0]][pos[1]] == 8 ||
              color[pos[0]][pos[1]] == 3) &&
            (color[vittima[0]][vittima[1]] != 5 &&
              color[vittima[0]][vittima[1]] != 6 &&
              color[vittima[0]][vittima[1]] != 7 &&
              color[vittima[0]][vittima[1]] != 8 &&
              color[vittima[0]][vittima[1]] != 3)){
    return true;
  }
  if (state[pos[0]][pos[1]] == 0){
      return false;
  }
  if (state[vittima[0]][vittima[1]] == 2){
    if (state[pos[0]][pos[1]] == 1 or state[pos[0]][pos[1]] == 3){
      return true;
    }
  }else{
    if (state[pos[0]][pos[1]] == 2){
      return true;
    }
  }
  return false;
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
    moves M(get<0>(state), true);
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
    moves M(get<0>(state), false);
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

void print_state(vector<vector<char>> state){
  for (int i = 0; i < 9; i++){
    for (int j = 0; j < 9; j++){
      cout << int(state[i][j]) << " ";
    }
    cout << endl;
  }
}

void initialize_socket(bool white){
  int valread;
  struct sockaddr_in serv_addr;
  string ss = "\"Eimar\"";
  const char * name = ss.c_str();
  char buffer[1024] = {0};
  if ((SOCK = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    printf("\n Socket creation error \n");
  }
  serv_addr.sin_family = AF_INET;
  if (white){
    serv_addr.sin_port = htons(PORT_WHITE);
  }else{
    serv_addr.sin_port = htons(PORT_BLACK);
  }
  if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0){
    printf("\nInvalid address/ Address not supported \n");
  }
  if (connect(SOCK, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
    printf("\nConnection Failed \n");
  }
  int name_lenght = ss.length();
  char * num = reinterpret_cast<char *>(&name_lenght);
  char * num_completo = new char[4];
  num_completo[0] = 0;
  num_completo[1] = 0;
  num_completo[2] = 0;
  num_completo[3] = num[0];
  send(SOCK, num_completo, 4, 0);
  send(SOCK , name, name_lenght, 0 );
  printf("My name sent!\n");
  int ricevuti = read(SOCK , buffer, 1);
}

void send_to_server(string ss){
  int name_lenght = ss.length();
  char * num = reinterpret_cast<char *>(&name_lenght);
  cout << int(num[1]) << " " << int(num[0]) << endl;
  char * num_completo = new char[4];
  num_completo[0] = 0;
  num_completo[1] = 0;
  num_completo[2] = 0;
  num_completo[3] = num[0];
  send(SOCK, num_completo, 4, 0);
  send(SOCK , ss.c_str(), name_lenght, 0 );
}

vector<vector<char>> recive_from_server(){
  char * buffer = new char[2048];
  int ricevuti = -1;
  while(true){
    ricevuti = read(SOCK , buffer, 1024);
    if (ricevuti >= 600){
      break;
    }
  }
  int interesting_finish = 673;
  vector<vector<char>> recived_status;
  recived_status.push_back(vector<char>());
  int i = 0;
  while(true){
    if (buffer[i] == '['){
      break;
    }
    i++;
  }
  string ss = "";
  while (true){
    if (buffer[i] == 't'){
      break;
    }
    if (buffer[i-2] == ']' && buffer[i-1] == ',' && buffer[i] == '['){
      recived_status.push_back(vector<char>());
      i++;
      i++;
    }
    ss.clear();
    while(true){
      if (buffer[i] == '"'){
        break;
      }
      ss += buffer[i];
      i++;
    }
    if (ss.compare("WHITE") == 0){
      recived_status[recived_status.size()-1].push_back(1);
    }else if (ss.compare("BLACK") == 0){
      recived_status[recived_status.size()-1].push_back(2);
    }else if (ss.compare("EMPTY") == 0){
      recived_status[recived_status.size()-1].push_back(0);
    }else if (ss.compare("KING") == 0){
      recived_status[recived_status.size()-1].push_back(3);
    }else if (ss.compare("THRONE") == 0){
      recived_status[recived_status.size()-1].push_back(0);
    }
    while(true){
      char el = buffer[i];
      if (buffer[i] != '"' && buffer[i] != ',' && buffer[i] != ']'){
        break;
      }
      i++;
    }
  }
  cout << endl;
  return recived_status;
}

int main(int argc, char *argv[]){
  int TIME;
  bool COLOR;
  if (argc == 1){
    cout << "./min_max_player <black|white> <time>" << endl;
    cout << "Non mi hai detto il colore!" << endl;
    return -1;
  }else if (argc == 2){
    if (strcmp(argv[1], "white") == 0){
      COLOR = true;
    }else if (strcmp(argv[1], "black") == 0){
      COLOR = false;
    }
    TIME = 60;
    cout << COLOR << endl;
  }else{
    if (strcmp(argv[1], "white") == 0){
      COLOR = true;
    }else if (strcmp(argv[1], "black") == 0){
      COLOR = false;
    }
    TIME = stoi(string(argv[2]));
  }
  cout << "Tempo per mossa: " << TIME << endl;
  bool server = true;
  if (server && COLOR){
    initialize_socket(true);
    usleep(100*1000);
    recive_from_server();
    vector<vector<char>> state = initial_state;
    string result = min_max_player_wrapper(state, COLOR, TIME) + "\"WHITE\"}";
    send_to_server(result);
    usleep(500*1000);
    recive_from_server();
    usleep(100*1000);
    while(true){
      vector<vector<char>> recived_status = recive_from_server();
      cout << "RECIVED FROM SERVER" << endl;
      print_state(recived_status);
      cout << "+++++++++++++++++++++++++++++++++" << endl;
      bool timedout = false;
      result = min_max_player_wrapper(recived_status, COLOR, TIME) + "\"WHITE\"}";
      cout << result << endl;
      send_to_server(result);
      usleep(100*1000);
      recive_from_server();
      usleep(100*1000);
    }
  }
  if (server && !COLOR){
    initialize_socket(false);
    usleep(100*1000);
    vector<vector<char>> recived_status;
    string result;
    recive_from_server();
    usleep(100*1000);
    while(true){
      recived_status = recive_from_server();
      cout << "RECIVED FROM SERVER" << endl;
      print_state(recived_status);
      cout << "+++++++++++++++++++++++++++++++++" << endl;
      bool timedout = false;
      result = min_max_player_wrapper(recived_status, COLOR, TIME) + "\"BLACK\"}";
      cout << result << endl;
      send_to_server(result);
      usleep(100*1000);
      recive_from_server();
      usleep(100*1000);
    }
  }
  time_t start = time(NULL);
  vector<vector<char>> state = initial_state;
  string result = min_max_player(state, false);
  time_t end = time(NULL);
  cout << "Tempo impiegato: " << end -start << " s" << endl;
  return 0;
}

// DEPTH 5 -> 13 s
