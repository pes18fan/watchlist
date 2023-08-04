#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "term.h"

#define bool int
#define true 1
#define false 0

/* Include windows headers if compiling on Windows. */
#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#endif

/* Maximum possible string length allowed. */
#define MAX_LEN 100

typedef struct {
    char name[MAX_LEN];
    bool watched;
} Movie;

typedef struct {
    char name[MAX_LEN];
    int episodes;
    bool watched;
} Series;

typedef struct {
    char uname[MAX_LEN];

    /* Both `movies` and `series` are malloc'd and grow their size as needed. */
    Movie* movies;
    int movie_count;
    int movie_cap;

    Series* series;
    int series_count;
    int series_cap;
} Profile;

Movie movie_new() {
    Movie movie;

    printf("Enter name for movie:\n");
    fgets(movie.name, MAX_LEN, stdin);
    /* Removes the newline character that fgets() puts at the end. */
    movie.name[strcspn(movie.name, "\n")] = 0;
    fflush(stdin);

    movie.watched = false;
    return movie;
}

void movie_toggle_watch(Movie* movie) {
    if (movie->watched == false) {
        movie->watched = true;
    } else {
        movie->watched = false;
    }
}

void movie_print(Movie movie) {
    printf("\tMovie name: %s\n", movie.name);
    printf("\tWatched: %s\n", movie.watched ? "yez" : "nope");
}

/* Get a movie from the movie list in a profile. */
Movie* movie_get(Profile* p, const char* name) {
    for (int i = 0; i < p->movie_count; i++) {
        if (strcmp(p->movies[i].name, name) == 0) {
            return &p->movies[i];
        }
    }

    return NULL;
}

/* Add a movie to the movie list in a profile. */
void movie_add(Profile* p, Movie movie) {
    /* This if statement checks if the capacity of the `movies` array is less
     * than the number of elements, and if so it increases the capacity by
     * using the realloc() function. Same thing done with series_add(). */
    if (p->movie_cap <= p->movie_count) {
        /* Get the current capacity of the movie array. */
        int cap = p->movie_cap;

        /* Grow the capacity as needed. */
        p->movie_cap = cap < 8 ? 8 : cap * 2;

        /* Now set the size of `movies` to the new grown capacity. */
        p->movies = realloc(p->movies, p->movie_cap * sizeof(Movie));
        if (p->movies == NULL) {
            perror("Out of memory for movies");
            exit(1);
        }
    }

    p->movies[p->movie_count++] = movie;
}

Series series_new() {
    Series series;

    printf("Enter name for series:\n");
    fgets(series.name, MAX_LEN, stdin);
    series.name[strcspn(series.name, "\n")] = 0;
    fflush(stdin);

    char buf[MAX_LEN];
    int value;
    printf("Enter number of episodes:\n");

retry:
    fgets(buf, MAX_LEN, stdin);
    if (sscanf(buf, "%d", &value) != 1) {
        fprintf(stderr, "Please enter a number!\n");
        goto retry;
    } else {
        series.episodes = value;
    }

    fflush(stdin);
    series.watched = false;
    return series;
}

void series_toggle_watch(Series* series) {
    if (series->watched == true) {
        series->watched = false;
    } else {
        series->watched = true;
    }
}

void series_print(Series series) {
    printf("\tSeries name: %s\n", series.name);
    printf("\tEpisodes: %d\n", series.episodes);
    printf("\tWatched: %s\n", series.watched ? "yez" : "nope");
}

Series* series_get(Profile* p, const char* name) {
    for (int i = 0; i < p->series_count; i++) {
        if (strcmp(p->series[i].name, name) == 0) {
            return &p->series[i];
        }
    }

    return NULL;
}

void series_add(Profile* p, Series series) {
    if (p->series_cap <= p->series_count) {
        int cap = p->series_cap;
        p->series_cap = cap < 8 ? 8 : cap * 2;
        p->series = realloc(p->series, p->series_cap * sizeof(Series));
        if (p->series == NULL) {
            perror("Out of memory for series");
            exit(1);
        }
    }

    p->series[p->series_count++] = series;
}

Profile* new_profile(const char* uname) {
    Profile* p = malloc(sizeof(Profile));
    if (p == NULL) {
        perror("Out of memory for profile");
        exit(1);
    }

    strncpy(p->uname, uname, MAX_LEN);

    /* Both `movies` and `series` start off completely empty. */
    p->movies = NULL;
    p->movie_count = 0;
    p->movie_cap = 0;

    p->series = NULL;
    p->series_count = 0;
    p->series_cap = 0;

    return p;
}

