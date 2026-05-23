#include "sync.hpp"

namespace brink {
    int Sync::load() {
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

        Sync::screen();
        Sync::file();
        return 0;
    }

    int Sync::save() {
        Sync::file(FALSE);
        // TODO: Delete swap file
        brink::del_file(swap_file_path);
        return 0;
    }

    void Sync::screen(){
        int y, x;
        getyx(win, y, x);
        werase(win);
        for (size_t i = 0; i < buffer.size(); ++i) {
            mvwaddnstr(win, i, 0, buffer[i].c_str(), buffer[i].length());
        }
        wmove(win, y, x);
        wrefresh(win);
    }

    void Sync::file(bool use_swap_file) {
        std::string path = swap_file_path;
        if (!use_swap_file) path = file_path;
        std::ofstream out_file(path, std::ios::trunc);
        if (out_file.is_open()) {
            for (const auto& line: buffer) {
                out_file << line << "\n";
            }
        }
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

    void Sync::new_line() {
        buffer.push_back("\n");
        Sync::screen();
        Sync::file();
    }

    WINDOW *Sync::get_win() {
        return win;
    }
};