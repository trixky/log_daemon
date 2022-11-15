#include "./locker.hpp"

int g_fd;

void lock(void)
{
    g_fd = open(LOCK_FILE, O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (g_fd < 0)
    {
        perror("Lock file opening failed.\n" LOCK_FILE);
        exit(EXIT_FAILURE);
    }

    if (flock(g_fd, LOCK_EX | LOCK_NB))
    {
        perror("Lock file locking failed.\n" LOCK_FILE);
        close(g_fd);
        exit(EXIT_FAILURE);
    }
}

void unlock(void)
{
    unlink(LOCK_FILE);
    flock(g_fd, LOCK_UN);
    close(g_fd);
}