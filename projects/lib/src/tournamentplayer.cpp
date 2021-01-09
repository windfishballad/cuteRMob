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

#include "tournamentplayer.h"
#include <cmath>


TournamentPlayer::TournamentPlayer(PlayerBuilder* builder,
				   const TimeControl& timeControl,
				   const OpeningBook* book,
				   int bookDepth)
	: m_builder(builder),
	  m_timeControl(timeControl),
	  m_book(book),
	  m_bookDepth(bookDepth),
	  m_games(0),
	  m_whiteGames(0),
	  m_classicalWins(0),
	  m_classicalLosses(0),
	  m_whiteClassicalWins(0),
	  m_whiteClassicalLosses(0),
	  m_rMobWins(0),
	  m_rMobLosses(0),
	  m_whiteRMobWins(0),
	  m_whiteRMobLosses(0),
	  m_komiWins(0),
	  m_komiLosses(0),
	  m_whiteKomiWins(0),
	  m_whiteKomiLosses(0),
	  m_exponentialPoints(0),
	  m_whiteExponentialPoints(0),
	  m_squareExponentialPoints(0),
	  m_whiteSquareExponentialPoints(0),
	  m_harmonicPoints(0),
	  m_whiteHarmonicPoints(0),
	  m_squareHarmonicPoints(0),
	  m_whiteSquareHarmonicPoints(0)
{
	Q_ASSERT(builder != nullptr);
	for(int i=0;i<876;i++)
	{
		m_Objectives[i]=0;
		m_whiteObjectives[i]=0;
	}
}

const PlayerBuilder* TournamentPlayer::builder() const
{
	return m_builder;
}

QString TournamentPlayer::name() const
{
	return m_builder->name();
}

void TournamentPlayer::setName(const QString& name)
{
	if (m_builder)
		m_builder->setName(name);
}

const TimeControl& TournamentPlayer::timeControl() const
{
	return m_timeControl;
}

const OpeningBook* TournamentPlayer::book() const
{
	return m_book;
}

int TournamentPlayer::bookDepth() const
{
	return m_bookDepth;
}

