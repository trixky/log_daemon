#ifndef LOCKER_HPP

#define LOCKER_HPP

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/file.h>
#include <sys/types.h>
#include <unistd.h>

#define LOCK_FILE "/var/lock/matt-daemon.lock"

void lock(void);
void unlock(void);

#endif
