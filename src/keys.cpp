#include "keys.hpp"

namespace brink {
    int quit(Sync& sync) {
        brink::del_file(sync.get_swap_file_path());
        delwin(sync.get_win());
        endwin();
        exit(0);
    }

    void cur_move(Sync& sync, int direction) {
        int x, y;
        getyx(sync.get_win(), y, x);
        switch(direction) {
            case KEY_UP:
                if (y > 0) {
                    int nx = std::min(x, sync.row_len(y - 1));
                    wmove(sync.get_win(), y - 1, nx);
                }
                break;
            case KEY_DOWN:
                if (y < sync.row_count() - 1) {
                    int nx = std::min(x, sync.row_len(y + 1));
                    wmove(sync.get_win(), y + 1, nx);
                }
                break;
            case KEY_LEFT:
                if (x > 0) wmove(sync.get_win(), y, x - 1);
                break;
            case KEY_RIGHT:
                if (x < sync.row_len(y)) wmove(sync.get_win(), y, x + 1);
                break;
            case KEY_BACKSPACE:
                if (x > 0) {
                    sync.del_str(y, x - 1);
                    wmove(sync.get_win(), y, x - 1);
                }
                break;
            default: std::cerr << "Invalid direction: %d" << direction;
        }
    }

    int handle_key_press(Sync& sync) {
        int key = wgetch(sync.get_win());
        switch(key) {
            case KEY_UP:
            case KEY_DOWN:
            case KEY_LEFT:
            case KEY_RIGHT: 
            case KEY_BACKSPACE: cur_move(sync, key); break;
            case '\r':
            case '\n':
            case KEY_ENTER: 
                sync.new_line();
                cur_move(sync, KEY_DOWN);
                break;
            case 's' & 0x1F: sync.save(); break;
            case 'c' & 0x1F:
            case 27: quit(sync); break;
            case 9: 
                for (int i = 0; i < 3; ++i) { 
                    cur_move(sync, KEY_RIGHT);
                    sync.add_str()
                }; 
                break;
            default: break;
        }

        // For normal alpha numeric keys just print to screend and advance cursor
        if ((32 <= key) && (key <= 126)) {
            int x, y;
            getyx(sync.get_win(), y, x);
            const std::string& ch = std::string(1, static_cast<char>(key));
            sync.add_str(y, x, ch);
            cur_move(sync, KEY_RIGHT);
        }

        return 0;
    }
}