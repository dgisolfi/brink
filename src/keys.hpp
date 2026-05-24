#pragma once
#include <ncurses.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include "editor.hpp"
#include "utils.hpp"
#ifndef BRINK_KEYS_HPP
#define BRINK_KEYS_HPP
#define KEY_TAB      9
#define KEY_ESC      27
#define KEY_SHFT_TAB 353
#define TAB_LENGTH   4

namespace brink {
    void cur_move(Editor& editor, int direction);
    int handle_key_press(Editor& editor);
};

#endif // BRINK_KEYS_HPP