/* Print out the stuff in a profile. */
void show_profile(Profile* profile) {
    RED();
    printf("PROFILE:\n");
    RESET();

    printf("Name: %s\n", profile->uname);

    printf("Movies in list: ");
    if (profile->movie_count == 0) {
        printf("none\n");
    } else {
        printf("\n");
        for (int i = 0; i < profile->movie_count; i++) {
            movie_print(profile->movies[i]);
        }
    }

    printf("Series in list: ");
    if (profile->series_count == 0) {
        printf("none\n");
    } else {
        printf("\n");
        for (int i = 0; i < profile->series_count; i++) {
            series_print(profile->series[i]);
        }
    }
}

void reset_profile(Profile* profile) {
    if (profile->movies != NULL) {
        free(profile->movies);
        profile->movies = NULL;
        profile->movie_cap = 0;
        profile->movie_count = 0;
    }

    if (profile->series != NULL) {
        free(profile->series);
        profile->series = NULL;
        profile->series_cap = 0;
        profile->series_count = 0;
    }
}

/* Read the profile from a file. */
Profile* get_profile() {
    Profile* profile = malloc(sizeof(Profile));
    if (profile == NULL) {
        perror("Out of memory for profile");
        exit(1);
    }
    FILE* f = fopen("profile.bin", "r");
    if (f == NULL) {
        // Profile does not exist / failed to open.
        return NULL;
    }

    /* Read basic data like the username, number of movies and number of series.
     * We'll need the two numbers while malloc-ing to allocate exactly the
     * right amount of memory. */
    fread(&profile->uname, sizeof(profile->uname), 1, f);
    fread(&profile->movie_count, sizeof(profile->movie_count), 1, f);
    fread(&profile->series_count, sizeof(profile->series_count), 1, f);

    /* Set the capacity for the movie and series arrays to some safe number. */
    profile->movie_cap = profile->movie_count * 2;
    profile->series_cap = profile->series_count * 2;

    /* Bunch of mallocs */
    profile->movies = malloc(sizeof(Movie) * profile->movie_cap);
    for (int i = 0; i < profile->movie_count; i++) {
        fread(&profile->movies[i], sizeof(Movie), 1, f);
    }

    profile->series = malloc(sizeof(Series) * profile->series_cap);
    for (int i = 0; i < profile->series_count; i++) {
        fread(&profile->series[i], sizeof(Series), 1, f);
    }

    fclose(f);
    return profile;
}

/* Save the profile to a file. */
void save_profile(Profile* profile) {
    FILE* f = fopen("profile.bin", "w");
    if (f == NULL) {
        perror("Failed to save profile");
        exit(1);
    }

    // Write basic data
    fwrite(&profile->uname, sizeof(profile->uname), 1, f);
    fwrite(&profile->movie_count, sizeof(profile->movie_count), 1, f);
    fwrite(&profile->series_count, sizeof(profile->series_count), 1, f);

    // write movies
    for (int i = 0; i < profile->movie_count; i++) {
        fwrite(&profile->movies[i], sizeof(Movie), 1, f);
    }

    // write series
    for (int i = 0; i < profile->series_count; i++) {
        fwrite(&profile->series[i], sizeof(Series), 1, f);
    }

    fclose(f);
}

/* Free all of the arrays allocated. Don't want memory leaks, yknow? */
void cleanup(Profile* profile) {
    if (profile->movies != NULL) {
        free(profile->movies);
        profile->movies = NULL;
    }

    if (profile->series != NULL) {
        free(profile->series); 
        profile->series = NULL;
    }
}

void wait_for_key() {
    printf("Press any key to go back.\n");
    __getch();
}

void print_header() {
    GREEN();
    center_text("Watchlist.\n");
    RESET();
    center_text("====================\n");
}

void print_menu() {
    printf("\t[1] View your profile\n");
    printf("\t[2] Add a movie\n");
    printf("\t[3] Add a series\n");
    printf("\t[4] Find and modify a movie or series\n");
    printf("\t[5] Reset the profile\n");
    printf("\t[6] Exit\n");
}

void print_movie_modification_menu() {
    printf("\t[1] Change name\n");
    printf("\t[2] Toggle the watched state\n");
    printf("\t[3] Go back\n");
}

void print_series_modification_menu() {
    printf("\t[1] Change name\n");
    printf("\t[2] Change number of episodes\n");
    printf("\t[3] Toggle the watched state\n");
    printf("\t[4] Go back\n");
}

