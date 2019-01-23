# Amazed: A small game collection, with synth support

[![Travis build](https://img.shields.io/travis/maxine-red/alex.svg)](https://travis-ci.org/maxine-red/alex)
[![License](https://img.shields.io/github/license/maxine-red/alex.svg)](https://github.com/maxine-red/alex/blob/master/LICENSE)

[![Codacy Badge]()]()
[![Codacy Coverage]()]()

[![Github Release Version](https://img.shields.io/github/release/maxine-red/alex.svg)](https://github.com/maxine-red/alex/releases)
[![Github Release Date](https://img.shields.io/github/release-date/maxine-red/alex.svg)](https://github.com/maxine-red/alex/releases)
[![Github Last Commit](https://img.shields.io/github/last-commit/maxine-red/alex.svg)](https://github.com/maxine-red/alex)

## TODO

### before demo release
- [ ] implement score system properly

## before official release
- [ ] implement time based drain
- [ ] implement SDL variant
- [ ] implement Alex support

## Description

Amazed is a maze crawler.

Several environments are planned, as well as different synths and interfaces.

Interfaces planned:
- [ ] text based (Curses)
- [ ] fully graphical (SDL)

Environments planned:
- [x] position based mazes
- [ ] round based snakes
- [ ] other games, that can be round based (single player)

Synths:
- [ ] Alex (SARSA)
- [ ] Tabi (DQN)

## Installation

For source install only: Make sure you have ncurses and SDL installed

`sudo apt update && sudo apt install ncurses-base libncurses5-dev libsdl2-2.0-0
 libsdl2-dev`

A simple three step installation from source

`./configure && make && (sudo) make install`

Binaries will be available too, for different platforms.

If code is obtained, there are a few other commands available:
- make love
- make friends
- make test
- make documentation
- make check-style

## Examples

```bash
alex [-g GAME] [-v VIDEODRIVER] [-a AGENT] [-V]
```

## Donations

[![Patreon](https://img.shields.io/badge/Patreon-donate-orange.svg)](https://www.patreon.com/maxine_red)
[![KoFi](https://img.shields.io/badge/KoFi-donate-blue.svg)](https://ko-fi.com/maxinered)

## License

[![GPLv3](https://www.gnu.org/graphics/gplv3-127x51.png)](https://www.gnu.org/licenses/gpl-3.0.en.html)

Copyright 2019 :copyright: Maxine Michalski

## Contributing

1. [Fork it](https://github.com/maxine-red/rinforce++/fork)
1. Create your feature branch (git checkout -b my-new-feature)
1. Commit your changes (git commit -am 'Add some feature')
1. Push to the branch (git push origin my-new-feature)
1. Create a new Pull Request
