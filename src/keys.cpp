#include "keys.hpp"

namespace brink {
    int quit(Sync& sync) {
        delwin(sync.get_win());
        endwin();
        exit(0);
    }

    void cur_move(Sync& sync, int direction) {
        int x, y;
        getyx(sync.get_win(), y, x);
        switch(direction) {
            case KEY_UP:        wmove(sync.get_win(), y-1, x); break;
            case KEY_DOWN:      wmove(sync.get_win(), y+1, x); break;
            case KEY_LEFT:      wmove(sync.get_win(), y, x-1); break;
            case KEY_RIGHT:     wmove(sync.get_win(), y, x+1); break;
            case KEY_BACKSPACE: 
                sync.del_str(y, x);
                cur_move(sync, KEY_LEFT);
                break;
            default: std::cerr << "Invalid direction: %d" << direction;
        }
        wrefresh(sync.get_win());
    }

    int handle_key_press(Sync& sync) {
        int key = wgetch(sync.get_win());
        switch(key) {
            case 27: quit(sync); break;
            case KEY_UP:
            case KEY_DOWN:
            case KEY_LEFT:
            case KEY_RIGHT: 
            case KEY_BACKSPACE: cur_move(sync, key); break;
            default: break;
        }

        if ((key < 32) && (key > 126)) {
            std::cerr << "Unsupported key: " << key << std::endl;
            return -1;
        } else {
            int x, y;
            getyx(sync.get_win(), y, x);
            const std::string& ch = std::string(1, static_cast<char>(key));
            sync.add_str(y, x, ch);
            cur_move(sync, KEY_RIGHT);
        }

        return 0;
    }
}