#include <unistd.h>
#include <stdio.h>
#include <fstream>
#include <sys/wait.h>
using namespace std;

const int PROCESSES = 5;
void Printer(int psno);
int iAmProcess = 0;

int main()
{

    int pids[PROCESSES];

    Printer(iAmProcess);

    for (int j = 0; j < PROCESSES; j++)
    {
        if (pids[j] = fork() < 0)
        {
            perror("Error in forking.\n");
            exit(EXIT_FAILURE);
        }
        else if (pids[j] == 0)
        {
            Printer(iAmProcess);
            exit(0);
        }
    }

    int status;
    int pid;
    int n = PROCESSES;
    while (n > 0)
    {
        pid= wait(&status);
        --n;
    }
    return 0;
}

void Printer(int psno)
{
    printf("I am process %d\n", psno);
}
