#pragma once
#include <ncurses.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include "editor.hpp"
#include "utils.hpp"
#ifndef BRINK_KEYS_HPP
#define BRINK_KEYS_HPP

namespace brink {
    int quit(Editor& editor);
    void cur_move(Editor& editor, int direction);
    int handle_key_press(Editor& editor);
};

#endif // BRINK_KEYS_HPP