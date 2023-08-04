#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>
#endif

#include "term.h"

void clrscr() {
#ifdef _WIN32
    HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord_screen = {0, 0};
    DWORD c_chars_written;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dw_con_size;

    // get number of character cells in the current buffer
    if (!GetConsoleScreenBufferInfo(h_console, &csbi)) return;
    dw_con_size = csbi.dwSize.X * csbi.dwSize.Y;

    // fill the entire buffer with spaces
    if (!FillConsoleOutputCharacter(h_console, (TCHAR)' ', dw_con_size,
                coord_screen, &c_chars_written)) {
        return;
    } 

    // reset the attributes of every character cell
    if (!FillConsoleOutputAttribute(h_console, csbi.wAttributes, dw_con_size,
                coord_screen, &c_chars_written)) {
        return;
    }

    // move the cursor back to the top left corner
    SetConsoleCursorPosition(h_console, coord_screen);
#else
    printf("\033[2J\033[H");
#endif /* _WIN32 */
}

int __kbhit() {
#ifdef _WIN32
    return _kbhit();
#else
#endif
}

int __getch() {
#ifdef _WIN32
    return _getch();
#else
#endif
}

void center_text(const char* text) {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
#else
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
#endif

#ifdef _WIN32
    int console_width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
#else
    int console_width = w.ws_col
#endif

    int text_length = strlen(text);
    int padding = (console_width - text_length) / 2;

    // print spaces to center the text
    for (int i = 0; i < padding; i++) {
        putchar(' ');
    }

    // print the text
    printf("%s", text);
}
