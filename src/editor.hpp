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
#define LOG_HEIGHT 2
#define MODE_VIEW 0
#define MODE_EDIT 1


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
            Editor(const std::string &fname, WINDOW *e_win, WINDOW *l_win)
                : file_path(fname), editor_win(e_win), log_win(l_win),
                  scroll_offset(0), editor_max_y(0), editor_max_x(0), mode(0) {
                swap_file_path = file_path;
                swap_file_path.append(".swp");
            };

            void quit() {
                brink::del_file(swap_file_path);
                delwin(editor_win);
                delwin(log_win);
                endwin();
                exit(0);
            }
            // I/O Methods
            int load();
            int save() { sync_file(FALSE); log("Saved file: " + file_path); return 0; };
            void sync() { sync_screen(); sync_file(); };
            void sync_screen();
            void sync_file(bool use_swap_file = TRUE);

            // Buffer Methods
            int get_scroll_offset() const { return scroll_offset; }
            void scroll_by(int delta) {
                int max_y, max_x;
                getmaxyx(editor_win, max_y, max_x);
                editor_max_y = max_y;
                scroll_offset += delta;
                scroll_offset = std::max(0, scroll_offset);
                scroll_offset = std::min(scroll_offset, std::max(0, (int)buffer.size() - editor_max_y));
                sync();
            }
            void add_str(int row, int col, const std::string& str);
            void del_str(int row, int col, int len = 1);
            int row_count() const { return buffer.size(); };
            int row_len(int row) const { return (row < (int)buffer.size()) ? buffer[row].size() : 0; };
            void row_delete(int row, int col) { 
                std::string post_cursor = "";
                post_cursor = buffer[row].substr(col, buffer[row].size());
                buffer.erase(buffer.begin() + row);
                add_str(row - 1, row_len(row - 1), post_cursor);
                sync(); 
            };
            void row_create(int row, int col) {
                std::string post_cursor = "";
                post_cursor = buffer[row].substr(col, buffer[row].size());
                del_str(row, col, post_cursor.size());
                buffer.insert((buffer.begin() + row + 1), post_cursor);
                sync();
            };

            // Modes
            int get_mode() { return mode; }
            void set_mode(int m) { mode = m; log("Switched mode to " + get_mode_name()); }
            std::string get_mode_name() {
                std::string m_name;
                switch(mode) {
                    case MODE_EDIT: m_name = "edit"; break;
                    default: m_name = "view"; break;
                }
                return m_name;
            }

            WINDOW *get_win() { return editor_win; };
            std::string get_swap_file_path() { return swap_file_path; };
            void log(std::string msg) {
                std::string log_msg = "\nm=" + get_mode_name();
                log_msg.append(" [brink] => " + msg);
                wprintw(log_win, "%s", log_msg.c_str());
                wrefresh(log_win);
                wrefresh(editor_win);
            }
    };
};

#endif // BRINK_SYNC_HPP