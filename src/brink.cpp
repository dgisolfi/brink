#include <iostream>
#include <ncurses.h>
#include "editor.hpp"
#include "keys.hpp"

void init() {
    setlocale(LC_ALL, "");
    initscr();
    // Refresh stdscr so newwin shows up on top
    refresh();
    // cbreak();
    // Allow for ctr chars to be captured
    raw();
    // Disable typed char echoing
    noecho();
    // Enable function and arrow keys
    // keypad(stdscr, TRUE);
    // Enable cursor visibility
    curs_set(1);
}


int loop(std::string file_path) {
    int status = 0;
    int log_height = 2;
    int editor_height = LINES - log_height;

    WINDOW* editor_win = newwin(editor_height, COLS, 0, 0);
    WINDOW* log_win    = newwin(log_height, COLS, editor_height, 0);
    if ((editor_win == nullptr) || (log_win == nullptr)) {
        endwin();
        std::cerr << "Failed to allocate ncurses window." << std::endl;
        return 1;
    }
    keypad(editor_win, TRUE);
    // autoscroll for logs
    scrollok(log_win, TRUE);

    start_color();
    use_default_colors();    
    init_pair(1, COLOR_BLACK, COLOR_BLUE);
    wbkgd(log_win,    ' ' | COLOR_PAIR(1));

    
    brink::Editor editor(file_path, editor_win, log_win);
    
    int ret = editor.load();
    if (ret > 0) {
        editor.log("File does not exist creating: " + file_path);
    } else {
        editor.log("Loaded: " + file_path);
    }
    

    while (status == 0) {
        brink::handle_key_press(editor);
    }
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Brink Editor\nUsage: " << argv[0] << " <file_path>" << std::endl;
        return 1;
    }
    init();
    loop(argv[1]);
    return 0;
}