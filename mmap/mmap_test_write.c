#include <stdio.h>  
#include <unistd.h>  
#include <sys/mman.h>  
#include <sys/types.h>  
#include <fcntl.h>  
#include <stdlib.h>  
#include <errno.h>

#define LEN (10*4096)
int main(void)
{
    int fd;
    char *vadr;

    if ((fd = open("/dev/mmaptest", O_RDWR)) < 0)
    {
        printf("open device /dev/mmaptest failed, errno %d\n", errno);
        return 0;
    }
    vadr = mmap(0, LEN, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_LOCKED, fd, 0);

    sprintf(vadr, "write from userspace");

    while (1)
    {
        sleep(1);
    }
    return 0;
}