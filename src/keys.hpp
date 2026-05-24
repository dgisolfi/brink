#pragma once
#include <ncurses.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include "sync.hpp"
#include "utils.hpp"
#ifndef BRINK_KEYS_HPP
#define BRINK_KEYS_HPP

namespace brink {
    int quit(Sync& sync);
    void cur_move(Sync& sync, int direction);
    int handle_key_press(Sync& sync);
};

#endif // BRINK_KEYS_HPP