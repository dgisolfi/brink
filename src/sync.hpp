#pragma once
#include <ncurses.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#ifndef BRINK_SYNC_HPP
#define BRINK_SYNC_HPP

namespace brink {
    class Sync {
        private:
            std::vector<std::string> buffer;
            std::string file_path;
            WINDOW* win;
        
        public:
            Sync(const std::string& fname, WINDOW* window) : file_path(fname), win(window) {
                // add blank like so buffer is not empty
                buffer.push_back("");
                // TODO setup swp file to save the temp buffer until user saves the file
            };
            int load(std::string file_path);
            void screen();
            void file();
            void add_str(int row, int col, const std::string& str);
            void del_str(int row, int col);
            WINDOW *get_win();
            int row_count() const { return buffer.size(); }
            int row_len(int row) const { return (row < (int)buffer.size()) ? buffer[row].size() : 0; }
    };
};

#endif // BRINK_SYNC_HPP