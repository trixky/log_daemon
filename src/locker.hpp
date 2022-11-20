#ifndef LOCKER_HPP

#define LOCKER_HPP

#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <unistd.h>

#define LOCK_FILE "/var/lock/matt-daemon.lock"

void lock(void);
void unlock(void);

#endif
