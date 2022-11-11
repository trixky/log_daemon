#include <iostream>
#include "tintin_reporter.hpp"

int main()
{
    Tintin_reporter reporter = Tintin_reporter();

    reporter.log((const char *)"test");

    return 0;
}