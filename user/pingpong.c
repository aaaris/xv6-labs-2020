#include "kernel/types.h"
#include "user/user.h"

void main(int argc, int argv[])
{
    int p1[2];
    int p2[2];
    if (pipe(p1) == -1) {
        fprintf(2,"pipe: error\n");
    }
    if (pipe(p2) == -1) {
        fprintf(2,"pipe: error\n");
    }
    int pid = fork();
    if (pid > 0)
    {
        close(p1[0]);
        close(p2[1]);
        write(p1[1], "a", 1);
        int res = read(p2[0], "a", 1);
        if (res > 0)
        {
            printf("%d: received pong\n", getpid());
        }
        close(p2[0]);
        close(p1[1]);
    }
    else if (pid == 0)
    {
        char buf[2];
        int res;
        close(p1[1]);
        close(p2[0]);
        if ((res = read(p1[0], buf, 1)) > 0) {
            printf("%d: received ping\n", getpid());
            write(p2[1], buf, 1);
        }
        close(p1[0]);
        close(p2[1]);
        exit(0);
    }
    else
    {
        fprintf(2, "fork: error");
        exit(1);
    }
    exit(0);
}
