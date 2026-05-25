#include "keys.hpp"

namespace brink {
    
    void handle_action(Editor& editor, int action) {
        switch(action) {
            case KEY_UP:    editor.move_up();    break;
            case KEY_DOWN:  editor.move_down();  break;
            case KEY_LEFT:  editor.move_left();  break;
            case KEY_RIGHT: editor.move_right(); break;
            case KEY_MAC_DEL:
            case KEY_BACKSPACE:  editor.backspace(); break;
            case '\r':
            case '\n':
            case KEY_ENTER: editor.enter(); break;
            case KEY_STAB:
            case KEY_SHFT_TAB: editor.stab(); break;
            case KEY_TAB:      editor.tab();  break;
            default: editor.log("Invalid action: " + std::to_string(action));
        }
    }

    void handle_key_press_edit(Editor& editor, int key) {
        if ((32 <= key) && (key <= 126)) {
            editor.type(key);
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
            case 'f': editor.search(); break;
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
