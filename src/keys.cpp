#include "keys.hpp"

namespace brink {
    int quit(Editor& editor) {
        brink::del_file(editor.get_swap_file_path());
        delwin(editor.get_win());
        endwin();
        exit(0);
    }

    void handle_edit_action(Editor& editor, int direction) {
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
                if (x == 0) {
                    editor.row_delete(y, x);
                    handle_edit_action(editor, KEY_UP);
                    wmove(editor.get_win(), y - 1, editor.row_len(y - 1) - x);
                    editor.sync();
                } 
                break;
            case '\r':
            case '\n':
            case KEY_ENTER:
                editor.row_create(y, x);
                wmove(editor.get_win(), y, 0);
                handle_edit_action(editor, KEY_DOWN);
                break;
            case KEY_STAB:
                editor.log("STAB");
                for (int i = 1; i < TAB_LENGTH; ++i) {
                    editor.del_str(y, x);
                    handle_edit_action(editor, KEY_RIGHT);
                }
            case KEY_TAB:
                // init to 1 so tab length is easier to configure 
                for (int i = 1; i < TAB_LENGTH; ++i) { 
                    editor.add_str(y, x, " ");
                    handle_edit_action(editor, KEY_RIGHT);
                }; 
                break;
            default: std::cerr << "Invalid direction: %d" << direction;
        }
    }

    int handle_key_press(Editor& editor) {
        int key = wgetch(editor.get_win());
        // For normal alpha numeric keys just print to screend and advance cursor
        if ((32 <= key) && (key <= 126)) {
            int x, y;
            getyx(editor.get_win(), y, x);
            const std::string& ch = std::string(1, static_cast<char>(key));
            editor.add_str(y, x, ch);
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