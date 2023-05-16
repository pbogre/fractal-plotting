# fractal-plotting

## What?

Currently heavily work in progress.

Plot famous fractal patterns from sets like [Mandelbrot](https://en.wikipedia.org/wiki/Mandelbrot\_set),
[Julia](https://en.wikipedia.org/wiki/Julia\_set),
[Burning ship](https://en.wikipedia.org/wiki/Burning\_Ship\_fractal).

This is done by converting pixel coordinates into real numbers on the argand diagram,
and estimating which numbers can remain bounded following a certain sequence
(which depends on the type of fractal) for infinity, but also color mapping the ones
that almost get there because it looks pretty.

## How?

Currently tested on MacOS and Windows.

### Requirements

- CMake
- C++ Compiler

### Instructions

- Create a `build` directory
- Enter the directory and run the cmake files generation (`cd build && cmake ..`).
  - This will also install the SFML library for you.
- Run the `make` command to compile everything, and run the `fractal` binary file.

## Next up

- [ ]  Improve coloring method / pallete (currently need to adjust for `max_iterations`)
- [ ]  Improve rendering (multithreading, parallel rendering, ...)
- [x] ~~Fix zooming / panning controls~~