void TournamentPlayer::addScore(Chess::Side side, Chess::rMobResult gResult, Chess::rMobKomi komi, int gCutoff, Chess::rScoring* exponentialScorer, Chess::rScoring* harmonicScorer)
{

	if (side == Chess::Side::NoSide)
		Q_UNREACHABLE();

	++m_games;
	if(side == gResult.gSide)
	{
		++m_Objectives[gResult.gScore];
		if(gResult.gScore<gCutoff) ++m_rMobWins;
		if(gResult.gScore==0) ++m_classicalWins;
		m_exponentialPoints+=exponentialScorer->rValue(gResult.gScore);
		m_harmonicPoints+=harmonicScorer->rValue(gResult.gScore);

		m_squareExponentialPoints+=std::pow(exponentialScorer->rValue(gResult.gScore),2.0);
		m_squareHarmonicPoints+=std::pow(harmonicScorer->rValue(gResult.gScore),2.0);

		if(gResult.gSide==komi.gSide)
		{
			if(2*std::min(gResult.gScore,gCutoff)<komi.komi)
			{
				++m_komiWins;
				if(side==Chess::Side::White) ++m_whiteKomiWins;
			}
			else if(2*std::min(gResult.gScore,gCutoff)>komi.komi)
			{
				++m_komiLosses;
				if(side==Chess::Side::White) ++m_whiteKomiLosses;
			}
		}
		else
		{
			if(komi.komi<2*gCutoff)
			{
				++m_komiWins;
				if(side==Chess::Side::White) ++m_whiteKomiWins;
			}
			else if(komi.komi==2*gCutoff)
			{
				if(gResult.gScore<gCutoff)
				{
					++m_komiWins;
					if(side==Chess::Side::White) ++m_whiteKomiWins;
				}
			}
			else
			{
				if(gResult.gScore<gCutoff)
				{
					++m_komiWins;
					if(side==Chess::Side::White) ++m_whiteKomiWins;
				}
				else
				{
					++m_komiLosses;
					if(side==Chess::Side::White) ++m_whiteKomiLosses;
				}
			}
		}


		if(side==Chess::Side::White)
		{
			++m_whiteGames;
			++m_whiteObjectives[gResult.gScore];
			if(gResult.gScore==0) ++m_whiteClassicalWins;
			m_whiteExponentialPoints+=exponentialScorer->rValue(gResult.gScore);
			m_whiteSquareExponentialPoints+=std::pow(exponentialScorer->rValue(gResult.gScore),2.0);

			m_whiteHarmonicPoints+=harmonicScorer->rValue(gResult.gScore);
			m_whiteSquareHarmonicPoints+=std::pow(harmonicScorer->rValue(gResult.gScore),2.0);

			if(gResult.gScore<gCutoff) ++m_whiteRMobWins;
		}
	}
	else
	{
		++m_Objectives[875-gResult.gScore];
		if(gResult.gScore<gCutoff) ++m_rMobLosses;
		if(gResult.gScore==0) ++m_classicalLosses;
		m_exponentialPoints+=1.0-exponentialScorer->rValue(gResult.gScore);
		m_harmonicPoints+=1.0-harmonicScorer->rValue(gResult.gScore);

		m_squareExponentialPoints+=std::pow(1.0-exponentialScorer->rValue(gResult.gScore),2.0);
		m_squareHarmonicPoints+=std::pow(1.0-harmonicScorer->rValue(gResult.gScore),2.0);

		if(gResult.gSide==komi.gSide)
		{
			if(2*std::min(gResult.gScore,gCutoff)<komi.komi)
			{
				++m_komiLosses;
				if(side==Chess::Side::White) ++m_whiteKomiLosses;
			}
			else if(2*std::min(gResult.gScore,gCutoff)>komi.komi)
			{
				++m_komiWins;
				if(side==Chess::Side::White) ++m_whiteKomiWins;
			}
		}
		else
		{
			if(komi.komi<2*gCutoff)
			{
				++m_komiLosses;
				if(side==Chess::Side::White) ++m_whiteKomiLosses;
			}
			else if(komi.komi==2*gCutoff)
			{
				if(gResult.gScore<gCutoff)
				{
					++m_komiLosses;
					if(side==Chess::Side::White) ++m_whiteKomiLosses;
				}
			}
			else
			{
				if(gResult.gScore<gCutoff)
				{
					++m_komiLosses;
					if(side==Chess::Side::White) ++m_whiteKomiLosses;
				}
				else
				{
					++m_komiWins;
					if(side==Chess::Side::White) ++m_whiteKomiWins;
				}
			}
		}
		if(side==Chess::Side::White)
		{
			++m_whiteGames;
			++m_whiteObjectives[875-gResult.gScore];
			if(gResult.gScore==0) ++m_whiteClassicalLosses;
			if(gResult.gScore<gCutoff) ++m_whiteRMobLosses;

			m_whiteExponentialPoints+=1.0-exponentialScorer->rValue(gResult.gScore);
			m_whiteSquareExponentialPoints+=std::pow(1.0-exponentialScorer->rValue(gResult.gScore),2.0);

			m_whiteHarmonicPoints+=1.0-harmonicScorer->rValue(gResult.gScore);
			m_whiteSquareHarmonicPoints+=std::pow(1.0-harmonicScorer->rValue(gResult.gScore),2.0);

		}
	}

}



template<Chess::rMobScoring rMobType>
qreal TournamentPlayer::points() const
{
	switch(rMobType)
	{
	case Chess::Exponential:
		return m_exponentialPoints;

	case Chess::Harmonic:
		return m_harmonicPoints;

	default:
		return qreal(wins<rMobType>())+0.5*qreal(draws<rMobType>());
	}
}

template qreal TournamentPlayer::points<Chess::Classical>() const;
template qreal TournamentPlayer::points<Chess::Exponential>() const;
template qreal TournamentPlayer::points<Chess::Harmonic>() const;
template qreal TournamentPlayer::points<Chess::AllOrNone>() const;
template qreal TournamentPlayer::points<Chess::Komi>() const;

template<Chess::rMobScoring rMobType>
qreal TournamentPlayer::squarePoints() const
{
	switch(rMobType)
	{
	case Chess::Exponential:
		return m_squareExponentialPoints;

	case Chess::Harmonic:
		return m_squareHarmonicPoints;

	default:
		return qreal(wins<rMobType>())+0.25*draws<rMobType>();
	}
}

