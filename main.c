#include <stdio.h>
#include "rogueutil.h"

void print_header(void) {
    colorPrint(LIGHTCYAN, BLACK, "WATCHLIST\n");
    colorPrint(LIGHTCYAN, BLACK, "Manage your watchlist\n");
    printf("===============================================\n");
}

int main(void) {
    cls();
    print_header();
    getch();
}
