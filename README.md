# Assignment 1

This is a C project using the `gtk3` and `cairo` graphics library to build an application

## Project Brief

This project draws a png image on a gtk window and detects the color pixels on the image based on the mouse pointer clicks. It separates the rgb values from the extracted color pixel and prints it out. It is the version 1 of an upgraded pick color and polish project for a2.

## Contribution

The application was built by:

```
- Emmanuel Ainoo
- Ayomide Oduba
```

## Requirements

- [`gcc C program compiler`](https://gcc.gnu.org)
- [`gtk3`](https://www.gtk.org)
- [`Cairo Graphics`](https://www.cairographics.org)

## Usage

Compile with gcc & gtk3 flags on terminal:

```shell
gcc `pkg-config --cflags gtk+-3.0` -o a1 a1.c `pkg-config --libs gtk+-3.0`
```

Run File:

```shell
./a1
```

Click anywhere on picture loaded and app will print the pixels located at the mouse click point
