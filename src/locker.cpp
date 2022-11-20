#include "./locker.hpp"

int g_fd;

void lock(void)
{
    g_fd = open(LOCK_FILE, O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (g_fd < 0)
    {
        fprintf(stderr, "Lock file opening failed.\n");
        exit(EXIT_FAILURE);
    }

    if (flock(g_fd, LOCK_EX | LOCK_NB) != 0)
    {
        fprintf(stderr, "Lock file locking failed.\n");
        close(g_fd);
        exit(EXIT_FAILURE);
    }
}

void unlock(void)
{
    printf("on exit par le unlock enfait");
    unlink(LOCK_FILE);
    flock(g_fd, LOCK_UN);
    close(g_fd);
}