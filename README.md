# terminal-mines
An [ncurses](https://en.wikipedia.org/wiki/Ncurses)-based minesweeper game. This was made because I wanted a minesweeper game that was efficient to play without mouse.

![screenshot](screenshots/terminal-mines.png)

## Compilation
After cloning the project, `cd` to the folder and run:

```
git submodule init
git submodule update
make
```

Now you should have a binary called `terminal-mines` in the folder. You can place it wherever you want!

## Starting the game
Run the game with `./terminal-mines` or add an alias for it.

To select size/difficulty, play with the following arguments:

- `--width` Width of the game board
- `--height` Height of the game board
- `--mine-density` a value between 0.0 and 1.0 where 1.0 means every tile will have a mine, and 0.0 means zero tiles will have mines.

All the arguments can be written shorthand with the first letter in the name.

Example:
```
./terminal-mines -w 20 -h 20 -m 0.5
```

## Controls
- Movement: `hjkl` or arrow keys
- Place flag: `f` or `g`
- Open tile: `,`

The controls were inspired by nethack/vim.