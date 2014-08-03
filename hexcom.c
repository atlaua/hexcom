/*
 * Copyright 2014 Luis Ressel
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
