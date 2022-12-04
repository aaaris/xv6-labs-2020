#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void main(int argc, char *argv[])
{
    int left[2];
    if (pipe(left) == -1)
    {
        fprintf(2, "pipe: error");
        exit(1);
    }
    int pid = fork();
    if (pid > 0)
    {
        int i;
        close(left[0]);
        for (i = 2; i <= 35; i++)
        {
            write(left[1], &i, sizeof(i));
        }
        close(left[1]);
        wait(0);
    }
    else if (pid == 0)
    {
        int prime, num;
		int flag = 0;
		int right[2];
        close(left[1]);
        read(left[0], &prime, sizeof(prime));
        printf("prime %d\n", prime);
        int len = 0;
        while ((len = read(left[0], &num, sizeof(num))) != 0)
        {
            if (num % prime!= 0)
            {
				// if right pipe has been built
				if (flag) {
					write(right[1], &num, sizeof(num));
				}
				else {
					// build right pipe
					if (pipe(right) == -1) {
						fprintf(2, "pipe: error\n");
						exit(1);
					}
					// fork child 
					int pid = fork(); 
					if (pid == 0) {
						close(right[1]);	
						close(left[0]);
						// redirect right[0] to left[0] for child
						dup(right[0]);
						close(right[0]);
						read(left[0], &prime, sizeof(prime));
						printf("prime %d\n", prime);
					}	
					else if (pid > 0) {
						flag = 1;
						close(right[0]);
						write(right[1], &num, sizeof(num));
					}
					else {
						fprintf(2, "fork: error");
						exit(1);
					}
				}
            }
        }
        close(left[0]);
		// if process fork
        if (flag)
        {
			close(right[1]);
            wait(0);
        }
        exit(0);
    }
    else
    {
        fprintf(2, "fork: error");
        exit(1);
    }
    exit(0);
}