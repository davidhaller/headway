# Headway

## Design

In 1970, the British mathematician John Conway invented a cellular automaton called *Game Of Life*. The simulation takes place in a two-dimensional grid of cells represented as rectangles. A cell can either be *alive* or *dead*. This state can change from one generation to another, depending on the amount of neighbor cells. If a cell has two neighbors, it keeps its state, three neighbors means that the cell will be revived when it was dead before. A cell dies when it has zero, one or more than four neighbors. Therefore, life is threatened by overpopulation and aloneness.

At every generation transition, the neighbors of each cell are counted and, depending on the result, the rules mentioned before are applied to each cell. There are several possible simulation outcomes which can occur, e.g:

- death of all cells
- all cells turn stable, so in every future generation no cell dies or gets revived
- periodic movement patterns
- chaotic fluctuation, no recognizable pattern

Although the simulation may appear to be chaotic, it's actually deterministic, so the same starting parameters will always lead to the same simulation progress.

![Headway Screenshot](screenshot.png?raw=true)

## Implementation

Of course you could perform this simulation using pencil and paper, but it makes more fun when using a computer. For that case, I've implemented a *Game Of Life* simulator. Using a graphical user interface, you can set all the parameters (like world size or simulation speed) and watch your simulation going on. You can also fill the world with random cells and create/kill single cells by clicking on them. Every generation can be saved as a snapshot to an XML or JSON file.

### Compiling and Running

Headway is implemented in C++ and QML. You can compile and run the app using Qt Creator, or you can execute the following commands inside the project directory:

```bash
$ mkdir build
$ cd build
$ cmake ..
$ make
```

Depending on your platform, a executable file named `Headway.exe` or `Headway.app` (or just `Headway`) will be created. You may need to install the latest Qt SDK first (6.9 or later) and the CMake build system (could be bundled with Qt). Other dependencies are not required. Headway should run on any desktop operating system supported by Qt (macOS, Windows, Linux).

### Executing unit tests

To execute the unit tests, you can either run them using Qt Creator, or using the command line with `ctest`:

```bash
$ ctest --verbose
```

## Examples

In the *examples* directory, example files can be found. There is also a XSD schema file describing their format, which can be used for validation:

```bash
$ xmllint --schema headway.xsd flower.xml --noout
flower.xml validates
```

## Documentation

If you have Doxygen installed on your system, you can generate source code documentation stored at *doc/html* by running

```bash
$ doxygen
```

inside the project root directory.