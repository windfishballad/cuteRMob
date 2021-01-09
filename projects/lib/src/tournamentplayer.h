/*
    This file is part of Cute Chess.
    Copyright (C) 2008-2018 Cute Chess authors

    Cute Chess is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Cute Chess is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Cute Chess.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TOURNAMENTPLAYER_H
#define TOURNAMENTPLAYER_H

#include "playerbuilder.h"
#include "timecontrol.h"
#include "board/side.h"
#include "board/result.h"

class OpeningBook;


/*! \brief A class for storing a player's tournament-specific details. */
class LIB_EXPORT TournamentPlayer
{
	public:
		/*! Creates a new TournamentPlayer object. */
		TournamentPlayer(PlayerBuilder* builder,
				 const TimeControl& timeControl,
				 const OpeningBook* book,
				 int bookDepth);

		/*! Returns the player's builder object. */
		const PlayerBuilder* builder() const;
		/*! Returns the player's name. */
		QString name() const;
		/*! Sets the player's name to \a name. */
		void setName(const QString& name);
		/*! Returns the player's time control. */
		const TimeControl& timeControl() const;
		/*! Returns the player's opening book. */
		const OpeningBook* book() const;
		/*! Returns the player's maximum book depth. */
		int bookDepth() const;
		/*!
		 * Returns the total number of wins the player has in the
		 * tournament.
		 */
		int objectives(int gScore) const;

		int whiteObjectives(int gScore) const;

		int blackObjectives(int gScore) const;


		int nWhiteGames() const;

		template<Chess::rMobScoring> int wins() const;
		template<Chess::rMobScoring> int losses() const;
		template<Chess::rMobScoring> int draws() const;
		template<Chess::rMobScoring> int whiteWins() const;
		template<Chess::rMobScoring> int whiteLosses() const;
		template<Chess::rMobScoring> int whiteDraws() const;
		template<Chess::rMobScoring> int blackWins() const;
		template<Chess::rMobScoring> int blackLosses() const;
		template<Chess::rMobScoring> int blackDraws() const;

		template<Chess::rMobScoring> qreal points() const;
		template<Chess::rMobScoring> qreal whitePoints() const;
		template<Chess::rMobScoring> qreal blackPoints() const;

		template<Chess::rMobScoring> qreal squarePoints() const;
		template<Chess::rMobScoring> qreal whiteSquarePoints() const;
		template<Chess::rMobScoring> qreal blackSquarePoints() const;

		/*! Adds \a score to the player's score in the tournament. */

		void addScore(Chess::Side side, Chess::rMobResult gResult, Chess::rMobKomi komi, int gCutoff, Chess::rScoring* exponentialScorer,Chess::rScoring* harmonicScorer);
		/*!
		 * Returns the total number of games the player has finished
		 * in the tournament.
		 */



		int gamesFinished() const;


	private:
		PlayerBuilder* m_builder;
		TimeControl m_timeControl;
		const OpeningBook* m_book;
		int m_bookDepth;

		int m_Objectives[876];
		int m_whiteObjectives[876];
		int m_games;
		int m_whiteGames;

		int m_classicalWins;
		int m_classicalLosses;

		int m_whiteClassicalWins;
		int m_whiteClassicalLosses;

		int m_rMobWins;
		int m_rMobLosses;

		int m_whiteRMobWins;
		int m_whiteRMobLosses;

		int m_komiWins;
		int m_komiLosses;

		int m_whiteKomiWins;
		int m_whiteKomiLosses;

		qreal m_exponentialPoints;
		qreal m_whiteExponentialPoints;

		qreal m_squareExponentialPoints;
		qreal m_whiteSquareExponentialPoints;

		qreal m_harmonicPoints;
		qreal m_whiteHarmonicPoints;

		qreal m_squareHarmonicPoints;
		qreal m_whiteSquareHarmonicPoints;

};

#endif // TOURNAMENTPLAYER_H
