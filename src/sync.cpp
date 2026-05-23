#include "sync.hpp"

namespace brink {
    int Sync::load(std::string file_path) {
        std::ifstream file(file_path);
        std::string line;

        if (file.is_open()) {
            while (std::getline(file, line)) {
                buffer.push_back(line);
            }
            file.close();
        } else {
            std::cerr << "Unable to open file: " << file_path << std::endl;
            return 1;
        }
        return 0;
    }

    void Sync::screen(){
        // clear the window buffer
        werase(win);
        for (size_t i = 0; i < buffer.size(); ++i) {
            // Write content into buffer inside bounds
            mvwaddnstr(win, i, 0, buffer[i].c_str(), buffer[i].length());
        }
        wrefresh(win);
    }

    void Sync::file() {
        // Skip for now to avoid saving files
    }

    void Sync::add_str(int row, int col, const std::string& str) {
        if (row >= buffer.size()) {
            buffer.resize(row + 1, "");
        }
        buffer[row].insert(col, str);

        Sync::screen();
        Sync::file();
    }

    void Sync::del_str(int row, int col) {
        if (!buffer[row].empty()) {
            buffer[row].pop_back();
        }
        
        Sync::screen();
        Sync::file();
    }
};