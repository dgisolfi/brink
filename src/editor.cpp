#include "editor.hpp"

namespace brink {

    Editor::Editor(const std::string &fname, WINDOW *e_win, WINDOW *l_win)
        : file_path(fname), editor_win(e_win), log_win(l_win),
          scroll_offset(0), editor_max_y(0), editor_max_x(0), mode(0) {
        swap_file_path = file_path;
        swap_file_path.append(".swp");
    }

    void Editor::quit() {
        brink::del_file(swap_file_path);
        delwin(editor_win);
        delwin(log_win);
        endwin();
        exit(0);
    }

    // I/O

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

    int Editor::save() {
        sync_file(FALSE);
        log("Saved file: " + file_path);
        return 0;
    }

    void Editor::sync() {
        sync_screen();
        sync_file();
    }

    void Editor::sync_screen() {
        int y, x, editor_max_x;
        getyx(editor_win, y, x);
        getmaxyx(editor_win, editor_max_y, editor_max_x);
        werase(editor_win);
        for (int i = 0; i < editor_max_y; ++i) {
            int buf_idx = i + scroll_offset;
            if (buf_idx < 0 || buf_idx >= (int)buffer.size()) break;
            mvwaddnstr(editor_win, i, 0, buffer[buf_idx].c_str(), buffer[buf_idx].length());
        }
        wmove(editor_win, y, x);
        wrefresh(editor_win);
    }

    void Editor::sync_file(bool use_swap_file) {
        std::string path = swap_file_path;
        if (!use_swap_file) path = file_path;
        std::ofstream out_file(path, std::ios::trunc);
        if (out_file.is_open()) {
            for (const auto& line : buffer) {
                out_file << line << "\n";
            }
        }
    }

    // Buffer

    int Editor::get_scroll_offset() const { return scroll_offset; }

    void Editor::scroll_by(int delta) {
        int max_y, max_x;
        getmaxyx(editor_win, max_y, max_x);
        editor_max_y = max_y;
        scroll_offset += delta;
        scroll_offset = std::max(0, scroll_offset);
        scroll_offset = std::min(scroll_offset, std::max(0, (int)buffer.size() - editor_max_y));
        sync();
    }

    void Editor::add_str(int row, int col, const std::string& str) {
        if (row >= (int)buffer.size()) {
            buffer.resize(row + 1, "");
        }
        buffer[row].insert(col, str);
        Editor::sync();
    }

    void Editor::del_str(int row, int col, int len) {
        if (!buffer[row].empty()) {
            buffer[row].erase(col, len);
        }
        Editor::sync();
    }

    int Editor::row_count() const { return buffer.size(); }

    int Editor::row_len(int row) const {
        return (row < (int)buffer.size()) ? buffer[row].size() : 0;
    }

    void Editor::row_delete(int row, int col) {
        std::string post_cursor = buffer[row].substr(col, buffer[row].size());
        buffer.erase(buffer.begin() + row);
        add_str(row - 1, row_len(row - 1), post_cursor);
        sync();
    }

    void Editor::row_create(int row, int col) {
        std::string post_cursor = buffer[row].substr(col, buffer[row].size());
        del_str(row, col, post_cursor.size());
        buffer.insert((buffer.begin() + row + 1), post_cursor);
        sync();
    }

    // Cursor accessors

    int Editor::cur_x() { int y, x; getyx(editor_win, y, x); return x; }
    int Editor::cur_y() { int y, x; getyx(editor_win, y, x); return y; }
    int Editor::cur_buf_row() { return cur_y() + scroll_offset; }
    int Editor::win_max_y() { int my, mx; getmaxyx(editor_win, my, mx); return my; }

    // Move

    void Editor::move_up() {
        int y = cur_y(), x = cur_x(), buf_row = cur_buf_row();
        if (buf_row <= 0) return;
        int nx = std::min(x, row_len(buf_row - 1));
        if (y > 0) {
            wmove(editor_win, y - 1, nx);
        } else {
            scroll_by(-1);
            wmove(editor_win, 0, nx);
        }
    }

    void Editor::move_down() {
        int y = cur_y(), x = cur_x(), buf_row = cur_buf_row();
        if (buf_row >= row_count() - 1) return;
        int nx = std::min(x, row_len(buf_row + 1));
        if (y < win_max_y() - 1) {
            wmove(editor_win, y + 1, nx);
        } else {
            scroll_by(1);
            wmove(editor_win, y, nx);
        }
    }

    void Editor::move_left() {
        int y = cur_y(), x = cur_x();
        if (x > 0) wmove(editor_win, y, x - 1);
    }

    void Editor::move_right() {
        if (cur_x() < row_len(cur_buf_row())) wmove(editor_win, cur_y(), cur_x() + 1);
    }

    // Edit

    void Editor::type(int key) {
        const std::string ch = std::string(1, static_cast<char>(key));
        add_str(cur_buf_row(), cur_x(), ch);
        move_right();
    }

    void Editor::backspace() {
        int y = cur_y(), x = cur_x(), buf_row = cur_buf_row();
        if (x > 0) {
            del_str(buf_row, x - 1);
            wmove(editor_win, y, x - 1);
        } else if (buf_row > 0) {
            row_delete(buf_row, 0);
            move_up();
        }
    }

    void Editor::enter() {
        int y = cur_y(), x = cur_x(), buf_row = cur_buf_row();
        row_create(buf_row, x);
        wmove(editor_win, y, 0);
        move_down();
    }

    void Editor::stab() {
        for (int i = 0; i < TAB_LENGTH; ++i) {
            int x = cur_x() - 1;
            if (x > 0) {
                del_str(cur_buf_row(), x);
                move_left();
            }
        }
    }

    void Editor::tab() {
        for (int i = 0; i < TAB_LENGTH; ++i) {
            add_str(cur_buf_row(), cur_x(), " ");
            move_right();
        }
    }

    // Modes

    int Editor::get_mode() { return mode; }

    void Editor::set_mode(int m) {
        mode = m;
        log("Switched mode to " + get_mode_name());
    }

    std::string Editor::get_mode_name() {
        switch (mode) {
            case MODE_EDIT: return "edit";
            default:        return "view";
        }
    }

    WINDOW *Editor::get_win() { return editor_win; }

    std::string Editor::get_swap_file_path() { return swap_file_path; }

    void Editor::prompt() {
        werase(log_win);
        wprintw(log_win, "%s", " [brink] m=");
        wattron(log_win, COLOR_PAIR(10));
        wprintw(log_win, "%s", get_mode_name().c_str());
        wattroff(log_win, COLOR_PAIR(10));
        wprintw(log_win, "%s", " ~> ");
    }

    void Editor::log(std::string msg) {
        prompt();
        wprintw(log_win, "%s", msg.c_str());
        wrefresh(log_win);
    }

    void Editor::search() {
        // TODO
    }

};
