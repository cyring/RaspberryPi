/*
* ============================================================================
*  Name     : CRollerCoasterContainer from RollerCoasterContainer.h
*  Part of  : RollerCoaster
*  Created  : 28.12.2004 by Plusplus
*  Description:
*     Declares container control for application.
*  Version  :
*  Copyright: 2004 Plusplus (plusplus AT free.fr)
* ============================================================================
*/

#ifndef ROLLERCOASTERCONTAINER_H
#define ROLLERCOASTERCONTAINER_H

// INCLUDES
#include <coecntrl.h>
#include "GLES/egl.h"
#include "RollerCoaster.h"

// FORWARD DECLARATIONS
class CEikLabel;        // for example labels

// CLASS DECLARATION

/**
*  CRollerCoasterContainer  container control class.
*
*/
class CRollerCoasterContainer : public CCoeControl, MCoeControlObserver
    {
    public: // Constructors and destructor

        /**
        * EPOC default constructor.
        * @param aRect Frame rectangle for container.
        */
        void ConstructL(const TRect& aRect);

        /**
        * Destructor.
        */
        ~CRollerCoasterContainer();

    public: // New functions

        static int DrawCallBack(TAny* aInstance);

    public: // Functions from base classes

    private: // Functions from base classes

       /**
        * From CoeControl,SizeChanged.
        */
        void SizeChanged();

       /**
        * From CoeControl,CountComponentControls.
        */
        TInt CountComponentControls() const;

       /**
        * From CCoeControl,ComponentControl.
        */
        CCoeControl* ComponentControl(TInt aIndex) const;

       /**
        * From CCoeControl,Draw.
        */
        void Draw(const TRect& aRect) const;


		/**
		* From MCoeControlObserver
		* Acts upon changes in the hosted control's state.
		*
		* @param	aControl	The control changing its state
		* @param	aEventType	The type of control event
		*/
        void HandleControlEventL(CCoeControl* aControl,TCoeEvent aEventType);

    private: //data

        // The display where the graphics are drawn
        EGLDisplay  iEglDisplay;
        // The rendering context
        EGLContext  iEglContext;
        // The window where the graphics are drawn
        EGLSurface  iEglSurface;
        // Active object for animation
        CPeriodic*  iPeriodic;
        // The generation state
        TBool  iIsGenerating;

    public: //data

        CRollerCoaster* iRollerCoaster;

    };

#endif

// End of File
