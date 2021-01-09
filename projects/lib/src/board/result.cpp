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

#include "result.h"
#include <cmath>

extern const int defaultCutoff=12;
extern const Chess::rMobKomi defaultKomi={23, Chess::Side::White};
extern const Chess::rMobResult initialResult={436,Chess::Side::White};

namespace Chess {



rScoring::rScoring(rMobScoring type)
{
	if(type == Exponential)
	{
		qreal addOn=0.5;
		for(int i=0;i<438;i++)
		{
			m_rValue[i]=0.5+addOn;
			addOn=addOn*0.5;
		}

	}
	else if(type == Harmonic)
	{
		for(int i=0;i<438;i++)
		{
			m_rValue[i]=0.5+0.5/(1+i);
		}
	}

}


qreal rScoring::rValue(int gScore)
{
	return m_rValue[gScore];
}

Result::Result(Type type, Side winner, rMobResult gResult, const QString& description)
	: m_type(type),
	  m_winner(winner),
	  m_gResult(gResult),
	  m_description(description)
{
}

Result::Result(Type type, Side winner, const QString& description)
	: m_type(type),
	  m_winner(winner),
	  m_description(description)
{
	m_gResult=rMobResult{0,winner};
}
Result::Result(rMobResult gResult, const QString& description)
	: m_gResult(gResult),
	  m_description(description)
{
	if(gResult.gScore==0)
	{
		m_type=Win;
		m_winner=gResult.gSide;
	}
	else
	{
		m_type=Draw;
		m_winner=Chess::Side::NoSide;
	}
}

Result::Result(const QString& str)
	: m_type(ResultError)
{
	if (str.startsWith("1-0"))
	{
		m_type = Win;
		m_winner = Side::White;
	}
	else if (str.startsWith("0-1"))
	{
		m_type = Win;
		m_winner = Side::Black;
	}
	else if (str.startsWith("1/2-1/2"))
		m_type = Draw;
	else if (str.startsWith("*"))
		m_type = NoResult;

	int start = str.indexOf('{');
	int end = str.lastIndexOf('}');
	if (start != -1 && end != -1)
		m_description = str.mid(start + 1, end - start - 1);
}

bool Result::operator==(const Result& other) const
{
	return (m_type == other.m_type &&
		m_winner == other.m_winner &&
		m_description == other.m_description);
}

bool Result::operator!=(const Result& other) const
{
	return (m_type != other.m_type ||
		m_winner != other.m_winner ||
		m_description != other.m_description);
}

bool Result::isNone() const
{
	return m_type == NoResult;
}

bool Result::isDraw() const
{
	return (m_winner.isNull() &&
		m_type != NoResult &&
		m_type != ResultError);
}

Side Result::winner() const
{
	return m_winner;
}

Side Result::loser() const
{
	if (m_winner.isNull())
		return Side::NoSide;
	return m_winner.opposite();
}

Result::Type Result::type() const
{
	return m_type;
}

QString Result::description() const
{
	QString w(winner().toString());
	QString l(loser().toString());
	QString str;

	if (m_type == Resignation)
		str = tr("%1 resigns").arg(l);
	else if (m_type == Timeout)
	{
		if (l.isEmpty())
			str = tr("Draw by timeout");
		else
			str = tr("%1 loses on time").arg(l);
	}
	else if (m_type == Adjudication)
	{
		if (w.isEmpty())
			str = tr("Draw by adjudication");
		else
			str = tr("%1 wins by adjudication").arg(w);
	}
	else if (m_type == IllegalMove)
		str = tr("%1 makes an illegal move").arg(l);
	else if (m_type == Disconnection)
	{
		if (l.isEmpty())
			str = tr("Draw by disconnection");
		else
			str = tr("%1 disconnects").arg(l);
	}
	else if (m_type == StalledConnection)
	{
		if (l.isEmpty())
			str = tr("Draw by stalled connection");
		else
			str = tr("%1's connection stalls").arg(l);
	}
	else if (m_type == Agreement)
	{
		if (w.isEmpty())
			str = tr("Draw by agreement");
		else
			str = tr("%1 wins by agreement").arg(w);
	}
	else if (m_type == NoResult)
		str = tr("No result");
	else if (m_type == ResultError)
		str = tr("Result error");

	if (m_description.isEmpty())
	{
		if (m_type == Win)
			str = tr("%1 wins").arg(w);
		else if (m_type == Draw)
			str = tr("Drawn game");
	}
	else
	{
		if (!str.isEmpty())
			str += ": ";
		str += m_description;
	}

	Q_ASSERT(!str.isEmpty());
	str[0] = str.at(0).toUpper();
	return str;
}

rMobResult Result::gResult() const
{
	return m_gResult;
}



QString Result::toShortString() const
{
	if (m_type == NoResult || m_type == ResultError)
		return "*";

	QString res;

	if (m_gResult.gSide == Side::White)
	{
		res=tr("G%1.%2").arg(m_gResult.gScore/2).arg(5*(m_gResult.gScore%2));
	}
	else if(m_gResult.gSide== Side::Black)
	{
		res=tr("-G%1.%2").arg(m_gResult.gScore/2).arg(5*(m_gResult.gScore%2));
	}
	else
	{
		res=tr("*");
	}

	return res;
}

QString Result::toLegacyString() const
{
	if (m_type == NoResult || m_type == ResultError)
		return "*";


	if(m_type==Win)
	{
		return (m_winner==Chess::Side::White) ? "1-0" : "0-1";
	}

	return "1/2-1/2";
}


QString Result::toVerboseString() const
{
	return toShortString() + QString(" {") + description() + "}";
}

rMobResult parseGValue(const QString& str)
{
	QStringList parts=str.split('.');
	Side::Type gSide;
	int gScore;
	if((parts.at(0).startsWith("G") || parts.at(0).startsWith("g")) && (parts.at(0).size()>=2))
	{
		gSide=Side::White;
		gScore=2*parts.at(0).mid(1).toInt();
		if(parts.size()>=2 && parts.at(1).size()>=1)
			gScore+=parts.at(1).left(1).toInt()/5;
	}
	else if((parts.at(0).startsWith("-G") || parts.at(0).startsWith("-g") || parts.at(0).startsWith("mG") || parts.at(0).startsWith("mg")) && parts.at(0).size()>=3)
	{
		gSide=Side::Black;
		gScore=2*parts.at(0).mid(2).toInt();
		if(parts.size()>=2 && parts.at(1).size()>=1)
			gScore+=parts.at(1).left(1).toInt()/5;
	}
	else
	{
		qWarning("Could not parse; defaulting to G218.5");
		gSide=Side::White;
		gScore=437;
	}
	return rMobResult{gScore,gSide};
}

rMobKomi parseKomi(const QString& str)
{
	Side::Type komiSide;
	int komi;
	QStringList parts=str.split('.');
	if((parts.at(0).startsWith("G") || parts.at(0).startsWith("g")) && parts.at(0).size()>=2)
	{
		komiSide=Side::White;
		komi=4*parts.at(0).mid(1).toInt();
		if(parts.size()>=2 && parts.at(1).size()>=1)
		{
			if(parts.at(1).left(1)=="2") komi+=1;
			else if(parts.at(1).left(1)=="5") komi+=2;
			else if(parts.at(1).left(1)=="7") komi+=3;
		}
	}
	else if((parts.at(0).startsWith("-G") || parts.at(0).startsWith("-g") || parts.at(0).startsWith("mG") || parts.at(0).startsWith("mg")) && parts.at(0).size()>=3)
	{
		komiSide=Side::Black;
		komi=4*parts.at(0).mid(2).toInt();
		if(parts.size()>=2 && parts.at(1).size() >= 1)
		{
			if(parts.at(1).left(1)=="2") komi+=1;
			else if(parts.at(1).left(1)=="5") komi+=2;
			else if(parts.at(1).left(1)=="7") komi+=3;
		}
	}
	else
	{
		qWarning("Could not parse; defaulting to G5.75 komi");
		komiSide=Side::White;
		komi=23;
	}
	return rMobKomi{komi,komiSide};

}


int parseCutoff(const QString& str)
{
	return parseGValue(str).gScore;
}




QString komiToString(const rMobKomi& komi)
{
	QString str=(komi.gSide==Chess::Side::White ? "G" : "-G");
	str+=QString::number(komi.komi/4)+".";
	if(komi.komi%4 ==0) str+=QString::number(0);
	else if(komi.komi%4 ==1) str+=QString::number(25);
	else if(komi.komi%4 == 2) str+=QString::number(5);
	else str+=QString::number(75);

	return str;
}

QString gValueToString(const rMobResult& gResult)
{
	QString str=(gResult.gSide==Chess::Side::White ? "G" : "-G");
	str+=QString::number(gResult.gScore/2)+"."+QString::number((gResult.gScore%2)*5);

	return str;
}

QString gValueToString(int gScore)
{
	return gValueToString(rMobResult{gScore,Side::Type::White});
}

QString rMobScoringName(const rMobScoring& rMobType)
{
	switch(rMobType)
	{
	case Classical:
		return "Classical";

	case Harmonic:
		return "Harmonic";

	case AllOrNone:
		return "RMobility Winner";

	case Komi:
		return "Komi";

	default:
		return "Exponential";
	}
}



}
// namespace Chess
