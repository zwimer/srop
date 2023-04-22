# srop
A simple srop tutorial challenge

_Note_: This will _not_ work if your CPU contains specialized hardware to prevent ROP.

# Docker Image

`zwimer/srop` is a prebuilt docker image containing the built challenge and `solution.py`.

# Compilation

A `Makefile` is provided in the `src/` directory

# Solution script

The offsets may need to be changed depending on your compiler. This solution script was made for `gcc 11.3.0` on `ubuntu 22.04`; offsets might need to be changed slightly for different compilers on different versions.

This script uses `pwntools`, it can be installed with `pip install pwntools`.
