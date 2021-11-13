#include <iostream>
#include <ostream>
#include <tuple>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

void print_state(vector<vector<char>>);
bool get_if_state_is_a_finish_game_state(vector<vector<char>>);
int state_evaluation(vector<vector<char>>);
int * get_king_position_on_board(vector<vector<char>>);
int ** get_around(int *);
tuple<tuple<vector<vector<char>> , int *, int *>, int> min_max(tuple<vector<vector<char>>, int *, int *>, int, int, int, int, bool);
bool check_if_killer(vector<vector<char>>, int *, int *);
tuple<vector<vector<char>>, int *, int *> get_new_state(vector<vector<char>>, int *, int *);
bool check_move(vector<vector<char>>, int *, int *);
string get_move_from_matrix(tuple<vector<vector<char>> , int *, int *>);
string min_max_player(vector<vector<char>>, bool);

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
                                      {0, 0, 0, 0, 1, 0, 0, 0, 0},
                                      {2, 0, 0, 0, 1, 0, 0, 0, 2},
                                      {2, 2, 1, 1, 3, 1, 1, 2, 2},
                                      {2, 0, 0, 0, 1, 0, 0, 0, 2},
                                      {0, 0, 0, 0, 1, 0, 0, 0, 0},
                                      {0, 0, 0, 0, 2, 0, 0, 0, 0},
                                      {0, 0, 0, 2, 2, 2, 0, 0, 0}};

int COUNTER = 0;

string min_max_player(vector<vector<char>> state, bool white){
  int MAX_DEPH = 4;
  tuple<vector<vector<char>> , int *, int *> next_state;
  int value;
  if (white){
    tuple<tuple<vector<vector<char>> , int *, int *>, int> result = min_max(make_tuple(state, nullptr, nullptr), MAX_DEPH, MAX_DEPH, -10000, 10000, true);
    tuple<vector<vector<char>> , int *, int *> next_state = get<0>(result);
    int value = get<1>(result);
    print_state(get<0>(next_state));
    cout << "Best Value: " << value << '\n';
  }else{
    tuple<tuple<vector<vector<char>> , int *, int *>, int> result = min_max(make_tuple(state, nullptr, nullptr), MAX_DEPH, MAX_DEPH, -10000, 10000, false);
    tuple<vector<vector<char>> , int *, int *> next_state = get<0>(result);
    int value = get<1>(result);
    cout << "Best Value: " << value << '\n';
  }
  cout << "COUNTER: " << COUNTER << endl;
  return "ciao";
  return get_move_from_matrix(next_state);
}

string get_move_from_matrix(tuple<vector<vector<char>> , int *, int *> state){
  vector<vector<char>> board = get<0>(state);
  int * start = get<1>(state);
  int * end = get<2>(state);
  string str_start = coordinates[get<1>(state)[0]][get<1>(state)[1]];
  string str_end = coordinates[end[0]][end[1]];
  return str_start + "to" + str_end;
}

