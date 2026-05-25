#include "keys.hpp"

namespace brink {
    
    void handle_action(Editor& editor, int action) {
        int x, y, max_y, max_x;
        getyx(editor.get_win(), y, x);  
        getmaxyx(editor.get_win(), max_y, max_x);
        int buf_row = y + editor.get_scroll_offset();

        switch(action) {
            /* Movement */
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
            /* Edit Operations */ 
            case KEY_MAC_DEL:
            case KEY_BACKSPACE:
                if (x > 0) {
                    editor.del_str(buf_row, x - 1);
                    wmove(editor.get_win(), y, x - 1);
                } else if (buf_row > 0) {
                    editor.row_delete(buf_row, 0);
                    handle_action(editor, KEY_UP);
                }
                break;
            case '\r':
            case '\n':
            case KEY_ENTER:
                editor.row_create(buf_row, x);
                wmove(editor.get_win(), y, 0);
                handle_action(editor, KEY_DOWN);
                break;
            case KEY_STAB:
            case KEY_SHFT_TAB:
                for (int i = 0; i < TAB_LENGTH; ++i) {
                    x--;
                    if (x > 0) {       
                        editor.del_str(buf_row, x);
                        handle_action(editor, KEY_LEFT);
                    }
                }
                break;
            case KEY_TAB:
                for (int i = 0; i < TAB_LENGTH; ++i) {
                    editor.add_str(buf_row, x, " ");
                    handle_action(editor, KEY_RIGHT);
                }
                break;
            default: editor.log("Invalid action: " + std::to_string(action));
        }
    }

    void handle_key_press_edit(Editor& editor, int key) {
        // alpha numeric keys for typing only
        if ((32 <= key) && (key <= 126)) {
            int x, y;
            getyx(editor.get_win(), y, x);
            int buf_row = y + editor.get_scroll_offset();
            const std::string& ch = std::string(1, static_cast<char>(key));
            editor.add_str(buf_row, x, ch);
            handle_action(editor, KEY_RIGHT);
        } else {
            switch(key) {
                case 's' & 0x1F: editor.save(); break;
                case 'c' & 0x1F:
                    editor.log("COPY");
                    break;
                case 'z' & 0x1F:
                    editor.log("UNDO");
                    break;
                // case 'm' & 0x1F: editor.set_mode(MODE_VIEW); break;
                // case KEY_ESC: editor.quit(); break;
                case KEY_ESC: editor.set_mode(MODE_VIEW); break;
                default: handle_action(editor, key); break;
            }
        }
    }

    void handle_key_press_view(Editor& editor, int key) {
        switch(key) {
            case KEY_LEFT:
            case KEY_RIGHT:
            case KEY_UP:
            case KEY_DOWN: handle_action(editor, key); break;
            case KEY_ESC: editor.set_mode(MODE_EDIT); break;
            case 'q': editor.quit(); break;
            // default: break;
        }
    }

    int handle_key_press(Editor& editor) {
        int key = wgetch(editor.get_win());

        switch (editor.get_mode()) {
            case MODE_EDIT: handle_key_press_edit(editor, key); break;
            case MODE_VIEW:
            default: handle_key_press_view(editor, key); break;
        }
        
        return 0;
    }
}