template qreal TournamentPlayer::squarePoints<Chess::Classical>() const;
template qreal TournamentPlayer::squarePoints<Chess::Exponential>() const;
template qreal TournamentPlayer::squarePoints<Chess::Harmonic>() const;
template qreal TournamentPlayer::squarePoints<Chess::AllOrNone>() const;
template qreal TournamentPlayer::squarePoints<Chess::Komi>() const;


template<Chess::rMobScoring rMobType>
qreal TournamentPlayer::whitePoints() const
{
	switch(rMobType)
	{
	case Chess::Exponential:
		return m_whiteExponentialPoints;

	case Chess::Harmonic:
		return m_whiteHarmonicPoints;

	default:
		return qreal(whiteWins<rMobType>())+0.5*whiteDraws<rMobType>();
	}
}

template qreal TournamentPlayer::whitePoints<Chess::Classical>() const;
template qreal TournamentPlayer::whitePoints<Chess::Exponential>() const;
template qreal TournamentPlayer::whitePoints<Chess::Harmonic>() const;
template qreal TournamentPlayer::whitePoints<Chess::AllOrNone>() const;
template qreal TournamentPlayer::whitePoints<Chess::Komi>() const;

template<Chess::rMobScoring rMobType>
qreal TournamentPlayer::whiteSquarePoints() const
{
	switch(rMobType)
	{
	case Chess::Exponential:
		return m_whiteSquareExponentialPoints;

	case Chess::Harmonic:
		return m_whiteSquareHarmonicPoints;

	default:
		return qreal(whiteWins<rMobType>())+0.25*whiteDraws<rMobType>();
	}
}

template qreal TournamentPlayer::whiteSquarePoints<Chess::Classical>() const;
template qreal TournamentPlayer::whiteSquarePoints<Chess::Exponential>() const;
template qreal TournamentPlayer::whiteSquarePoints<Chess::Harmonic>() const;
template qreal TournamentPlayer::whiteSquarePoints<Chess::AllOrNone>() const;
template qreal TournamentPlayer::whiteSquarePoints<Chess::Komi>() const;

template<Chess::rMobScoring rMobType>
qreal TournamentPlayer::blackPoints() const
{
	return points<rMobType>()-whitePoints<rMobType>();
}

template qreal TournamentPlayer::blackPoints<Chess::Classical>() const;
template qreal TournamentPlayer::blackPoints<Chess::Exponential>() const;
template qreal TournamentPlayer::blackPoints<Chess::Harmonic>() const;
template qreal TournamentPlayer::blackPoints<Chess::AllOrNone>() const;
template qreal TournamentPlayer::blackPoints<Chess::Komi>() const;

template<Chess::rMobScoring rMobType>
qreal TournamentPlayer::blackSquarePoints() const
{
	return squarePoints<rMobType>()-whiteSquarePoints<rMobType>();
}

template qreal TournamentPlayer::blackSquarePoints<Chess::Classical>() const;
template qreal TournamentPlayer::blackSquarePoints<Chess::Exponential>() const;
template qreal TournamentPlayer::blackSquarePoints<Chess::Harmonic>() const;
template qreal TournamentPlayer::blackSquarePoints<Chess::AllOrNone>() const;
template qreal TournamentPlayer::blackSquarePoints<Chess::Komi>() const;

int TournamentPlayer::gamesFinished() const
{
	return m_games;
}

template<Chess::rMobScoring rMobType>
int TournamentPlayer::wins() const
{
	return m_rMobWins;
}

template<Chess::rMobScoring rMobType>
int TournamentPlayer::losses() const
{
	return m_rMobLosses;
}

template<Chess::rMobScoring rMobType>
int TournamentPlayer::whiteWins() const
{
	return m_whiteRMobWins;
}

template<Chess::rMobScoring rMobType>
int TournamentPlayer::whiteLosses() const
{
	return m_whiteRMobLosses;
}

template int TournamentPlayer::wins<Chess::Exponential>() const;
template int TournamentPlayer::wins<Chess::Harmonic>() const;
template int TournamentPlayer::wins<Chess::AllOrNone>() const;

template int TournamentPlayer::losses<Chess::Exponential>() const;
template int TournamentPlayer::losses<Chess::Harmonic>() const;
template int TournamentPlayer::losses<Chess::AllOrNone>() const;

template int TournamentPlayer::whiteWins<Chess::Exponential>() const;
template int TournamentPlayer::whiteWins<Chess::Harmonic>() const;
template int TournamentPlayer::whiteWins<Chess::AllOrNone>() const;

