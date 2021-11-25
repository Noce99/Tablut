#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>
#include <time.h>

using namespace std;

#define MSGSIZE 16
char* msg1 = "hello, world #1";

int main(){
  int p[2];
  time_t start = time(NULL);
  time_t end = 0;
  if (pipe(p) < 0)
        exit(1);
  int pid = fork();
  if (pid > 0){
    close(p[0]);
    char * msg = new char[3]{'a', 'b', 'c'};
    write(p[1], msg1, MSGSIZE);
    usleep(5000000);
    write(p[1], msg1, MSGSIZE);
    usleep(10000000);
    write(p[1], msg1, MSGSIZE);
    wait(NULL);
  }else{
    close(p[1]);
    while (true){
      end = time(NULL);
      if (end-start > 3){
        cout << end-start << endl;
        break;
      }
    }
    //kill(pid, SIGTERM);
    char inbuf[MSGSIZE];
    while (true){
      int nbytes = read(p[0], inbuf, MSGSIZE);
      printf("% s\n", inbuf);
      if (nbytes <= 0){
        break;
      }
    }
    cout << "Tempo impiegato: " << end - start << " s" << endl;
  }
  cout << p[0] << " " << p[1] << endl;
}
