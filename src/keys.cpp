#include "keys.hpp"

namespace brink {
    int quit() {
        endwin();
        exit(0);
    }

    void cur_move(Sync sync, int direction) {
        int x, y;
        getyx(stdscr, y, x);
        // switch(direction) {
        //     case KEY_UP:    break;
        // }
    }

    int handle_key_press(Sync sync) {
        int key = getch();
        switch(key) {
            case 27: quit(); break;
            // case KEY_UP:
            // case KEY_DOWN:
            // case KEY_LEFT:
            // case KEY_RIGHT: 
            // case KEY_BACKSPACE: cur_move(sync, key); break;
        }

        if ((key < 32) && (key > 126)) {
            std::cerr << "Unsupported key: " << key << std::endl;
            return -1;
        } else {
            int x, y;
            getyx(stdscr, y, x);
            const std::string& ch = std::string(1, static_cast<char>(key));
            sync.add_str(x+1, y, ch);
        }
    }
}