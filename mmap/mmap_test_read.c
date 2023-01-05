#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>

#define LEN (10 * 4096)

int main(void)
{
    int fd, loop;
    char *vadr;

    if ((fd = open("/dev/mmaptest", O_RDWR)) < 0)
    {
        printf("open device /dev/mmaptest failed, errno is %d\n", errno);
        return 0;
    }
    vadr = mmap(0, LEN, PROT_READ, MAP_PRIVATE | MAP_LOCKED, fd, 0);
    for (loop = 0; loop < 10; loop++)
    {
        printf("[%-10s----%lx]\n", vadr + 4096 * loop, vadr + 4096 * loop);
    }
    while (1)
    {
        sleep(1);
    }
}