void modify_movie(Movie* movie) {
    clrscr();
    print_header();
    movie_print(*movie);
    print_movie_modification_menu();

    while (true) {
        if (__kbhit()) {
            int key = __getch();

            switch (key) {
                case 49: { // 1
                    clrscr();
                    printf("Enter a new name.\n");
                    printf("%s --> ", movie->name);
                    fgets(movie->name, MAX_LEN, stdin);
                    wait_for_key();
                    fflush(stdin);
                    break;
                }
                case 50: { // 2
                    clrscr();
                    movie_toggle_watch(movie);
                    printf("%s is now set as ", movie->name);
                    printf(movie->watched ? "watched\n" : "not watched\n");
                    wait_for_key();
                    break;
                } 
                case 51: goto end; // 3
                default: {
                    fprintf(stderr, "Invalid option. Try again!\n");
                    wait_for_key();
                    break;
                }
            }

            clrscr();
            print_header();
            movie_print(*movie);
            print_movie_modification_menu();
        }
    }

end: return;
}

void modify_series(Series* series) {
    clrscr();
    print_header();
    series_print(*series);
    print_series_modification_menu();

    while (true) {
        if (__kbhit()) {
            int key = __getch();

            switch (key) {
                case 49: { // 1: Change the name
                    clrscr();
                    printf("Enter a new name.\n");
                    printf("%s --> ", series->name);
                    fgets(series->name, MAX_LEN, stdin);
                    printf("Name changed.\n");
                    wait_for_key();
                    fflush(stdin);
                    break;
                }
                case 50: { // 2: Change the number of episodes
                    clrscr();
                    printf("Enter the new number of episodes.\n");
                    printf("%d --> ", series->episodes);

                    char buf[MAX_LEN];
                    int value;
retry:
                    fgets(buf, MAX_LEN, stdin);
                    if (sscanf(buf, "%d", &value) != 1) {
                        fprintf(stderr, "Please enter a number!\n");
                        goto retry;
                    }
                    series->episodes = value;

                    printf("Number of episodes changed.\n");
                    wait_for_key();
                    fflush(stdin);
                    break;
                } 
                case 51: { // 3: Toggle the watched state
                    clrscr();
                    series_toggle_watch(series);
                    printf("%s is now set as ", series->name);
                    printf(series->watched ? "watched\n" : "not watched\n");
                    wait_for_key();
                    break;
                }
                case 52: goto end; // 4: Go back
                default: {
                    fprintf(stderr, "Invalid option. Try again!\n");
                    wait_for_key();
                    break;
                }
            }

            clrscr();
            print_header();
            series_print(*series);
            print_series_modification_menu();
        }
    }

end: return;
}

void open_menu(Profile* profile) {
    print_header();
    print_menu();

    while (true) {
        /* If a key was pressed, handle it. */
        if (__kbhit()) {
            int key = __getch();
            fflush(stdin);
            clrscr();
            switch (key) {
                /* Checking the ASCII values. */
                case 49: { // 1
                    show_profile(profile);
                    wait_for_key();
                    fflush(stdin);
                    break;
                }
                case 50: { // 2
                    movie_add(profile, movie_new());
                    printf("Movie added.\n");
                    wait_for_key();
                    fflush(stdin);
                    break;
                }
                case 51: { // 3
                    series_add(profile, series_new());
                    printf("Series added.\n");
                    wait_for_key();
                    fflush(stdin);
                    break; 
                }
                case 52: { // 4
                    char query[MAX_LEN];
                    printf("Enter the movie or series to look for:\n");
                    fgets(query, MAX_LEN, stdin);
                    query[strcspn(query, "\n")] = 0;
                    
                    Movie* found_movie = movie_get(profile, query);
                    if (found_movie != NULL) {
                        printf("Found a movie with the provided name:\n");
                        modify_movie(found_movie);
                        goto search_done;
                    }

                    Series* found_series = series_get(profile, query);
                    if (found_series != NULL) {
                        printf("Found a series with the provided name:\n");
                        modify_series(found_series);
                        goto search_done;
                    }

                    fprintf(stderr, "Nothing found with that name.\n");
                    wait_for_key();
search_done:
                    fflush(stdin);
                    break;
                }
                case 53: { // 5
                    reset_profile(profile);
                    printf("Profile has been reset.\n");
                    wait_for_key();
                    fflush(stdin);
                    break;
                }
                case 54: // 6
                    goto end;
                default: {
                    fprintf(stderr, "Invalid key! Try again.\n");
                    wait_for_key();
                    fflush(stdin);
                    break;
                }
            }
            clrscr();
            print_header();
            print_menu();
        }
    }
end: return;
}

int main(void) {
    Profile* p = get_profile();
    if (p == NULL) {
        p = new_profile("heisenberg");
    }

    open_menu(p);
    save_profile(p);

    cleanup(p);

    return 0;
}
