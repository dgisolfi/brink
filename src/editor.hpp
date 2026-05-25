#pragma once
#include <ncurses.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "utils.hpp"
#ifndef BRINK_SYNC_HPP
#define BRINK_SYNC_HPP
#define LOG_HEIGHT 1
#define MODE_VIEW  0
#define MODE_EDIT  1
#define TAB_LENGTH 4

namespace brink {
    class Editor {
        private:
            int mode;
            int scroll_offset;
            int editor_max_y;
            int editor_max_x;
            std::string file_path;
            std::string swap_file_path;
            std::vector<std::string> buffer;
            WINDOW* editor_win;
            WINDOW* log_win;

        public:
            Editor(const std::string &fname, WINDOW *e_win, WINDOW *l_win);

            void quit();

            // I/O
            int load();
            int save();
            void sync();
            void sync_screen();
            void sync_file(bool use_swap_file = TRUE);

            // Buffer
            int get_scroll_offset() const;
            void scroll_by(int delta);
            void add_str(int row, int col, const std::string& str);
            void del_str(int row, int col, int len = 1);
            int row_count() const;
            int row_len(int row) const;
            void row_delete(int row, int col);
            void row_create(int row, int col);

            // Cursor accessors
            int cur_x();
            int cur_y();
            int cur_buf_row();
            int win_max_y();

            // Move
            void move_up();
            void move_down();
            void move_left();
            void move_right();

            // Edit
            void type(int key);
            void backspace();
            void enter();
            void stab();
            void tab();

            // Modes
            int get_mode();
            void set_mode(int m);
            std::string get_mode_name();

            WINDOW *get_win();
            std::string get_swap_file_path();

            void prompt();
            void log(std::string msg);
            void search();
    };
};

#endif // BRINK_SYNC_HPP
