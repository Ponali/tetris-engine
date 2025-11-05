# tetris-engine

This is an engine made for simulating Tetris playfields, with support for the Tetris Company's guidelines (SRS, T-Spins, etc.)

It also contains a default "terminal" frontend to test if the engine works, and uses everything the engine currently supports.

Every port contains markdown documents for interacting with the engine with the specific language used.

## Supported features
This is a list of all the features supported by the current version (v1.0.0) of the current version of this engine:

- SRS wall kicks and tetrimino data
- 7-piece bag system
- Lock delay (can be reset up until 15 movements)
- T-Spins
- Customizable columns and rows
- Optionally randomly spawn pieces at 180°
- Sending events to the frontend for line clears, T-spins, and updating parts of the board if necessary
- Level and score
- Tracking combos and Back-to-backs
- Holding pieces
- Game over logic
- Pausing
- Moving current piece left and right, and soft drop
- Rotating current piece clockwise, counter-clockwise and 180°
- Sonic drop, and hard drop

NOTE: The frontend is in charge of hiding the 4 first rows, and needs to add 4 to the row amount when passing it to the engine.

## Ports
This engine has first been made in python, and will be supported frequently in this language, but other versions in different languages will exist so that other platforms can be supported.

Here are all the ports of this engine that are planned and finished:

- [X] Python
  - [ ] [TKinter](https://docs.python.org/3/library/tkinter.html) frontend
  - [ ] [Casioplot](https://therainbowphoenix.github.io/PythonExtra/library/casioplot.html) frontend for [Casio Graph Math+](https://www.casio-education.fr/produits/graph-math-plus/)
- [ ] Lua
  - [ ] [OpenComputers](https://ocdoc.cil.li/) TUI frontend ([Halyde](https://github.com/Team-Cerulean-Blue/Halyde), OpenOS)
  - [ ] [OpenComputers](https://ocdoc.cil.li/) GUI frontend ([MineOS](https://github.com/IgorTimofeev/MineOS))
- [ ] Rust
- [ ] Pure Bash

## Default terminal frontend
This frontend comes with every port of the engine, and contains a few quirks depending on the terminal emulator or the operating system you're using.

### Controls
The controls in gameplay are based on Tetr.IO, except for keys like Ctrl, Shift, and Alt, and the new "move to wall" keystrokes (see "Limitations").

- Left arrow key / `4`: Move left
- Right arrow key / `6`: Move right
- `s`: Move to leftest possible position
- `d`: Move to rightest possible position
- Down arrow key / `2`: Move down
- Up arrow key / `x` / `1` / `5` / `9`: Rotate clockwise
- `z` / `3` / `7`: Rotate counter-clockwise
- `a`: Rotate 180°
- `,` / `.`: Sonic drop
- Space / `8`: Hard drop
- `c` / `0`: Hold current piece

The "Move to leftest/rightest possible position" mean that the piece will move to the left or right wall, unless if the stack is in the way.

Press the `p` key to pause, the `r` key to reset, and `q` or Ctrl+C to exit.

### Limitations
As of right now, this frontend is only tested on a Linux system running Konsole, so it might break or work differently on Windows, macOS, or other terminal emulators. If so, please report an issue on this repo.

Unfortunately, the terminal is in charge of the DAS, and there is no way to disable this, even using raw mode. To change the DAS speed for this frontend, please check the settings for your terminal emulator or your operating system for anything related to DAS.

This also means that ARR won't be implemented (it could, but it's not a good idea). Thus, if moving to the leftest or rightest possible position has to be used, you will have to use seperate keystrokes (see "Controls").

The terminal is also in charge of putting the user input into "standard in", and unfortunately, it doesn't show anything when only pressing Ctrl, Shift, or Alt, which makes these keys impossible to capture.

Rewards (e.g. tetris, t-spin, and all clear) show up below the playfield with twice the size as how characters usually show up (done through [these VT100 escape codes](https://en.wikipedia.org/wiki/ANSI_escape_code#nF_Escape_sequences)). If your terminal emulator doesn't support double size text, it will show up twice on the middle left.

This frontend expects the background to be black. If your terminal emulator is set to have a white background (like xterm and macOS), the color of the reward text, (and some other stuff, like the "HOLD" text) will show incorrectly.

The frontend also expects the aspect ratio of a character to be 1:2. If the aspect ratio is taller or wider, the playfield will squish to the the aspect ratio change.

The reccomended resolution is 80x24 (on the default playfield size). If your terminal is using a small resolution, the game may display incorrectly and break.

### Command line arguments
Unicode characters are used to render block cells. If your terminal emulator lags because of this, or these characters render incorrectly, insert `fast` to the command line arguments when launching the program.

To change the amount of columns and rows the playfield has, or the amount of lines the [buffer zone] takes, use the flags `w`/`width`/`c`/`cols`/`columns`, `h`/`height`/`r`/`rows`/`l`/`lines`, and/or `ir`/`irows`/`bz`/`bufferzone`. For example, inserting `w=20` and `h=40` into the command line arguments will make the playfield twice the usual size, and inserting `bz=20` will make the buffer zone use 20 lines instead of 4.

The Tetris Wiki mentions that pieces spawn horizontally, but they don't specify the rotation. By default, pieces only spawn at 0°, but you can insert `sr`, `rotate` or `spawnrotated` in the command line arguments to have pieces sometimes randomly spawn at 180°.

You can also find something interesting by mentioning the first version of Tetris.