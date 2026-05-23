#include <iostream>
#include "sync.hpp"

void init() {
    initscr();
    cbreak();
    // Disable typed char echoing
    noecho();
    // Enable function and arrow keys
    keypad(stdscr, TRUE);
    // Enable cursor visibility
    curs_set(1);
}


int loop(std::string file_path) {
    int status = 0;

    WINDOW* editor_win = newwin(LINES, COLS, 0, 0);
    if (editor_win == nullptr) {
        endwin();
        std::cerr << "Failed to allocate ncurses window." << std::endl;
        return 1;
    }

    brink::Sync sync(file_path, editor_win);
    int ret = sync.load(file_path);
    if (ret > 0) {
        // CALL EXIT FUNC
    }

    while (status == 0) {
        // Main editor loop

        // Check for mode switch / quit
    }

    return 0;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
        return 1;
    }
    std::string file_path = argv[1];

    init();

    loop(file_path);

    // Check if file exists, open it for editing if so

    // else create a new file

    return 0;
}