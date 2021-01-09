# Cute Chess fork for reset-Mobility (r-Mobility) chess

Cute Chess is a graphical user interface, command-line interface and a library
for playing chess. Cute Chess is written in C++ using the [Qt
framework](https://www.qt.io/).

Cute Chess was written by Ilari Pihlajisto, Arto Jonsson and [contributors](https://github.com/cutechess/cutechess/graphs/contributors)

This branch, forked by windfishballad, modifies CuteChess in order to play the western board chess variants by the rules of reset-mobility, also known as [r-Mobility](https://wiki.chessdom.org/R-Mobility).

[r-Mobility](https://wiki.chessdom.org/R-Mobility), invented by Aloril, is a natural generalization of the scoring of Chess (an is also generalization of Lasker Chess scoring).

In points scoring, it allows for more granular results than 1-0 or 0.5-0.5, thinning the optimal play tree by an order of magnitude (while remaining fully included in regular chess optimal play tree), and keeping engames which are drawn by classical rules interesting. It often also rewards the attacking side in the middlegame if attack is strong but insufficient to win, since a perpetual is likely to give a low G-score (typically G1.0 or G2.0).

In Komi scoring, it allows for drawless chess. 

## Installation

Please refer to the main cutechess [page](https://github.com/cutechess/cutechess).

## Modifications to Cute Chess game play:

### Draw by insufficiant material

Draw by insufficient material no longer applies. It is replaced by "End by K vs K G8.5 rule", in which the game stop if both sides have had only bare kings AND the current position has been a G8.5 for two consecutive plies. The G-score of the game is then the lowest G-score achieved since the last capture.

Justifiation for this rule is that if this occurs, in optimal play G-score of the game can no longer be improved, and optimal play is fairly trivial. 

### Draw by 3-fold repetition

3-fold repetition still ends the game. G-score is then the lowest G-score since the last reset of the game, as per [r-Mobility](https://wiki.chessdom.org/R-Mobility) rules.

### Fifty moves rule

The default implementation of the 50 move rules is that any reset OR any improvement to the best G-score since last reset zeroes the 50 moves counter. The improvement is only counted if it is strictly lower than the G-Score cutoff. The cutoff defaults to G6.0 and cannot be changed in the GUI. It is set in cli using the `-rMobilityCutoff` option, for example `-rMobilityCutoff g101.5`.

The rationale is that being close to the 50 moves boundary would otherwise allow to enter sequences that could make a player lock a better g-Score, because their refutation would not be possible due to hitting the 50-moves rule. A side effect is that some (but not all) tablebase cursed mates become actual mates, s improved G-scores on the path would reset the counter.

However, including the option `-legacyMode` in cli makes the 50 moves rule stop the game in the usual way, to be used in case one of the engines does not support the new default 50-moves rule. Legacy mode is not available through the GUI. In the Legacy mode, a G-score improvement at the last ply (which triggers the 50-moves rule) is not counted, to avoid low score by adjacent unprotected check (which is the most trivial example of the boundary "cheating" that the default rule avoids).

### Tablebase adjudication

Win tablebase adjudication works as in usual cutechess, and results in a G0.0 /-G0.0 score. Draw tablebase adjudication no longer applies, and setting is just being ignored.

## Points scoring of games

### Exponential (default)

Score is 0.5 +/- 0.5^(1+2G). See table [here](https://wiki.chessdom.org/R-Mobility#TCEC).

### Classical

Set by `-rMobilityType classical`
Classical scoring of a game. G0.0 is 1-0, -G0.0 is 0-1, any other score is 0.5-0.5.
Note that the overriden 50-moves rule still applies by default even if scoring is Classical.

### Harmonic

Set by `-rMobilityType harmonic`
Score is 0.5+/-0.5/(1+2G). It makes fighting for the best G-score in high scoe games still relevant (contrary to Exponential), but the tradeoff is that it makes achieving low scores like stalemate, G1.0 or G1.5 less important.

### RMobility Winner

Set by `rMobilityType rMobilityWinner`
If G score is lower than cutoff (set in cli with `-rMobilityCutoff` option, for example `-rMobbilityCutoff G130.5`), then 1-0 if White is the holder of the G-Score, 0-1 if Black is the holder of the G-Score.

### Komi

#### Setting

Set by `-rMobilityType komi`

The komi is read from the opening (only compatible with PGN files), for example `[Komi "G4.25"]`, `[Komi "-G5.75"]`, `[Komi "G2.0"]`

If no opening is provided, or if does not contain the `[Komi..` tag, a default komi will be used, which is set using`-defaultKomi`, for example:
`-defaultKomi g5.0`, `-defaultKomi mg5.75` (m has to be used in lieu of - sign when setting the komi in cli). If this option is not set, default value is G5.75. it cannot be set in the GUI, therefore G5.75 will always be used if the opening does not provide a komi.

#### Scoring

Komi can take integer, half-integer and quater-integer values (G3.0, G3.25, G3.5, G3.75 are examples of possible Komi values). The player whose sign has the komi wins if he scores a better G-score, loses if he scores a worse G-score, draws if he scores exactly the Komi.

Examples: If Komi is G2.25: white wins at G2.0 and losses at G2.5 (and at -G anything). If Komi is -G3.5, Black wins at -G3.0, draws at -G3.5 and loses at -G4.0 (an at any positive G-score).


#### Komi behavior at and above cutoff

All g-value above the cutoff are considered equivalent (a dead draw). if komi is set at the cutoff, a dead draw is scored as a draw: 0.5-0.5. If komi is set above the cutoff, it means that the player who has the komi (White is positive, Black is negative) has dead draw odds.

Examples (in all examples below, cutoff is 6.0).
Komi is G6.0 or -G6.0: White wins as G5.75 or better, Black wins at -G5.75 or better, any score G6.0 or higher, or -G6.0 or higher is a draw
Komi is G6.25: White wins at any positive G score, and at a negative G score which is -G6.0 and above. Black needs -G5.75 or better to win. Draw is not possible.
Komi is -G6.25: WHite needs G5.75 or better to win. Black wins at ny negative G-score, and at positive G-score G6.0 and above. Draw is not possible.



## Inputs and Outputs:

### Options in cli

The below are the inputs options which are aded to the standard cutechess options (they all still work as usual except `-draw`; setting `-draw` won't break it but will be ignored).

None of these options can be set in the GUI.

`-rMopilityType`: values are `exponential` (default), `classical`, `harmonic`, `rMobilityWinner`, `komi`. Decides the main r-Mobility scoring (which will be consumed by SPTR and displayed after each game, and be written in the PGN output). Tournament results output always contains all tables with all possible scoring conventions.

`-rMobilityCutoff`: sets the cutoff, in th format `gX.0`, `gX.5`, where `X` is an integer with any number of digits. Defaults to G6.0.

`-legacyMode`: if passed, 50 moves rule will be per legacy mode. Defaults to non-legacy mode.

`-defaultKomi`: the komi to use for komi scoring if not available in the opening., in the format `gX.0`, `gX.25`, `gX.5`, `gX.75` or `mGX.0`, `mGX.25`, `mGX.5`, `mGX.75` where `X` is an integer with any number of digits. Defaults to G5.75.


### FENs

FENs (inputs and outputs) have an aditional accreted G-Score, as per the two below examples:
`rnbqkbnr/pp2pppp/3p4/2p5/2P1P3/5N2/PP1P1PPP/RNBQKB1R b KQkq - 0 3 G28.5`
`rnbqkbnr/1p2pppp/p2p4/2p5/2P1P3/2N2N2/PP1P1PPP/R1BQKB1R b KQkq - 1 4 -G26.5`

If the 50 move rule ply count is 0 (as in fist example), this accreted G-score is redunant since the current position resets the G-score. It is also redunant if the score of the current FEN position is equal to this G-score (with same sign inclued) or is better (with any sign).

FENs without this aditional field still work, and are understood as that the current position realizes the accreted G-score.

### PGNs

As opening inputs, they can consume expanded starting FENs as describe above. They also consume a Komi field (`[Komi "G5.5"]`) to set the Komi of the game.

As outputs:

`[Result]` tag is the classical result of the game in the usual format (and the move scores also ends by the classical result of the game).

`[RMobiliyResult]` is the GScore of the game.

`[Points]` contains the points from the game as per the chosen scoring. If Classical, RMobilityWinner or Komi, format is "1-0";"0-1";"1/2-1/2". If Exponential or Harmonic, format is float with 4 decimal places, for example "0.6250 - 0.3750".

`[RMobilityCutoff]` is the cutoff that was applied for the tournament.

`[Komi]` is the komi at which the game was played, only present in th PGN output if the scoring mode of the tournament is Komi.

If the tournament is in legacy mode, `[Legacy]` tag is added as such (without any value).

### ELO and LOS

ELO an LOS are using the usual calculation, with the r-Mobility points of the corresponding scoring. ELO only depends on the total point ratio (for example, 54.5%), its confidence interval and the LOS also depend on the variance of the points per game. Only difference is that points per game may take more values than 1.0, 0.5 an 0.0 in Exponential and Harmonic scoring, which does not change the calculations.

### SPRT and LLR

SPRT and LLR have been extended in  way that they coinide with the usual calculation if all realized games point take value in {1.0,0.5,0.0}.
Number of wins, number of losses and number of draws are now real-value.
If points>=0.5 in  game, number of wins is incremented by (points-0.5)/0.5, number of draws by (1-points)/0.5.
If points <0.5, number of losses is incremented by (0.5-points)/0.5 and number of draws by points/0.5.
The condition that number of draws>0 for an SPRT to be valid is relaxed, because in komi play draws can be impossible (for non-half integers komi) or very rare event (for hal-integer but large komi). Number of draws > 0 is not a really necessary, as it does not alter the mathematical rationale of SPRT/LLS (it just means that drawElo=0).
On other hand, condition that number of wins >= 1 and number of losses >=1 is enforced (results are nonsensical if they are >0 but very small).


###

## License

Base cutechess code (except for the components in the `projects/lib/components` and the `projects/gui components` which are released under the MIT license) and r-Mobility additional code are released under the GPLv3+ license.

## Credits

Reset-Mobility (aka rMobility) was invnted by Aloril.
Base cutechess code was written by Ilari Pihlajisto, Arto Jonsson and [contributors](https://github.com/cutechess/cutechess/graphs/contributors).
Additional r-Mobility support code, and required modifications to base cose,  was written by windfishballad. 


