#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
	if (argc <= 1)
	{
		fprintf(2, "Usage: %s <tick>\n", argv[0]);
		exit(1);
	}
	int tick = atoi(argv[1]);
	int res = sleep(tick);
	if (res == -1)
	{
		fprintf(2, "sleep: error\n");
		exit(1);
	}
	exit(0);
}