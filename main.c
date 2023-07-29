#include <stdio.h>
#include <conio.h>
#include <windows.h>

void col_green(HANDLE handle) {
    SetConsoleTextAttribute(handle, FOREGROUND_GREEN);
}

void col_red(HANDLE handle) {
    SetConsoleTextAttribute(handle, FOREGROUND_RED);
}

void col_blue(HANDLE handle) {
    SetConsoleTextAttribute(handle, FOREGROUND_BLUE);
}

void col_reset(HANDLE handle) {
    SetConsoleTextAttribute(handle, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
}

int main(void) {
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

    col_green(console);
    printf("hello, world!\n");
    col_reset(console);

    _getch();

    return 0;
}
