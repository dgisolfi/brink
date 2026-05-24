#include "keys.hpp"

namespace brink {
    int quit(Editor& editor) {
        brink::del_file(editor.get_swap_file_path());
        delwin(editor.get_win());
        endwin();
        exit(0);
    }

    void cur_move(Editor& editor, int direction) {
        int x, y;
        getyx(editor.get_win(), y, x);
        switch(direction) {
            case KEY_UP:
                if (y > 0) {
                    int nx = std::min(x, editor.row_len(y - 1));
                    wmove(editor.get_win(), y - 1, nx);
                }
                break;
            case KEY_DOWN:
                if (y < editor.row_count() - 1) {
                    int nx = std::min(x, editor.row_len(y + 1));
                    wmove(editor.get_win(), y + 1, nx);
                }
                break;
            case KEY_LEFT:
                if (x > 0) wmove(editor.get_win(), y, x - 1);
                break;
            case KEY_RIGHT:
                if (x < editor.row_len(y)) wmove(editor.get_win(), y, x + 1);
                break;
            case KEY_BACKSPACE:
                if (x > 0) {
                    editor.del_str(y, x - 1);
                    wmove(editor.get_win(), y, x - 1);
                }
                if ((x == 0) && (editor.row_len(y) == 0)) {
                    editor.row_delete(y);
                    cur_move(editor, KEY_UP);
                }
                break;
            case KEY_ENTER:
                editor.row_new(y);
                cur_move(editor, KEY_DOWN);
                break;
            // KEY_TAB
            case 9:
                editor.add_str(y, x, "\t");
                for (int i = 0; i < 3; ++i) { 
                    cur_move(editor, KEY_RIGHT);
                }; 
                break;
            default: std::cerr << "Invalid direction: %d" << direction;
        }
    }

    int handle_key_press(Editor& editor) {
        int key = wgetch(editor.get_win());
        switch(key) {
            case 9: 
            case KEY_UP:
            case KEY_DOWN:
            case KEY_LEFT:
            case KEY_RIGHT: 
            case KEY_BACKSPACE: cur_move(editor, key); break;
            case '\r':
            case '\n':
            case KEY_ENTER:  cur_move(editor, KEY_ENTER); break;
            case 's' & 0x1F: editor.save(); break;
            case 'c' & 0x1F:
            case 27: quit(editor); break;
            default: break;
        }

        // For normal alpha numeric keys just print to screend and advance cursor
        if ((32 <= key) && (key <= 126)) {
            int x, y;
            getyx(editor.get_win(), y, x);
            const std::string& ch = std::string(1, static_cast<char>(key));
            editor.add_str(y, x, ch);
            cur_move(editor, KEY_RIGHT);
        }

        return 0;
    }
}