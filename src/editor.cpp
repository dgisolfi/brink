#include "editor.hpp"

namespace brink {
    int Editor::load() {
        std::ifstream file(file_path);
        std::string line;

        if (file.is_open()) {
            while (std::getline(file, line)) {
                buffer.push_back(line);
            }
            file.close();
        } else {
            return 1;
        }

        Editor::sync();
        return 0;
    }

    void Editor::sync_screen(){
        int y, x;
        getyx(editor_win, y, x);
        werase(editor_win);
        for (size_t i = 0; i < buffer.size(); ++i) {
            mvwaddnstr(editor_win, i, 0, buffer[i].c_str(), buffer[i].length());
        }
        wmove(editor_win, y, x);
        wrefresh(editor_win);
    }

    void Editor::sync_file(bool use_swap_file) {
        std::string path = swap_file_path;
        if (!use_swap_file) path = file_path;
        std::ofstream out_file(path, std::ios::trunc);
        if (out_file.is_open()) {
            for (const auto& line: buffer) {
                out_file << line << "\n";
            }
        }
    }

    void Editor::add_str(int row, int col, const std::string& str) {
        if (row >= buffer.size()) {
            buffer.resize(row + 1, "");
        }
        buffer[row].insert(col, str);
        
        Editor::sync();
    }

    void Editor::del_str(int row, int col) {
        if (!buffer[row].empty()) {
            buffer[row].erase(col, 1);
        }
        
        Editor::sync();
    }
};