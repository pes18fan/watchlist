# watchlist

Manage your watchlist of movies, anime or anything else.

A college project. Written by me and [@SuperShekhar](https://www.github.com/SuperShekhar).

## building

Prerequesites:

- A C compiler (preferably `gcc`, but MSVC and `clang` will also work)
- `make` (not compulsory but useful)

Clone the repository, move to it and simply run `make`:

```bash
git clone https://github.com/pes18fan/watchlist.git
cd watchlist
make
```

This will produce a `wl.exe` in the repo folder.

If you don't have `make` installed, you can compile `main.c` and `term.c`
yourself:

```bash
# for MSVC:
cl main.c term.c

# for gcc or clang:
gcc main.c term.c -o wl.exe
```
