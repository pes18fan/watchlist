#ifndef wl_colors_h
#define wl_colors_h

/* Some macros to handle terminal colors more easily. Work on both Windows
 * and Linux */
#ifdef _WIN32
#define RED() \
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED)
#define GREEN() \
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN)
#define RESET() \
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | \
            FOREGROUND_GREEN | FOREGROUND_BLUE)
/* Below assumed to be for Linux or any Unix-like. */
#else
#define RED() printf("\033[1;31m")
#define GREEN() printf("\033[1;32m")
#define RESET() printf("\033[0m")
#endif

#endif /* wl_colors_h */
