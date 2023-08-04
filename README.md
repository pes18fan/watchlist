# watchlist

Manage your watchlist.

In development.

## building

Make sure you have `gcc` installed. MSVC or `clang` should also work,
but the code was written with `gcc` in mind, so there are no guarantees.

Clone the repository, move to it and simply run `make`:

```bash
git clone https://github.com/pes18fan/watchlist.git
cd watchlist
make
```

This will produce a `wl.exe` in the repo folder.

If you don't have `make` installed, you can also run the provided PowerShell 
build script which does the same thing, or you can compile it yourself, if you're
using another compiler, for example. (make sure to compile both `main.c` and
`term.c`).
