/*
* ============================================================================
*  Name     : CRollerCoasterDocument from RollerCoasterDocument.h
*  Part of  : RollerCoaster
*  Created  : 28.12.2004 by Plusplus
*  Description:
*     Declares document for application.
*  Version  :
*  Copyright: 2004 Plusplus (plusplus AT free.fr)
* ============================================================================
*/

#ifndef ROLLERCOASTERDOCUMENT_H
#define ROLLERCOASTERDOCUMENT_H

// INCLUDES
#include <akndoc.h>
   
// CONSTANTS

// FORWARD DECLARATIONS
class  CEikAppUi;

// CLASS DECLARATION

/**
*  CRollerCoasterDocument application class.
*/
class CRollerCoasterDocument : public CAknDocument
    {
    public: // Constructors and destructor
        /**
        * Two-phased constructor.
        */
        static CRollerCoasterDocument* NewL(CEikApplication& aApp);

        /**
        * Destructor.
        */
        virtual ~CRollerCoasterDocument();

    public: // New functions

    public: // Functions from base classes
    protected:  // New functions

    protected:  // Functions from base classes

    private:

        /**
        * EPOC default constructor.
        */
        CRollerCoasterDocument(CEikApplication& aApp);
        void ConstructL();

    private:

        /**
        * From CEikDocument, create CRollerCoasterAppUi "App UI" object.
        */
        CEikAppUi* CreateAppUiL();
    };

#endif

// End of File

