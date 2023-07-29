#include <stdio.h>
#include <conio.h>
#include <windows.h>

#define bool _Bool
#define true 1
#define false 0

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

typedef struct {
    const char* name;
    bool watched;
} Movie;

Movie mv_new(const char* name) {
    Movie movie;
    movie.name = name;
    movie.watched = false;
    return movie;
}

void mv_toggle_watch(Movie* movie) {
    if (movie->watched == false) {
        movie->watched = true;
    } else {
        movie->watched = false;
    }
}

void mv_print(Movie movie) {
    printf("Movie name: %s\n", movie.name);
    printf("Watched: %s\n", movie.watched ? "yez" : "nope");
}

typedef struct {
    const char* name;
    int episodes;
    bool watched;
} Series;

Series sr_new(const char* name, int episodes) {
    Series series;
    series.name = name;
    series.episodes = episodes;
    series.watched = false;
    return series;
}

void sr_toggle_watch(Series* series) {
    if (series->watched == true) {
        series->watched = false;
    } else {
        series->watched = true;
    }
}

void sr_print(Series series) {
    printf("Series name: %s\n", series.name);
    printf("Episodes: %d\n", series.episodes);
    printf("Watched: %s\n", series.watched ? "yez" : "nope");
}

int main(void) {
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

    Movie opp = mv_new("Oppenheimer");

    col_green(console);
    mv_print(opp); 
    col_reset(console);

    Series sg = sr_new("Steins;Gate", 24);
    sr_toggle_watch(&sg);

    col_blue(console);
    sr_print(sg);
    col_reset(console);

    _getch();

    return 0;
}
