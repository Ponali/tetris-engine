# Initializing the engine

To first use the engine, you need to give it a function for getting the current time in milliseconds, named `getTimestamp`.
This is done instead of directly relying on the included `time` module, for platforms with other Python versions that don't have that module (e.g. Casio's MicroPython).

If your platform does support the `time` module, you can start your code with this:

```py
from engine import initTetrisEngine, ...

def getTimestamp():
	return time.time()*1000

initTetrisEngine(getTimestamp)
```

# Handling events

Before making the Tetris grid, you will need to make a function for handling the events it will send. These events are:

- `place`: Sent when a piece gets placed.
- `updateNext`: Sent when the next pieces have changed (usually when a new piece is available).
- `updateScore`: Sent when the score has changed.
- `updateHoldPiece`: Sent when the hold piece has changed.
  NOTE: this event will be sent twice - one for when the piece is unavailable, and another for when the piece is available.
- `tSpin`: Sent when a T-Spin has occured. Contains a boolean `mini` and an integer `lines` in the dictionary argument.
- `lineClearAnim`: Sent when lines have been cleared, with a `lines` list in the dictionary argument, containing all the Y positions of lines that have been cleared.
- `lineClear`: Sent when lines have been cleared, after `lineClearAnim`. Contains the number of lines in `lines`, and a boolean in `allClear` (set to `True` when the playfield is empty.)

The function that handles these events has to take three arguments:

1. The grid that sent the event
2. The string (or "ID") of the event
3. A dict containing extra data about the event

# Creating a Tetris grid

The Tetris grid is a class available as the name `TetrisGrid`. When created, it takes in 4 arguments:

1. Amount of columns
2. Amount of rows (including 4 invisible rows)
3. Randomly spawn pieces at 180°
4. Event handler function

Here is an example:

```py
from engine import TetrisGrid, ...

grid = TetrisGrid(columns,rows+4,False,eventHandler)
```

# Handling a 2D grid of cells

To get the current grid of cells that can be displayed to a screen by the frontend, use `grid.getScreenWithCurrentPiece()`.
This function will return a `2DGrid` object, which contains the following values and methods:

- `get(x: int,y: int,[replace: bool])`: returns the cell at a specific position. if the index is out of bounds, return the `replace` argument (if no such argument is given, errors out.)
- `set(x: int,y: int,value: int)`: sets the cells to a specific value.
- `copy()`: returns another Grid2D object with the same content.

The cells in a `2DGrid` object have the following colors:

- `0`: air/empty
- `1`: cyan (I piece)
- `2`: yellow (O piece)
- `3`: purple (T piece)
- `4`: orange (L piece)
- `5`: blue (J piece)
- `6`: green (S piece)
- `7`: red (Z piece)
- `8`: gray (shadow)

You can use this in order to render the grid and show it to the screen in your frontend code.

# Handling the stack and the piece differently
If you want to make a frontend with smooth piece movement, you will need to handle the stack and the piece differently.

To get the stack of the playfield, use `grid.grid`, which is again a `2DGrid` object, and has the same colors attributed as `getScreenWithCurrentPiece`.

To get the current piece, use:

- `grid.tx` for the X position
- `grid.ty` for the Y position
- `grid.tt` for the type of tetrimino
- `grid.tr` for the tetrimino rotation

The color of the tetrimino is one less than the cell value in the grid. If you rely on a single function for getting the color of a grid's cell, use this to get the tetrimino color:

`tetriminoColor = getCellColor(grid.tt+1)`

To get tetrimino data, make sure to import `tetrominoData` (make sure to spell "tetr**o**mino" right!) from the engine.
You will get a 4D array (7x4x4x4) for every 4x4 plane for every rotation of every piece. The index order goes like this:

`tetriminoData[grid.tt][grid.tr][grid.ty][grid.tx]`

Please note that this uses vanilla Python arrays instead of [NumPy](https://numpy.org/) arrays.