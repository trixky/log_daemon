#include "./locker.hpp"
#include "tintin_reporter.hpp"

int g_fd;

/* lock locks the lock file */
void lock(void)
{
    // Open or create the lock file if does not exist
    g_fd = open(LOCK_FILE, O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

    if (g_fd < 0)
    {
        // If lock file opening / creation failed
        fprintf(stderr, "Lock file opening failed.\n");
        g_reporter.error("Error file locked.");
        exit(EXIT_FAILURE);
    }

    if (flock(g_fd, LOCK_EX | LOCK_NB) != 0)
    {
        // If lock file locking failed
        fprintf(stderr, "Lock file locking failed.\n");
        close(g_fd);
        g_reporter.error("Error file locked.");
        exit(EXIT_FAILURE);
    }
}

/* unlock unlocks the lock file */
void unlock(void)
{
    // Unlink the lock file
    unlink(LOCK_FILE);
    // Unlock the lock file
    flock(g_fd, LOCK_UN);
    // Close the lock file
    close(g_fd);
}
