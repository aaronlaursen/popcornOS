wumpOS
=========

tiny, hand-rolled operating system

by Aaron Laursen 
-- aaronlaursen (at) gmail (dot) com
-- alaursen (at) macalester (dot) edu

requirements:
* QEMU
* Clang/LLVM
* bash, make, etc.

gcc can probably be used by editing the makefile

running make should compile, link, and run the whole thing.

should be pretty straightforward from there:
* it runs a little presentation slide show (press h,j,k, or l to advance)
* prints some debugging
* prints the fib. sequence < 10000
* plays hunt the wumpus on a 128 cell, 4-way junctioned, toroidal grid (up and
  down advance/retreat 32 cells), instructions should display on-screen.
