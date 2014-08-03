#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

__attribute ((noreturn)) void fail(const char* msg)
{
    printf("error: %s\n", msg);
    exit(EXIT_FAILURE);
}

void from_fd(int fd)
{
    char buf[128];
    ssize_t len;

    len = read(fd, (void*) &buf, 128);
    if (len <= 0)
        fail("read");

    for (int i = 0; i < len; i++)
    {
        printf("%.2hhx ", buf[i]);
    }
    puts("");
}

void to_fd(int fd)
{
    char byte;

    if (scanf("%hhx", &byte) != 1)
        fail("scanf");

    if (write(fd, (const void*) &byte, 1) != 1)
        fail("write");
}


int main(const int argc, const char* argv[])
{
    int fd, nfds;
    fd_set set;

    if (argc != 2)
        fail("arg");

    fd = open(argv[1], O_RDWR | O_NOCTTY);
    if (fd < 0)
        fail("open");

    nfds = fd + 1;

    for (;;)
    {
        FD_ZERO(&set);
        FD_SET(STDIN_FILENO, &set);
        FD_SET(fd, &set);

        if (select(nfds, &set, NULL, NULL, NULL) < 0)
            fail("select");

        if(FD_ISSET(fd, &set))
        {
            from_fd(fd);
        }

        if (FD_ISSET(STDIN_FILENO, &set))
            to_fd(fd);
    }

    exit(EXIT_SUCCESS);
}
