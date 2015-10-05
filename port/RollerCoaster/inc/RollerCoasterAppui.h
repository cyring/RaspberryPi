/*
* ============================================================================
*  Name     : CRollerCoasterAppUi from RollerCoasterAppui.h
*  Part of  : RollerCoaster
*  Created  : 28.12.2004 by Plusplus
*  Description:
*     Declares UI class for application.
*  Version  :
*  Copyright: 2004 Plusplus (plusplus AT free.fr)
* ============================================================================
*/

#ifndef ROLLERCOASTERAPPUI_H
#define ROLLERCOASTERAPPUI_H

// INCLUDES
#include <aknappui.h>

// FORWARD DECLARATIONS
class CRollerCoasterContainer;


// CLASS DECLARATION

/**
* Application UI class.
* Provides support for the following features:
* - EIKON control architecture
* 
*/
class CRollerCoasterAppUi : public CAknAppUi
    {
    public: // // Constructors and destructor

        /**
        * EPOC default constructor.
        */      
        void ConstructL();

        /**
        * Destructor.
        */      
        ~CRollerCoasterAppUi();
        
    public: // New functions

    public: // Functions from base classes

    private:
        // From MEikMenuObserver
        void DynInitMenuPaneL(TInt aResourceId,CEikMenuPane* aMenuPane);

    private:
        /**
        * From CEikAppUi, takes care of command handling.
        * @param aCommand command to be handled
        */
        void HandleCommandL(TInt aCommand);

        /**
        * From CEikAppUi, handles key events.
        * @param aKeyEvent Event to handled.
        * @param aType Type of the key event. 
        * @return Response code (EKeyWasConsumed, EKeyWasNotConsumed). 
        */
        virtual TKeyResponse HandleKeyEventL(
            const TKeyEvent& aKeyEvent,TEventCode aType);

    private: //Data
        CRollerCoasterContainer* iAppContainer; 
    };

#endif

// End of File
