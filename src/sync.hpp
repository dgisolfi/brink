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
            std::string filepath;
            WINDOW* win;
        
        public:
            Sync(const std::string& fname, WINDOW* window) : filepath(fname), win(window) {
                // add blank like so buffer is not empty
                buffer.push_back("");
                // TODO setup swp file to save the temp buffer until user saves the file
            };
            int load(std::string file_path);
            void screen();
            void file();
            void add_str(int row, int col, const std::string& str);
            void del_str(int row, int col);
    };
};

#endif // BRINK_SYNC_HPP