class moves{
  public:
  vector<char> ai;
  vector<char> aj;
  int * actual_peace = new int[2];
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
      int * king_pos = get_king_position_on_board(board);
      if (king_pos != nullptr){
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

  tuple<vector<vector<char>> , int *, int *> get_next_move(){
    if (done_actual_peace == false){
      return get_next_move_for_peace();
    }else{
      if (ai.size() == 0){
        return make_tuple(vector<vector<char>>(), nullptr, nullptr);
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

  tuple<vector<vector<char>> , int *, int *> get_next_move_for_peace(){
    if (done_actual_peace){
      done_actual_peace = false;
      actual_direction = 'u';
    }
    if (actual_direction == 'u'){
      ii -= 1;
      int * start = new int[2];
      int * end = new int[2];
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
      int * start = new int[2];
      int * end = new int[2];
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
      int * start = new int[2];
      int * end = new int[2];
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
      int * start = new int[2];
      int * end = new int[2];
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
    return make_tuple(vector<vector<char>>(), nullptr, nullptr);
  }
};

bool check_move(vector<vector<char>> state, int * start, int * end){
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

tuple<vector<vector<char>> , int *, int *> get_new_state(vector<vector<char>> state, int * start, int * end){
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
  new_state[ii][jj] = new_state[i][j];
  new_state[i][j] = 0;
  int ** to_check = get_around(end);
  for (int k = 0; k < 4; k++){
    int * e = to_check[k];
    if (e[0] != -1){
      int ** around = get_around(e);
      int ** opposites_1 = new int*[2];
      opposites_1[0] = around[0];
      opposites_1[1] = around[2];
      int ** opposites_2 = new int*[2];
      opposites_2[0] = around[1];
      opposites_2[1] = around[3];
      if ((new_state[e[0]][e[1]] == 3) && (new_state[ii][jj] == 2)){
        if ((color[e[0]][e[1]] == 3) || (color[e[0]][e[1]] == 4)){
          if (check_if_killer(new_state, around[0], e) &&
              check_if_killer(new_state, around[2], e) &&
              check_if_killer(new_state, around[1], e) &&
              check_if_killer(new_state, around[3], e)){
            new_state[e[0]][e[1]] = 0;
          }
        }else if (((end[0] == opposites_1[0][0] && end[1] == opposites_1[0][1]) || (end[0] == opposites_1[1][0] && end[1] == opposites_1[1][1])) &&
                   check_if_killer(new_state, around[0], e) &&
                   check_if_killer(new_state, around[2], e)){
          new_state[e[0]][e[1]] = 0;
        }else if (((end[0] == opposites_2[0][0] && end[1] == opposites_2[0][1]) || (end[0] == opposites_2[1][0] && end[1] == opposites_2[1][1])) &&
                   check_if_killer(new_state, around[1], e) &&
                   check_if_killer(new_state, around[3], e)){
          new_state[e[0]][e[1]] = 0;
        }
      }else{
        if (((end[0] == opposites_1[0][0] && end[1] == opposites_1[0][1]) || (end[0] == opposites_1[1][0] && end[1] == opposites_1[1][1])) && check_if_killer(new_state, around[0], e) && check_if_killer(new_state, around[2], e)){
          new_state[e[0]][e[1]] = 0;
        }else if (((end[0] == opposites_2[0][0] && end[1] == opposites_2[0][1]) || (end[0] == opposites_2[1][0] && end[1] == opposites_2[1][1])) && check_if_killer(new_state, around[1], e) && check_if_killer(new_state, around[3], e)){
          new_state[e[0]][e[1]] = 0;
        }
      }
    }
  }
  return make_tuple(new_state, start, end);
}

int ** get_around(int * pos){
    int ** to_check = new int *[4];
    for (int k = 0;k < 4; k++){
      to_check[k] = new int[2];
      to_check[k][0] = -1;
      to_check[k][1] = -1;
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

bool check_if_killer(vector<vector<char>> state, int * pos, int * vittima){
  if (pos[0] == -1 && pos[1] == -1){
    return false;
  }
  if (state[vittima[0]][vittima[1]] == 0){
    return false;
  }
  if (state[pos[0]][pos[1]] == 0){
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

int * get_king_position_on_board(vector<vector<char>> state){
  for (int i = 0; i < 9; i++){
    for (int j = 0; j < 9; j++){
      if (state[i][j] == 3){
        int * pos = new int[2];
        pos[0] = i;
        pos[1] = j;
        return pos;
      }
    }
  }
  return nullptr;
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
  int * pos = get_king_position_on_board(state);
  if (pos == nullptr){
    return true;
  }
  if (color[pos[0]][pos[1]] == 2){
    return true;
  }
  return false;
}

int state_evaluation(vector<vector<char>> state){
  COUNTER++;
  int * pos = get_king_position_on_board(state);
  if (pos == nullptr){
    return -1000;
  }
  if (color[pos[0]][pos[1]] == 2){
    return 1000;
  }
  int tot = 0;
  tot += get_num_of_white_peaces(state) * 15;
  tot += get_num_of_black_peaces(state) * -10;
  return tot;
}

tuple<tuple<vector<vector<char>> , int *, int *>, int> min_max(tuple<vector<vector<char>>, int *, int *> state, int depth, int max_depth, int alpha, int beta, bool maximize){
  vector<vector<char>> board = get<0>(state);
  if (depth == 0 || get_if_state_is_a_finish_game_state(board))
    return make_tuple(make_tuple(vector<vector<char>>(), nullptr, nullptr), state_evaluation(board));
  // fai un while true infinito e poi fai conto di avere la prossima mossa figlio in una variabile tipo child poi lo finisco io che è ciò che sto facendo ora

  if (maximize){
    // EVALUATION FOR MAXIMIZER
    int maxEval = -10000;
    tuple<vector<vector<char>>, int *, int *> child;
    tuple<vector<vector<char>>, int *, int *> maxChild;
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
      if (beta <= alpha)
        break;
    }
    if (depth == max_depth)
      return make_tuple(maxChild, maxEval);
    return make_tuple(make_tuple(vector<vector<char>>(), nullptr, nullptr), maxEval);
  } else {
    // EVALUATION FOR MINIMIZER
    int minEval = 10000;
    tuple<vector<vector<char>>, int *, int *> child;
    tuple<vector<vector<char>>, int *, int *> minChild;
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
      if (beta <= alpha)
        break;
    }
    if (depth == max_depth)
      return make_tuple(minChild, minEval);
    return make_tuple(make_tuple(vector<vector<char>>(), nullptr, nullptr), minEval);
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

int main(){
  cout << "Inizio a calcolare le mosse per lo stato iniziale!" << endl;
  min_max_player(initial_state, true);
  return 0;
}
