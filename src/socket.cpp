#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <vector>
#include <string>
#include <string.h>
#include <iostream>
#include "headers/socket.hpp"

using namespace std;

Socket::Socket(bool white, string ip){
    struct sockaddr_in serv_addr;
    string ss = "\"Einars\"";
    const char * name = ss.c_str();
    char buffer[1024] = {0};
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("\n Socket creation error \n");
    }
    serv_addr.sin_family = AF_INET;
    if (white){
        serv_addr.sin_port = htons(PORT_WHITE);
    }else{
        serv_addr.sin_port = htons(PORT_BLACK);
    }
    if(inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr)<=0){
        printf("\nInvalid address/ Address not supported \n");
    }
    if (connect(fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
        printf("\nConnection Failed \n");
    }
    int name_lenght = ss.length();
    char * num = reinterpret_cast<char *>(&name_lenght);
    char * num_completo = new char[4];
    num_completo[0] = 0;
    num_completo[1] = 0;
    num_completo[2] = 0;
    num_completo[3] = num[0];
    if(send(fd, num_completo, 4, 0) < 0){
        printf("Error during sending team's name!\nExit...\n");
        exit(-1);
    }
    if(send(fd , name, name_lenght, 0 ) < 0){
        printf("Error during sending team's name!\nExit...\n");
        exit(-1);
    }
    printf("My name sent!\n");
    int ricevuti = read(fd , buffer, 1);
}

void Socket::send_to_server(string ss){
    int name_lenght = ss.length();
    char * num = reinterpret_cast<char *>(&name_lenght);
    // cout << int(num[1]) << " " << int(num[0]) << endl;
    char * num_completo = new char[4];
    num_completo[0] = 0;
    num_completo[1] = 0;
    num_completo[2] = 0;
    num_completo[3] = num[0];
    send(fd, num_completo, 4, 0);
    send(fd , ss.c_str(), name_lenght, 0 );
}

vector<vector<char>> Socket::recive_from_server(){
    char * buffer = new char[2048];
    int ricevuti = -1;
    while(true){
        ricevuti = read(fd , buffer, 1024);
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
    //cout << endl;
    return recived_status;
}
