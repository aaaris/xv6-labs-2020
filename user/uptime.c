#include "kernel/types.h"
#include "user/user.h" 

void main(int argc, char *argv[])
{
	int tick = uptime();
	printf("%d\n", tick);
    exit(0);
}
