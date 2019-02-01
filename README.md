# Amazed: An amazing game about mazes

[![License](https://img.shields.io/github/license/maxine-red/amazed.svg)](https://github.com/maxine-red/amazed/blob/master/LICENSE)

[![Github Release Version](https://img.shields.io/github/release/maxine-red/amazed.svg)](https://github.com/maxine-red/amazed/releases)
[![Github Release Date](https://img.shields.io/github/release-date/maxine-red/amazed.svg)](https://github.com/maxine-red/amazed/releases)
[![Github Last Commit](https://img.shields.io/github/last-commit/maxine-red/amazed.svg)](https://github.com/maxine-red/amazed)

## Description

Amazed is a simple curses based maze solving game for Linux, other Unices and
Windows.

You start in the center of a randomly generated maze and have to find a power
cell.

The power cell is denoted by a cyan diamond.

Your objective, is to reach the power cell without running out of energy.
Though each second and each step drain your energy. Also the longer you take and
the more you move, the quicker your energy is drained!

You can pick between 3 maze generation algoriths:
- Kruskal (Favors short pathes and dead ends)
- Depth-First (Favors long corridors and isn't very user friendly)
- Prim (Very much like Kruskal, with more structure at the center)

Depth-First is really luck based, as the corridors tend to be very long. I still
kept it in, for people who like that kind of thing.

Kruskal is the default.

You move with arrow keys. With 'q' you can go back to main menu or quit the
game. Items, in menus, are selected with enter or return.

## Installation

Download an appropriate binary and just run it.

Windows users can just double click on the .exe file, as all libraries are
statically linked (all code is included, but it's bigger).

For Linux and other Unix-like users, you may have to install ncurses, if you
haven't already.
Most systems should have it installed already, but if you're on Ubunty/Debian,
you can run `sudo apt install ncurses-base`.

For xterm users, you have to set the environment variable TERM to
'xterm-256color'. This ensures that the game is able to change color
definitions. You can easily do that by staring the game like this: 
`TERM=xterm-256color amazed [options]`

If you like to compile the game, there are some considerations:

### Windows

Builds on Windows are not directly supported! I develop on Debian/GNU Linux and
cross compile for windows. Windows uses 
[PDCurses](https://github.com/wmcbrine/PDCurses) and statically links to it.

Like I mentioned though. I have a private script to cross-compile things and
can't test a build setup on Windows. So if you like you can make it work!

### Linux / Unices

First make sure that you have a proper building environment and development
styles.

Then: `apt install libncurses5-dev` for Debian/Ubuntu

`./configure && make` will create a binary for your system, you can run.

If code is obtained, there are a few other commands available:
- make love
- make friends
- make documentation
- make check-style

## Examples

`amazed [-d] [-k] [-p]`

Make sure to use the xterm specific variable, mentioned above, if you use xterm.

## Donations

[![Patreon](https://img.shields.io/badge/Patreon-donate-orange.svg)](https://www.patreon.com/maxine_red)
[![KoFi](https://img.shields.io/badge/KoFi-donate-blue.svg)](https://ko-fi.com/maxinered)

## License

[![GPLv3](https://www.gnu.org/graphics/gplv3-127x51.png)](https://www.gnu.org/licenses/gpl-3.0.en.html)

Copyright 2019 :copyright: Maxine Michalski

## Contributing

1. [Fork it](https://github.com/maxine-red/amazed/fork)
1. Create your feature branch (git checkout -b my-new-feature)
1. Commit your changes (git commit -am 'Add some feature')
1. Push to the branch (git push origin my-new-feature)
1. Create a new Pull Request
