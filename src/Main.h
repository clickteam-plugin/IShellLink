/*
 * Copyright 2014 Olivier WENGER
 *     http://www.magicspark.org/ or http://oulydev.free.fr/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __MMF2EXT_ISHELLLINK_MAIN_H__
#define __MMF2EXT_ISHELLLINK_MAIN_H__

#include "shobjidl.h"
#include "shlguid.h"

// Object identifier
#define IDENTIFIER	                            (0x01AA0010)

// Object versions
#define	KCX_CURRENT_VERSION                     (1)

// ---------------------------
// DEFINITION OF ACTIONS CODES
// ---------------------------
#define ACT_CLEARLASTERRORCODE                  (0)

#define ACT_LOAD                                (1)
#define ACT_SAVETO                              (2)

#define ACT_RESOLVE                             (3)
#define ACT_SETARGS                             (4)
#define ACT_SETDESCRIPTION                      (5)
#define ACT_SETHOTKEY                           (6)
#define ACT_SETICONLOCATION                     (7)
#define ACT_SETPATH                             (8)
#define ACT_SETRELATIVEPATH                     (9)
#define ACT_SETSHOWCMD                          (10)
#define ACT_SETWORKINGDIRECTORY                 (11)

#define ACT_RESET                               (12)

#define	ACT_LAST					            (13)

// ------------------------------
// DEFINITION OF CONDITIONS CODES
// ------------------------------
#define CND_ONFAILURE                           (0)
#define CND_ONERROR                             (1)

#define CND_ISDIRTY                             (2)

#define	CND_LAST					            (3)

// -------------------------------
// DEFINITION OF EXPRESSIONS CODES
// -------------------------------
#define EXP_GETLASTERRORCODE                    (0)

#define EXP_GETARGS                             (1)
#define EXP_GETDESCRIPTION                      (2)
#define EXP_GETHOTKEY                           (3)
#define EXP_GETICONLOCATIONPATH                 (4)
#define EXP_GETICONLOCATIONINDEX                (5)
#define EXP_GETRAWPATH                          (6)
#define EXP_GETSHOWCMD                          (7)
#define EXP_GETWORKINGDIRECTORY                 (8)

#define	EXP_LAST                                (9)

// ---------------------
// OBJECT DATA STRUCTURE 
// ---------------------
// Used at edit time and saved in the MFA/CCN/EXE files

typedef struct tagEDATA_V1
{
    // Header - required
    extHeader eHeader;

    // Object's data
} EDITDATA;
typedef EDITDATA * LPEDATA;

// --------------------------------
// RUNNING OBJECT DATA STRUCTURE
// --------------------------------
// Used at runtime. Initialize it in the CreateRunObject function.
// Free any allocated memory or object in the DestroyRunObject function.
//
// Note: if you store C++ objects in this structure and do not store 
// them as pointers, you must call yourself their constructor in the
// CreateRunObject function and their destructor in the DestroyRunObject
// function. As the RUNDATA structure is a simple C structure and not a C++ object.

typedef struct tagRDATA
{
    // Main header - required
    headerObject    rHo;

    // Optional headers - depend on the OEFLAGS value, see documentation and examples for more info
//  rCom            rc; // Common structure for movements & animations
//  rMvt            rm; // Movements
//  rSpr            rs; // Sprite (displayable objects)
//  rVal            rv; // Alterable values

    // Object's runtime data
    HRESULT lastHResult;
    IShellLink* psl;
    BOOL reportLastHResult;

} RUNDATA;
typedef	RUNDATA	* LPRDATA;

#define MMF2EXT_EMASK_SEVEREFAILURE (0xC0000000)
#define MMF2EXT_EMASK_CUSTOMER (0x20000000)
#define MMF2EXT_E_SEVEREFAILURE (MMF2EXT_EMASK_SEVEREFAILURE|MMF2EXT_EMASK_CUSTOMER|E_UNEXPECTED)

#define MMF2EXT_IS_SEVERE_FAILURE(hr) \
    ( ( (hr) & MMF2EXT_EMASK_SEVEREFAILURE ) == MMF2EXT_EMASK_SEVEREFAILURE )

void MMF2EXT_onFailure( LPRDATA rdPtr );
void MMF2EXT_onError( LPRDATA rdPtr, HRESULT hResult, BOOL immediate=TRUE );

// Size when editing the object under level editor
// -----------------------------------------------
#define	MAX_EDITSIZE            (sizeof(EDITDATA))

// Default flags - see documentation for more info
// -------------
#define	OEFLAGS                 (OEFLAG_DISPLAYINFRONT)
#define	OEPREFS                 (0)

// If to handle message, specify the priority of the handling procedure
// 0= low, 255= very high. You should use 100 as normal.                                                
// --------------------------------------------------------------------
#define	WINDOWPROC_PRIORITY     (100)

#endif /* __MMF2EXT_ISHELLLINK_MAIN_H__ */

/* END OF FILE */
