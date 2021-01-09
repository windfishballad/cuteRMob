# Cute Chess fork for reset-Mobility (r-Mobility) chess

Cute Chess is a graphical user interface, command-line interface and a library
for playing chess. Cute Chess is written in C++ using the [Qt
framework](https://www.qt.io/).

Cute Chess was written by Ilari Pihlajisto, Arto Jonsson and [contributors](https://github.com/cutechess/cutechess/graphs/contributors)

This branch, forked by windfishballad, modifies CuteChess in order to play the western board chess variants by the rules of reset-mobility, also known as [r-Mobility](https://wiki.chessdom.org/R-Mobility).

[r-Mobility](https://wiki.chessdom.org/R-Mobility), invented by Aloril, is a natural extension of the rules of Chess.

In points scoring, it allows for more granular results than 1-0 or 0.5-0.5, thinning the optimal play tree by an order of magnitude (while remaining fully included in regular chess optimal play tree), and keeping engames which are drawn by classical rules interesting.

In Komi scoring, it allows for drawless chess. 

## Installation

Please refer to the main cutechess [page](https://github.com/cutechess/cutechess).

## License

Base cutechess code (except for the components in the `projects/lib/components` and the `projects/gui components` which are released under the MIT license) and r-Mobility additional code are released under the GPLv3+ license.

## Credits

Base cutechess code was written by Ilari Pihlajisto, Arto Jonsson and [contributors](https://github.com/cutechess/cutechess/graphs/contributors).
Additional r-Mobility support code, and required modifications to base cose,  was written by windfishballad. 


