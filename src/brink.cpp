#include <iostream>
#include <ncurses.h>
#include "editor.hpp"
#include "keys.hpp"

brink::Editor* init(const std::string& file_path) {
    setlocale(LC_ALL, "");
    initscr();
    set_escdelay(25);
    // Refresh stdscr so newwin shows up on top
    refresh();
    // cbreak();
    // Allow for ctr chars to be captured
    raw();
    // Disable typed char echoing
    noecho();
    // Enable cursor visibility
    curs_set(1);

    int log_height = LOG_HEIGHT;
    int editor_height = LINES - log_height;

    WINDOW* editor_win = newwin(editor_height, COLS, 0, 0);
    WINDOW* log_win    = newwin(log_height, COLS, editor_height, 0);
    if ((editor_win == nullptr) || (log_win == nullptr)) {
        endwin();
        std::cerr << "Failed to allocate ncurses window." << std::endl;
        return nullptr;
    }
    keypad(editor_win, TRUE);
    // autoscroll for logs
    scrollok(editor_win, TRUE);
    scrollok(log_win, TRUE);

    if (has_colors()) {
        start_color();
        use_default_colors();
        init_pair(10, COLOR_RED, COLOR_WHITE);
        init_pair(1, COLOR_BLACK, COLOR_WHITE);
        wbkgd(log_win, COLOR_PAIR(1));
    }
  
    return new brink::Editor(file_path, editor_win, log_win);
}


int loop(const std::string& file_path, brink::Editor& editor) {
    int status = 0;

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
        std::cerr << "Brink Editor - Usage: " << argv[0] << " <file_path>" << std::endl;
        return 1;
    }
    std::string file_path = argv[1];
    brink::Editor *e = init(file_path);
    if (e == nullptr) return 1;
    loop(file_path, *e);
    delete e;
    return 0;
}