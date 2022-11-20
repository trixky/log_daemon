#ifndef UTILS_HPP

#define UTILS_HPP

#include <iostream>
#include <chrono>
#include <ctime>
#include "locker.hpp"
#include "tintin_reporter.hpp"
#include "server.hpp"

std::string get_time();
void _exit(int status, bool unlock_mode, bool server_mode);

#endif
