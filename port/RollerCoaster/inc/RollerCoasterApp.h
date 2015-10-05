/*
* ============================================================================
*  Name     : CRollerCoasterApp from RollerCoasterApp.h
*  Part of  : RollerCoaster
*  Created  : 28.12.2004 by Plusplus
*  Description:
*     Declares main application class.
*  Version  :
*  Copyright: 2004 Plusplus (plusplus AT free.fr)
* ============================================================================
*/

#ifndef ROLLERCOASTERAPP_H
#define ROLLERCOASTERAPP_H

// INCLUDES
#include <aknapp.h>

// CONSTANTS
// UID of the application
const TUid KUidRollerCoaster = { 0x06EE0477 };

// CLASS DECLARATION

/**
* CRollerCoasterApp application class.
* Provides factory to create concrete document object.
* 
*/
class CRollerCoasterApp : public CAknApplication
    {
    
    public: // Functions from base classes
    private:

        /**
        * From CApaApplication, creates CRollerCoasterDocument document object.
        * @return A pointer to the created document object.
        */
        CApaDocument* CreateDocumentL();
        
        /**
        * From CApaApplication, returns application's UID (KUidRollerCoaster).
        * @return The value of KUidRollerCoaster.
        */
        TUid AppDllUid() const;
    };

#endif

// End of File