template int TournamentPlayer::whiteLosses<Chess::Exponential>() const;
template int TournamentPlayer::whiteLosses<Chess::Harmonic>() const;
template int TournamentPlayer::whiteLosses<Chess::AllOrNone>() const;

template<>
int TournamentPlayer::wins<Chess::Classical>() const
{
	return m_classicalWins;
}

template<>
int TournamentPlayer::losses<Chess::Classical>() const
{
	return m_classicalLosses;
}

template<>
int TournamentPlayer::whiteWins<Chess::Classical>() const
{
	return m_whiteClassicalWins;
}

template<>
int TournamentPlayer::whiteLosses<Chess::Classical>() const
{
	return m_whiteClassicalLosses;
}

template<>
int TournamentPlayer::wins<Chess::Komi>() const
{
	return m_komiWins;
}

template<>
int TournamentPlayer::losses<Chess::Komi>() const
{
	return m_komiLosses;
}


template<>
int TournamentPlayer::whiteWins<Chess::Komi>() const
{
	return m_whiteKomiWins;
}

template<>
int TournamentPlayer::whiteLosses<Chess::Komi>() const
{
	return m_whiteKomiLosses;
}

template<Chess::rMobScoring rMobType>
int TournamentPlayer::draws() const
{
	return m_games-wins<rMobType>()-losses<rMobType>();
}

template int TournamentPlayer::draws<Chess::Classical>() const;
template int TournamentPlayer::draws<Chess::Exponential>() const;
template int TournamentPlayer::draws<Chess::Harmonic>() const;
template int TournamentPlayer::draws<Chess::AllOrNone>() const;
template int TournamentPlayer::draws<Chess::Komi>() const;

template<Chess::rMobScoring rMobType>
int TournamentPlayer::blackWins() const
{
	return wins<rMobType>()-whiteWins<rMobType>();
}

template int TournamentPlayer::blackWins<Chess::Classical>() const;
template int TournamentPlayer::blackWins<Chess::Exponential>() const;
template int TournamentPlayer::blackWins<Chess::Harmonic>() const;
template int TournamentPlayer::blackWins<Chess::AllOrNone>() const;
template int TournamentPlayer::blackWins<Chess::Komi>() const;

template<Chess::rMobScoring rMobType>
int TournamentPlayer::blackLosses() const
{
	return losses<rMobType>()-whiteLosses<rMobType>();
}

template int TournamentPlayer::blackLosses<Chess::Classical>() const;
template int TournamentPlayer::blackLosses<Chess::Exponential>() const;
template int TournamentPlayer::blackLosses<Chess::Harmonic>() const;
template int TournamentPlayer::blackLosses<Chess::AllOrNone>() const;
template int TournamentPlayer::blackLosses<Chess::Komi>() const;


template<Chess::rMobScoring rMobType>
int TournamentPlayer::whiteDraws() const
{
	return m_whiteGames-whiteWins<rMobType>()-whiteLosses<rMobType>();
}

template int TournamentPlayer::whiteDraws<Chess::Classical>() const;
template int TournamentPlayer::whiteDraws<Chess::Exponential>() const;
template int TournamentPlayer::whiteDraws<Chess::Harmonic>() const;
template int TournamentPlayer::whiteDraws<Chess::AllOrNone>() const;
template int TournamentPlayer::whiteDraws<Chess::Komi>() const;

template<Chess::rMobScoring rMobType>
int TournamentPlayer::blackDraws() const
{
	return draws<rMobType>()-whiteDraws<rMobType>();
}

template int TournamentPlayer::blackDraws<Chess::Classical>() const;
template int TournamentPlayer::blackDraws<Chess::Exponential>() const;
template int TournamentPlayer::blackDraws<Chess::Harmonic>() const;
template int TournamentPlayer::blackDraws<Chess::AllOrNone>() const;
template int TournamentPlayer::blackDraws<Chess::Komi>() const;

int TournamentPlayer::nWhiteGames() const
{
	return m_whiteGames;
}

int TournamentPlayer::objectives(int i) const
{
	return m_Objectives[i];
}

int TournamentPlayer::whiteObjectives(int i) const
{
	return m_whiteObjectives[i];
}

int TournamentPlayer::blackObjectives(int i) const
{
	return m_Objectives[i]-m_whiteObjectives[i];
}

