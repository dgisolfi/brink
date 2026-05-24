#include "keys.hpp"

namespace brink {
    int quit(Editor& editor) {
        brink::del_file(editor.get_swap_file_path());
        delwin(editor.get_win());
        endwin();
        exit(0);
    }

    void handle_edit_action(Editor& editor, int action) {
        int x, y, max_y, max_x;
        getyx(editor.get_win(), y, x);
        getmaxyx(editor.get_win(), max_y, max_x);
        int buf_row = y + editor.get_scroll_offset();

        switch(action) {
            case KEY_UP: {
                if (buf_row <= 0) break;
                int nx = std::min(x, editor.row_len(buf_row - 1));
                if (y > 0) {
                    wmove(editor.get_win(), y - 1, nx);
                } else {
                    editor.scroll_by(-1);
                    wmove(editor.get_win(), 0, nx);
                }
                break;
            }
            case KEY_DOWN: {
                if (buf_row >= editor.row_count() - 1) break;
                int nx = std::min(x, editor.row_len(buf_row + 1));
                if (y < max_y - 1) {
                    wmove(editor.get_win(), y + 1, nx);
                } else {
                    editor.scroll_by(1);
                    wmove(editor.get_win(), y, nx);
                }
                break;
            }
            case KEY_LEFT:
                if (x > 0) wmove(editor.get_win(), y, x - 1);
                break;
            case KEY_RIGHT:
                if (x < editor.row_len(buf_row)) wmove(editor.get_win(), y, x + 1);
                break;
            case KEY_BACKSPACE:
                if (x > 0) {
                    editor.del_str(buf_row, x - 1);
                    wmove(editor.get_win(), y, x - 1);
                } else if (buf_row > 0) {
                    editor.row_delete(buf_row, 0);
                    handle_edit_action(editor, KEY_UP);
                }
                break;
            case '\r':
            case '\n':
            case KEY_ENTER:
                editor.row_create(buf_row, x);
                wmove(editor.get_win(), y, 0);
                handle_edit_action(editor, KEY_DOWN);
                break;
            case 353:
                editor.log("STAB");
                for (int i = 1; i < TAB_LENGTH; ++i) {
                    editor.del_str(buf_row, x);
                    handle_edit_action(editor, KEY_LEFT);
                }
                break;
            case KEY_TAB:
                for (int i = 1; i < TAB_LENGTH; ++i) {
                    editor.add_str(buf_row, x, " ");
                    handle_edit_action(editor, KEY_RIGHT);
                }
                break;
            default: editor.log("Invalid action: " + std::to_string(action));
        }
    }

    int handle_key_press(Editor& editor) {
        int key = wgetch(editor.get_win());
        if ((32 <= key) && (key <= 126)) {
            int x, y;
            getyx(editor.get_win(), y, x);
            int buf_row = y + editor.get_scroll_offset();
            const std::string& ch = std::string(1, static_cast<char>(key));
            editor.add_str(buf_row, x, ch);
            handle_edit_action(editor, KEY_RIGHT);
        } else {
            switch(key) {
                case 's' & 0x1F: editor.save(); break;
                case 'c' & 0x1F:
                case KEY_ESC: quit(editor); break;
                default: handle_edit_action(editor, key); break;
            }
        }
        return 0;
    }
}
