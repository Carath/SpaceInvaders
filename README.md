# Space Invaders

*Space Invaders -like game*


## Installation

Only Linux (Ubuntu) is supported at this time.

* Installing the graphical library SDL2, and some FreeType fonts:

```
sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libfreetype6-dev fonts-dejavu
```


## Compilation

Compiling is done by running ``` make ```.


## Runtime

```
./spaceinvaders.exe
```

There is an user guide, though only in French as this time: *manuel.pdf*


## Limitations

- Only one supported resolution: 1000 x 1000
- Only ASCII characters can be used.
- Controls are hardcoded.


## Known issues

SDL2 v2.0.10 causes a drawing bug: rendering something changes the color of a specific pixel of the last drawn object. This bug however is not present in versions 2.0.8 and 2.0.14. Should it arise, uninstall the libsdl2-dev package, then install [this one](https://packages.debian.org/sid/libsdl2-2.0-0) and [this one.](https://packages.debian.org/source/sid/libsdl2) Finally, libsdl2-image-dev and libsdl2-ttf-dev may need to be updated after that.
