#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Include windows headers if compiling on Windows. */
#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#endif

/* Maximum possible string length allowed. */
#define MAX_LEN 100

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
#endif
#ifdef __linux__
#define RED() printf("\033[1;31m")
#define GREEN() printf("\033[1;32m")
#define RESET() printf("\033[0m")
#endif

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
    printf("Movie name: %s\n", movie.name);
    printf("Watched: %s\n", movie.watched ? "yez" : "nope");
}

/* Get a movie from the movie list in a profile. */
Movie* movie_get(Profile* p, const char* name) {
    for (int i = 0; i < p->movie_count; i++) {
        if (strncmp(p->movies[i].name, name, MAX_LEN)) {
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

    printf("Enter number of episodes:\n");
    /* Currently crashing at this line on windows. Dunno why. */
    scanf("%d", &series.episodes);
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
    printf("Series name: %s\n", series.name);
    printf("Episodes: %d\n", series.episodes);
    printf("Watched: %s\n", series.watched ? "yez" : "nope");
}

Series* series_get(Profile* p, const char* name) {
    for (int i = 0; i < p->series_count; i++) {
        if (strncmp(p->series[i].name, name, MAX_LEN)) {
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
    
    printf("Movies in list:\n");
    for (int i = 0; i < profile->movie_count; i++) {
        movie_print(profile->movies[i]);
    }

    printf("Series in list:\n");
    for (int i = 0; i < profile->series_count; i++) {
        series_print(profile->series[i]);
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

    // read basic data
    fread(&profile->uname, sizeof(profile->uname), 1, f);
    fread(&profile->movie_count, sizeof(profile->movie_count), 1, f);
    fread(&profile->series_count, sizeof(profile->series_count), 1, f);

    /* Bunch of mallocs */
    profile->movies = malloc(sizeof(Movie) * profile->movie_count);
    for (int i = 0; i < profile->movie_count; i++) {
        fread(&profile->movies[i], sizeof(Movie), 1, f);
    }

    profile->series = malloc(sizeof(Series) * profile->series_count);
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

int main(void) {
    Profile* p = get_profile();
    if (p == NULL) {
        p = new_profile("heisenberg");
    }

    show_profile(p);
    save_profile(p);

    /* Call getch() only if on Windows, since its not on Linux. Probably
     * won't need this for long anyways, once there's a proper main menu. */
#ifdef _WIN32
    _getch();
#endif
    cleanup(p);

    return 0;
}
