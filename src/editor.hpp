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

namespace brink {
    class Editor {
        private:
            std::string file_path;
            std::string swap_file_path;
            std::vector<std::string> buffer;
            WINDOW* editor_win;
            WINDOW* log_win;
        
        public:
            Editor(const std::string &fname, WINDOW *e_win, WINDOW *l_win) : file_path(fname), editor_win(e_win), log_win(l_win) {
                swap_file_path = file_path;
                swap_file_path.append(".swp");
            };

            // I/O Methods
            int load();
            int save() { sync_file(FALSE); log("Saved file: " + file_path); return 0; };
            void sync() { sync_screen(); sync_file(); };
            void sync_screen();
            void sync_file(bool use_swap_file = TRUE);

            // Buffer Methods
            void add_str(int row, int col, const std::string& str);
            void del_str(int row, int col);
            int row_count() const { return buffer.size(); };
            int row_len(int row) const { return (row < (int)buffer.size()) ? buffer[row].size() : 0; };
            void row_delete(int row) { buffer.erase(buffer.begin() + row);  sync(); };
            void row_new(int row) {  buffer.insert((buffer.begin() + row + 1), ""); sync(); };

            WINDOW *get_win() { return editor_win; };
            std::string get_swap_file_path() { return swap_file_path; };
            void log(std::string msg) {
                std::string log_msg = "\n[brink] => " + msg;
                wprintw(log_win, "%s", log_msg.c_str());
                wrefresh(log_win);
            }
    };
};

#endif // BRINK_SYNC_HPP