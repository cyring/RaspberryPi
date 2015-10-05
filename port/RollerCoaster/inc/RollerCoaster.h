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

#ifndef _ROLLERCOASTER_H_
#define _ROLLERCOASTER_H_

// INCLUDES
#include <e32base.h>
#include "roller.h"

class CRollerCoaster : public CBase
{
	public:
		static CRollerCoaster* NewL(TUint, TUint);
		virtual ~CRollerCoaster();

		void AppInit();
		void AppExit();
		void AppCycle(TInt aFrame);

		void FlatShading();
		void SmoothShading();
		void CRollerCoaster::SetPerspectiveCorrection(bool aPerspective);
		bool CRollerCoaster::GetPerspectiveCorrection();
		void CRollerCoaster::SetMipmapping(bool aMipmapping);
		bool CRollerCoaster::GetMipmapping();

	protected:
		CRollerCoaster(TUint, TUint);
		void ConstructL();

	private:
		TUint iScreenWidth;
		TUint iScreenHeight;
		TTime iStartTime;
		struct rollercoaster iRollerCoaster;
};

#endif
