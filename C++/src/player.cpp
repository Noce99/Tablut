#include <unistd.h>
#include <iostream>
#include <string>
#include <string.h>
#include <iostream>
#include <vector>
#include "headers/min_max.hpp"
#include "headers/socket.hpp"

using namespace std;

vector<vector<char>> initial_state = {{0, 0, 0, 2, 2, 2, 0, 0, 0},
                                      {0, 0, 0, 0, 2, 0, 0, 0, 0},
                                      {0, 1, 0, 0, 0, 0, 0, 0, 0},
                                      {2, 0, 0, 0, 1, 0, 0, 0, 2},
                                      {2, 2, 1, 1, 3, 1, 1, 2, 2},
                                      {2, 0, 0, 0, 1, 0, 0, 0, 2},
                                      {0, 0, 0, 0, 1, 0, 0, 0, 0},
                                      {0, 0, 0, 0, 2, 0, 0, 0, 0},
                                      {0, 0, 0, 2, 2, 2, 0, 0, 0}
                                    };

void print_state(vector<vector<char>>);

int main(int argc, char *argv[]){
  int time;
  bool white;
  if (argc == 1){
    cout << "Usage: ./einars_player <black|white> [time]" << endl;
    cout << "You not specify player color\nExit...\n" << endl;
    exit(-1);
  }else if (argc == 2){
    if (strncmp(argv[1], "white", 5) == 0){
      white = true;
    }else if (strncmp(argv[1], "black", 5) == 0){
      white = false;
    } else {
      cout << "Choose black or white color!\nExit...\n" << endl;
    }
    time = 60;
  }else{
    if (strncmp(argv[1], "white", 5) == 0){
      white = true;
    }else if (strncmp(argv[1], "black", 5) == 0){
      white = false;
    }
    time = stoi(string(argv[2]));
  }
  cout << "Player: " << (white ? "WHITE" : "BLACK") << endl;
  cout << "Timeout: " << time << "s" << endl;
  if (white){
    Socket s (true);
    usleep(100*1000);
    s.recive_from_server();
    vector<vector<char>> state = initial_state;
    string result = min_max_player_wrapper(state, white, time) + "\"WHITE\"}";
    s.send_to_server(result);
    usleep(500*1000);
    s.recive_from_server();
    usleep(100*1000);
    while(true){
      vector<vector<char>> recived_status = s.recive_from_server();
      cout << "RECIVED FROM SERVER" << endl;
      print_state(recived_status);
      cout << "+++++++++++++++++++++++++++++++++" << endl;
      bool timedout = false;
      result = min_max_player_wrapper(recived_status, white, time) + "\"WHITE\"}";
      cout << result << endl;
      s.send_to_server(result);
      usleep(100*1000);
      s.recive_from_server();
      usleep(100*1000);
    }
  } else {
    Socket s (false);
    usleep(100*1000);
    vector<vector<char>> recived_status;
    string result;
    s.recive_from_server();
    usleep(100*1000);
    while(true){
      recived_status = s.recive_from_server();
      cout << "RECIVED FROM SERVER" << endl;
      print_state(recived_status);
      cout << "+++++++++++++++++++++++++++++++++" << endl;
      bool timedout = false;
      result = min_max_player_wrapper(recived_status, white, time) + "\"BLACK\"}";
      cout << result << endl;
      s.send_to_server(result);
      usleep(100*1000);
      s.recive_from_server();
      usleep(100*1000);
    }
  }
  // time_t start = time(NULL);
  // vector<vector<char>> state = initial_state;
  // string result = min_max_player(state, false);
  // time_t end = time(NULL);
  // cout << "Tempo impiegato: " << end -start << " s" << endl;
  return 0;
}

void print_state(vector<vector<char>> state){
  for (int i = 0; i < 9; i++){
    for (int j = 0; j < 9; j++){
      cout << int(state[i][j]) << " ";
    }
    cout << endl;
  }
}