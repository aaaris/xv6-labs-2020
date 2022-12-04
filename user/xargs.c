#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"  

void xargs(int cmdN, char *cmd[])
{
	char buf[512], *p;
    char *argv[MAXARG];
    int cnt = cmdN;
	
	if (cmdN + 2 > MAXARG) {
		fprintf(2,"xargs: too many args");
		exit(1);
	}
    for (int i = 0; i < cmdN; i++)
    {
        argv[i] = cmd[i];
    }
    p = buf;
	argv[cnt] = buf;
	argv[cnt+1] = 0;
    while (1)
    {
        if (read(0, p, 1) == 0)
            break;
        if (*p == '\n' || p == buf + 512)
        {
            *p = 0;
            p = buf; 
			int pid = fork();
			if (pid == 0){
				close(0);
				exec(argv[0],argv);
				fprintf(2,"xargs: exec error\n");
				exit(1);
			}
			else if (pid > 0)
			wait(0);
			else {
				fprintf(2,"xargs: fork error\n");
				exit(1);
			}
        }
        else
        {
            p++;
        }
    } 

}

void main(int argc, char *argv[])
{
    xargs(argc-1, &argv[1]);
    exit(0);
}