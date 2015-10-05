/*
 *	RollerCoaster2005
 *	Copyright (C) 2005 Plusplus (plusplus AT free.fr)
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program; if not, write to the Free Software
 *	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "RollerCoaster.h"

CRollerCoaster::CRollerCoaster(TUint aWidth, TUint aHeight)
: iScreenWidth(aWidth), iScreenHeight(aHeight)
{
}


void CRollerCoaster::ConstructL()
{
}


CRollerCoaster* CRollerCoaster::NewL(TUint aWidth, TUint aHeight)
{
	CRollerCoaster* self = new CRollerCoaster(aWidth, aHeight);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
}


CRollerCoaster::~CRollerCoaster()
{
}


void CRollerCoaster::AppInit()
{
	static const point array[] = {
		{4.0f, -2.0f, 0.2f}, {0.0f, 0.0f, 0.0f},
		{3.0f, 0.0f, 0.21f}, {-0.5f, 1.0f, 0.0f},
		{2.0f, 2.0f, 3.5f}, {0.0f, 0.0f, 0.0f},
		{0.0f, 3.0f, 3.4f}, {0.0f, 0.0f, 0.0f},
		{-3.0f, 0.0f, 0.2f}, {0.0f, 0.0f, 0.0f},
		{-3.0f, -2.0f, 2.5f}, {0.0f, 0.0f, 0.0f},
		{0.0f, -3.0f, 0.2f}, {1.0f, 0.0f, 0.0f},
		{1.0f, -3.1f, 1.2f}, {0.0f, 0.0f, 0.0f},
		{0.0f, -3.2f, 2.2f}, {0.0f, 0.0f, 0.0f},
		{-1.0f, -3.3f, 1.2f}, {0.0f, 0.0f, 0.0f},
		{0.0f, -3.4f, 0.2f}, {1.0f, 0.0f, 0.0f},
		{0.5f, -3.4f, 0.2f}, {1.0f, 0.0f, 0.0f},
		{1.5f, -3.5f, 1.2f}, {0.0f, 0.0f, 0.0f},
		{0.5f, -3.6f, 2.2f}, {0.0f, 0.0f, 0.0f},
		{-0.5f, -3.7f, 1.2f}, {0.0f, 0.0f, 0.0f},
		{0.5f, -3.8f, 0.2f}, {1.0f, 0.0f, 0.0f},
		{4.0f, -4.0f, 2.5f}, {0.0f, 0.0f, 0.0f},
		{4.0f, -1.0f, 3.0f}, {0.0f, 0.0f, 0.0f},
		{2.0f, 2.0f, 1.0f}, {0.0f, 0.0f, 0.0f},
		{-3.0f, -2.0f, 1.0f}, {0.0f, 0.0f, 0.0f},
		{-3.0f, 3.0f, 3.3f}, {0.0f, 0.0f, 0.0f},
		{0.0f, 3.3f, 0.7f}, {0.0f, 0.0f, 0.0f},
		{2.0f, 3.5f, 0.2f}, {0.0f, 0.0f, 0.0f},
		{3.5f, 3.4f, 1.7f}, {0.0f, 0.0f, 0.0f},
		{2.0f, 3.3f, 3.2f}, {0.0f, 0.0f, 0.0f},
		{0.5f, 3.2f, 1.7f}, {0.0f, 0.0f, 0.0f},
		{2.0f, 3.1f, 0.3f}, {1.0f, 0.0f, 0.0f},
		{4.0f, 2.0f, 0.3f}, {0.0f, 0.0f, 0.0f},
		{3.0f, 0.0f, 2.0f}, {0.0f, 0.0f, 0.0f},
		{0.0f, -1.0f, 3.0f}, {0.0f, 0.0f, 0.0f},
		{-3.0f, -3.0f, 1.0f}, {0.0f, 0.0f, 0.0f},
		{0.0f, -5.0f, 0.2f}, {4.0f, -0.5f, 0.0f},
		{5.0f, -4.0f, 0.2f}, {-0.5f, 1.0f, 0.0f}
	};
	iRollerCoaster.trkd.nbControlPoint = 33;
	iRollerCoaster.trkd.control = (point*)array;
	iRollerCoaster.trkd.startSegment = 2;
	iRollerCoaster.trkd.brakeSegment = -12;
	iRollerCoaster.trkd.twistFactor = 1.5f;
	iRollerCoaster.trkd.averageSegmentLength = 0.35f;

	iStartTime.UniversalTime();
	iRollerCoaster.eng.doIntro = 1;
	iRollerCoaster.eng.wndWidth = iScreenWidth;
	iRollerCoaster.eng.wndHeight = iScreenHeight;

	static const point coord[] = {
		{0.98f, -3.05f, 1.0f}, {1.0f, 0.0f, 0.0f},
		{0.0f, -3.0f, 1.0f}, {1.0f, 0.0f, 0.0f},
		{0.25f, -3.4f, 1.0f}, {1.0f, 0.0f, 0.0f},
		{0.5f, -3.8f, 1.0f}, {1.0f, 0.0f, 0.0f},
		{4.44f, -2.88f, 0.0f}, {-0.5f, 1.0f, -0.18f},
		{2.0f, 3.5f, 1.0f}, {1.0f, 0.0f, 0.0f},
		{3.4f, 3.4f, 0.0f}, {1.0f, 0.0f, 0.0f},
		{1.0f, 3.05f, 2.0f}, {1.0f, 0.0f, 0.0f},
		{2.5f, 3.05f, 1.0f}, {1.0f, 0.0f, 0.0f}
	};
	iRollerCoaster.supd.pillar_coord = (point*)coord;
	iRollerCoaster.supd.nbPillarCoord = 9;
	static const int absc[] = {
		9,
		15,
		22,
		30,
		40,
		50,
		60,
		115, //after double loop
		120,
		135,
		140,
		150,
		155,
		165,
		180,
		252,
		258,
		265,
		275,
		280,
		285,
		290,
		295
	};
	iRollerCoaster.supd.pillar_absc = (int*)absc;
	iRollerCoaster.supd.nbPillarAbsc = 23;

	InitializeRoller(&iRollerCoaster, "");
}


void CRollerCoaster::AppExit()
{
	ExitRoller(&iRollerCoaster);
}


void CRollerCoaster::AppCycle(TInt /*aFrame*/)
{
	TTime curTime;
	curTime.UniversalTime();
	unsigned long timeTick = (curTime.MicroSecondsFrom(iStartTime).Int64() / 1000).Low();
	DrawRoller(timeTick, &iRollerCoaster);
}


void CRollerCoaster::FlatShading()
{
	SetRollerShadeMode(&iRollerCoaster, ROLLER_SHADE_FLAT);
}


void CRollerCoaster::SmoothShading()
{
	SetRollerShadeMode(&iRollerCoaster, ROLLER_SHADE_SMOOTH);
}


void CRollerCoaster::SetPerspectiveCorrection(bool aPerspective)
{
	SetRollerPerspectiveCorrection(&iRollerCoaster, aPerspective);
}


bool CRollerCoaster::GetPerspectiveCorrection()
{
	return iRollerCoaster.eng.perspective_correction ? TRUE : FALSE;
}


void CRollerCoaster::SetMipmapping(bool aMipmapping)
{
	SetRollerMipmapping(&iRollerCoaster, aMipmapping);
}


bool CRollerCoaster::GetMipmapping()
{
	return iRollerCoaster.eng.mipmapping ? TRUE : FALSE;
}
