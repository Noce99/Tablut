#include <vector>
#include <tuple>
#include <string>
#include "headers/moves.hpp"
#include "headers/utils.hpp"

using namespace std;

Moves::Moves(vector<vector<char>> board, bool white){
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


Moves::Moves(){

}

tuple<vector<vector<char>> , vector<int>, vector<int>> Moves::get_next_move(){
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

tuple<vector<vector<char>> , vector<int>, vector<int>> Moves::get_next_move_for_peace(){
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

bool Moves::check_move(vector<vector<char>> state, vector<int> start, vector<int> end){
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

tuple<vector<vector<char>> , vector<int>, vector<int>> Moves::get_new_state(vector<vector<char>> state, vector<int> start, vector<int> end){
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

bool Moves::check_if_deadth(vector<vector<char>> new_state,int attack_color, int ii, int jj, int i_check, int j_check, int i_check_2, int j_check_2){
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

vector<vector<int>> Moves::get_around(vector<int> pos){
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

bool Moves::check_if_killer(vector<vector<char>> state, vector<int> pos, vector<int> vittima){
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

vector<int> Moves::get_king_position_on_board(vector<vector<char>> state){
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