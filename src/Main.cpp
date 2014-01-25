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

/*
 * TODO: resolving shell link may need MsiGetShortcutTarget() and
 *       MsiGetComponentPath().
 */

/*
 * TODO: using link's path may need to call ExpandEnvironmentStrings().
 *       It could be handy to provide it in this extension.
 */

// ============================================================================
//
// This file are where the Conditions/Actions/Expressions are defined.
// See the Extension FAQ in this SDK for more info and where to download it
//
// ============================================================================

// Common MMF2SDK Include
#include "common.h"

#include "windows.h"
#include "winnls.h"
#include "objbase.h"
#include "objidl.h"

void MMF2EXT_onFailure( LPRDATA rdPtr )
{
    if ( rdPtr != NULL )
    {
        rdPtr->lastHResult = MMF2EXT_E_SEVEREFAILURE;

        callRunTimeFunction(
            rdPtr,
            RFUNCTION_GENERATEEVENT,
            CND_ONFAILURE,
            0 );
    }
}

void MMF2EXT_onError( LPRDATA rdPtr, HRESULT hResult, BOOL immediate )
{
    if ( rdPtr != NULL )
    {
        /*
         * Note: don't erase severe failure error code.
         */
        if ( !MMF2EXT_IS_SEVERE_FAILURE( rdPtr->lastHResult ) )
        {
            rdPtr->lastHResult = IS_ERROR(hResult)
                ? hResult
                : E_UNEXPECTED;
            if ( immediate )
            {
                callRunTimeFunction(
                    rdPtr,
                    RFUNCTION_GENERATEEVENT,
                    MMF2EXT_IS_SEVERE_FAILURE( rdPtr->lastHResult  )
                        ? CND_ONFAILURE
                        : CND_ONERROR,
                    0 );
            }
            rdPtr->reportLastHResult = !immediate;
        }
    }
}

// ============================================================================
//
// ACTIONS ROUTINES
// 
// ============================================================================

short WINAPI DLLExport Action_clearLastHResult(
    LPRDATA rdPtr,
    long    param1,
    long    param2 )
{
    /*
     * Note: don't erase severe failure error code.
     */
    if ( rdPtr != NULL
        && !MMF2EXT_IS_SEVERE_FAILURE( rdPtr->lastHResult ) )
    {
        rdPtr->lastHResult = S_OK;
    }

    return 0;
}

short WINAPI DLLExport Action_load(
    LPRDATA rdPtr,
    long    param1,
    long    param2 )
{
    IPersistFile* ppf   = NULL;
    HRESULT hres        = E_UNEXPECTED;
    LPCSTR lpszPath     = (LPCSTR) CNC_GetStringParameter( rdPtr );
 
    if ( rdPtr != NULL
        && rdPtr->psl != NULL )
    {
        // Query IShellLink for the IPersistFile interface.
        hres = rdPtr->psl->QueryInterface(
            IID_IPersistFile,
            (LPVOID*) &ppf );

        if ( SUCCEEDED(hres) )
        {
            // Ensure that the string is Unicode.
            WCHAR wsz[MAX_PATH];
            
            if ( MultiByteToWideChar(
                    CP_ACP,
                    MB_ERR_INVALID_CHARS,
                    lpszPath,
                    -1,
                    wsz,
                    MAX_PATH ) > 0 )
            {
                // Load the link by calling IPersistFile::Load.
                hres = ppf->Load( wsz, STGM_READWRITE );
            }
            else
            {
                hres = HRESULT_FROM_WIN32( GetLastError() );
            }
            ppf->Release();
        }
    }

    /*
     * Note: throw error at the end, ensuring everything has already been
     *       released before.
     */
    if ( IS_ERROR( hres ) )
    {
        MMF2EXT_onError( rdPtr, hres );
    }

    return 0;
}

short WINAPI DLLExport Action_saveTo(
    LPRDATA rdPtr,
    long    param1,
    long    param2 )
{
    IPersistFile* ppf   = NULL;
    HRESULT hres        = E_UNEXPECTED;
    LPCSTR lpszPath     = (LPCSTR) CNC_GetStringParameter( rdPtr );
 
    if ( rdPtr != NULL
        && rdPtr->psl != NULL )
    {
        // Query IShellLink for the IPersistFile interface.
        hres = rdPtr->psl->QueryInterface(
            IID_IPersistFile,
            (LPVOID*) &ppf );

        if ( SUCCEEDED(hres) )
        {
            // Ensure that the string is Unicode.
            WCHAR wsz[MAX_PATH];
            
            if ( MultiByteToWideChar(
                    CP_ACP,
                    MB_ERR_INVALID_CHARS,
                    lpszPath,
                    -1,
                    wsz,
                    MAX_PATH ) > 0 )
            {
                // Save the link by calling IPersistFile::Save.
                hres = ppf->Save( wsz, TRUE );
            }
            else
            {
                hres = HRESULT_FROM_WIN32( GetLastError() );
            }
            ppf->Release();
        }
    }

    /*
     * Note: throw error at the end, ensuring everything has already been
     *       released before.
     */
    if ( IS_ERROR( hres ) )
    {
        MMF2EXT_onError( rdPtr, hres );
    }

    return 0;
}

short WINAPI DLLExport Action_resolve(
    LPRDATA rdPtr,
    long    param1,
    long    param2 )
{
    HRESULT hres    = E_UNEXPECTED;
 
    if ( rdPtr != NULL
        && rdPtr->psl != NULL )
    {
        hres = rdPtr->psl->Resolve( NULL, SLR_NO_UI );
    }

    if ( IS_ERROR( hres ) )
    {
        MMF2EXT_onError( rdPtr, hres );
    }

    return 0;
}

short WINAPI DLLExport Action_setArguments(
    LPRDATA rdPtr,
    long    param1,
    long    param2 )
{
    HRESULT hres    = E_UNEXPECTED;
    LPCSTR lpszArgs = (LPCSTR) CNC_GetStringParameter( rdPtr );
 
    if ( rdPtr != NULL
        && rdPtr->psl != NULL )
    {
        hres = rdPtr->psl->SetArguments( lpszArgs );       
    }

    if ( IS_ERROR( hres ) )
    {
        MMF2EXT_onError( rdPtr, hres );
    }

    return 0;
}

short WINAPI DLLExport Action_setDescription(
    LPRDATA rdPtr,
    long    param1,
    long    param2 )
{
    HRESULT hres    = E_UNEXPECTED;
    LPCSTR lpszDesc = (LPCSTR) CNC_GetStringParameter( rdPtr );
 
    if ( rdPtr != NULL
        && rdPtr->psl != NULL )
    {
        hres = rdPtr->psl->SetDescription( lpszDesc );       
    }

    if ( IS_ERROR( hres ) )
    {
        MMF2EXT_onError( rdPtr, hres );
    }

    return 0;
}

short WINAPI DLLExport Action_setHotKey(
    LPRDATA rdPtr,
    long    param1,
    long    param2 )
{
    HRESULT hres    = E_UNEXPECTED;
    WORD wHotKey    = (WORD) CNC_GetIntParameter( rdPtr );
 
    if ( rdPtr != NULL
        && rdPtr->psl != NULL )
    {
        hres = rdPtr->psl->SetHotkey( wHotKey );       
    }

    if ( IS_ERROR( hres ) )
    {
        MMF2EXT_onError( rdPtr, hres );
    }

    return 0;
}

short WINAPI DLLExport Action_setIconLocation(
    LPRDATA rdPtr,
    long    param1,
    long    param2 )
{
    HRESULT hres        = E_UNEXPECTED;
    LPCSTR lpszILPath   = (LPCSTR) CNC_GetStringParameter( rdPtr );
    int iconIndex       = (int) CNC_GetIntParameter( rdPtr );
 
    if ( rdPtr != NULL
        && rdPtr->psl != NULL )
    {
        hres = rdPtr->psl->SetIconLocation( lpszILPath, iconIndex );       
    }

    if ( IS_ERROR( hres ) )
    {
        MMF2EXT_onError( rdPtr, hres );
    }

    return 0;
}

short WINAPI DLLExport Action_setPath(
    LPRDATA rdPtr,
    long    param1,
    long    param2 )
{
    HRESULT hres    = E_UNEXPECTED;
    LPCSTR lpszPath = (LPCSTR) CNC_GetStringParameter( rdPtr );
 
    if ( rdPtr != NULL
        && rdPtr->psl != NULL )
    {
        hres = rdPtr->psl->SetPath( lpszPath );       
    }

    if ( IS_ERROR( hres ) )
    {
        MMF2EXT_onError( rdPtr, hres );
    }

    return 0;
}

short WINAPI DLLExport Action_setRelativePath(
    LPRDATA rdPtr,
    long    param1,
    long    param2 )
{
    HRESULT hres    = E_UNEXPECTED;
    LPCSTR lpszPath = (LPCSTR) CNC_GetStringParameter( rdPtr );
 
    if ( rdPtr != NULL
        && rdPtr->psl != NULL )
    {
        hres = rdPtr->psl->SetRelativePath( lpszPath, 0 );       
    }

    if ( IS_ERROR( hres ) )
    {
        MMF2EXT_onError( rdPtr, hres );
    }

    return 0;
}

short WINAPI DLLExport Action_setShowCmd(
    LPRDATA rdPtr,
    long    param1,
    long    param2 )
{
    HRESULT hres    = E_UNEXPECTED;
    int showCmd     = (WORD) CNC_GetIntParameter( rdPtr );
 
    if ( rdPtr != NULL
        && rdPtr->psl != NULL )
    {
        hres = rdPtr->psl->SetShowCmd( showCmd );       
    }

    if ( IS_ERROR( hres ) )
    {
        MMF2EXT_onError( rdPtr, hres );
    }

    return 0;
}

short WINAPI DLLExport Action_setWorkingDirectory(
    LPRDATA rdPtr,
    long    param1,
    long    param2 )
{
    HRESULT hres    = E_UNEXPECTED;
    LPCSTR lpszDir = (LPCSTR) CNC_GetStringParameter( rdPtr );
 
    if ( rdPtr != NULL
        && rdPtr->psl != NULL )
    {
        hres = rdPtr->psl->SetWorkingDirectory( lpszDir );
    }

    if ( IS_ERROR( hres ) )
    {
        MMF2EXT_onError( rdPtr, hres );
    }

    return 0;
}

short WINAPI DLLExport Action_reset(
    LPRDATA rdPtr,
    long    param1,
    long    param2 )
{
    /* 1. Clear everything */
    /* rdPtr->lastHResult = S_OK; // unneeded, see paragraph 2 */

    if ( rdPtr->psl != NULL )
    {
        rdPtr->psl->Release();
        /*
         * Note: there is no guarantee that IShellLink has been
         *       freed but we don't really care here. In fact we are
         *       more splitting from existing instance than really
         *       clearing it.
         *
         * @see IUnknown::Release method, http://msdn.microsoft.com/en-us/library/windows/desktop/ms682317%28v=vs.85%29.aspx
         */
        rdPtr->psl = NULL;
    }

    /* 2. Create a brand new IShellLink instance */
    /*
     * Note: Shell link is complex file format, even by clearing
     *       everything available through IShellLink interface, there is
     *       no guarantee that nothing will remain. Recreating a brand new
     *       object seems the best option.
     */
    rdPtr->lastHResult = CoCreateInstance(
        CLSID_ShellLink,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_IShellLink,
        (LPVOID*) &( rdPtr->psl ) );

    if ( IS_ERROR( rdPtr->lastHResult ) )
    {
        MMF2EXT_onError( rdPtr, rdPtr->lastHResult );
    }

    return 0;
}

// ============================================================================
//
// CONDITION ROUTINES
// 
// ============================================================================

long WINAPI DLLExport Condition_onFailure(
    LPRDATA rdPtr,
    long    param1,
    long    param2 )
{
    /*
     * Note: lastHResult may have been changed during true event processing.
     *       Therefore it's better to consider this condition as satisfy when
     *       handling a true event, no matters in which state this object is.
     */
	return rdPtr == NULL
        || ( rdPtr->rHo.hoFlags & HOF_TRUEEVENT )
        || MMF2EXT_IS_SEVERE_FAILURE( rdPtr->lastHResult );
}

long WINAPI DLLExport Condition_onError(
    LPRDATA rdPtr,
    long    param1,
    long    param2 )
{
    /*
     * Note: lastHResult may have been changed during true event processing.
     *       Therefore it's better to consider this condition as satisfy when
     *       handling a true event, no matters in which state this object is.
     */
    return rdPtr == NULL
        || ( rdPtr->rHo.hoFlags & HOF_TRUEEVENT )
        || IS_ERROR( rdPtr->lastHResult );
}

long WINAPI DLLExport Condition_isDirty(
    LPRDATA rdPtr,
    long    param1,
    long    param2 )
{
    IPersistFile* ppf   = NULL;
    HRESULT hres        = E_UNEXPECTED;

    if ( rdPtr != NULL
        && rdPtr->psl != NULL )
    {
        // Query IShellLink for the IPersistFile interface.
        hres = rdPtr->psl->QueryInterface(
            IID_IPersistFile,
            (LPVOID*) &ppf );

        if ( SUCCEEDED(hres) )
        {
            hres = ppf->IsDirty();
        }
        ppf->Release();
    }

    if ( IS_ERROR( hres ) )
    {
        MMF2EXT_onError( rdPtr, hres );
    }

    return hres == S_OK;
}


// ============================================================================
//
// EXPRESSIONS ROUTINES
// 
// ============================================================================

long WINAPI DLLExport Expression_getLastHResult(
    LPRDATA rdPtr,
    long    param1 )
{
    /*
     * Note: for now this expression is more returning the last significant
     *       HRESULT code than really returning the last HRESULT code.
     *       Perhaps it will need to be changed in the future, to see.
     */
    return ( rdPtr != NULL ) ? rdPtr->lastHResult : E_UNEXPECTED;
}

long WINAPI DLLExport Expression_getArgs(
    LPRDATA rdPtr,
    long    param1 )
{
    LPSTR lnkArgs   = NULL;
    HRESULT hres    = E_UNEXPECTED;

    if ( rdPtr != NULL
        && rdPtr->psl != NULL )
    {
        lnkArgs = (LPSTR) callRunTimeFunction(
            rdPtr,
            RFUNCTION_GETSTRINGSPACE_EX,
            0,
            INFOTIPSIZE + 1 );

        if ( lnkArgs != NULL )
        {
            hres = rdPtr->psl->GetArguments( lnkArgs, INFOTIPSIZE );
            lnkArgs[INFOTIPSIZE] = 0;
            if ( IS_ERROR( hres ) )
            {
                lnkArgs[0] = 0;
            }
        }
    }

    if ( IS_ERROR( hres ) )
    {
        MMF2EXT_onError( rdPtr, hres, FALSE );
    }

    if ( lnkArgs == NULL )
    {
        // MMF doesn't like NULL pointer, give it something
        lnkArgs = "";
    }

	rdPtr->rHo.hoFlags |= HOF_STRING;
    return (long) lnkArgs;
}

long WINAPI DLLExport Expression_getDescription(
    LPRDATA rdPtr,
    long    param1 )
{
    LPSTR lnkDesc   = NULL;
    HRESULT hres    = E_UNEXPECTED;

    if ( rdPtr != NULL
        && rdPtr->psl != NULL )
    {
        lnkDesc = (LPSTR) callRunTimeFunction(
            rdPtr,
            RFUNCTION_GETSTRINGSPACE_EX,
            0,
            INFOTIPSIZE + 1 );

        if ( lnkDesc != NULL )
        {
            hres = rdPtr->psl->GetDescription( lnkDesc, INFOTIPSIZE );
            lnkDesc[INFOTIPSIZE] = 0;
            if ( IS_ERROR( hres ) )
            {
                lnkDesc[0] = 0;
            }
        }
    }

    if ( IS_ERROR( hres ) )
    {
        MMF2EXT_onError( rdPtr, hres, FALSE );
    }

    if ( lnkDesc == NULL )
    {
        // MMF doesn't like NULL pointer, give it something
        lnkDesc = "";
    }

	rdPtr->rHo.hoFlags |= HOF_STRING;
    return (long) lnkDesc;
}


long WINAPI DLLExport Expression_getHotKey(
    LPRDATA rdPtr,
    long    param1 )
{
    WORD hotkey     = 0;
    HRESULT hres    = E_UNEXPECTED;

    if ( rdPtr != NULL
        && rdPtr->psl != NULL )
    {
        hres = rdPtr->psl->GetHotkey( &hotkey );
    }

    if ( IS_ERROR( hres ) )
    {
        MMF2EXT_onError( rdPtr, hres, FALSE );
    }

    return hotkey;
}

long WINAPI DLLExport Expression_getIconLocationPath(
    LPRDATA rdPtr,
    long    param1 )
{
    LPSTR lnkILPath = NULL;
    int iconIndex   = 0;
    HRESULT hres    = E_UNEXPECTED;

    if ( rdPtr != NULL
        && rdPtr->psl != NULL )
    {
        lnkILPath = (LPSTR) callRunTimeFunction(
            rdPtr,
            RFUNCTION_GETSTRINGSPACE_EX,
            0,
            MAX_PATH + 1 );

        if ( lnkILPath != NULL )
        {
            hres = rdPtr->psl->GetIconLocation(
                lnkILPath,
                MAX_PATH,
                &iconIndex );
            lnkILPath[MAX_PATH] = 0;
            if ( IS_ERROR( hres ) )
            {
                lnkILPath[0] = 0;
            }
        }
    }

    if ( IS_ERROR( hres ) )
    {
        MMF2EXT_onError( rdPtr, hres, FALSE );
    }

    if ( lnkILPath == NULL )
    {
        // MMF doesn't like NULL pointer, give it something
        lnkILPath = "";
    }

	rdPtr->rHo.hoFlags |= HOF_STRING;
    return (long) lnkILPath;
}

long WINAPI DLLExport Expression_getIconLocationIndex(
    LPRDATA rdPtr,
    long    param1 )
{
    int iconIndex   = 0;
    HRESULT hres    = E_UNEXPECTED;

    if ( rdPtr != NULL
        && rdPtr->psl != NULL )
    {
        hres = rdPtr->psl->GetIconLocation( NULL, 0, &iconIndex );
    }

    if ( IS_ERROR( hres ) )
    {
        MMF2EXT_onError( rdPtr, hres, FALSE );
    }

    return iconIndex;
}

long WINAPI DLLExport Expression_getRawPath(
    LPRDATA rdPtr,
    long    param1 )
{
    LPSTR lnkRawPath    = NULL;
    HRESULT hres        = E_UNEXPECTED;

    if ( rdPtr != NULL
        && rdPtr->psl != NULL )
    {
        lnkRawPath = (LPSTR) callRunTimeFunction(
            rdPtr,
            RFUNCTION_GETSTRINGSPACE_EX,
            0,
            MAX_PATH + 1 );

        if ( lnkRawPath != NULL )
        {
            hres = rdPtr->psl->GetPath(
                lnkRawPath,
                MAX_PATH,
                NULL,
                SLGP_RAWPATH );
            lnkRawPath[MAX_PATH] = 0;
            if ( hres != S_OK )
            {
                lnkRawPath[0] = 0;
            }
        }
    }

    if ( IS_ERROR( hres ) )
    {
        MMF2EXT_onError( rdPtr, hres, FALSE );
    }

    if ( lnkRawPath == NULL )
    {
        // MMF doesn't like NULL pointer, give it something
        lnkRawPath = "";
    }

	rdPtr->rHo.hoFlags |= HOF_STRING;
    return (long) lnkRawPath;
}

long WINAPI DLLExport Expression_getShowCmd(
    LPRDATA rdPtr,
    long    param1 )
{
    int showCmd   = 0;
    HRESULT hres    = E_UNEXPECTED;

    if ( rdPtr != NULL
        && rdPtr->psl != NULL )
    {
        hres = rdPtr->psl->GetShowCmd( &showCmd );
    }

    if ( IS_ERROR( hres ) )
    {
        MMF2EXT_onError( rdPtr, hres, FALSE );
    }

    return showCmd;
}

long WINAPI DLLExport Expression_getWorkingDirectory(
    LPRDATA rdPtr,
    long    param1 )
{
    LPSTR lnkWrkDir = NULL;
    HRESULT hres    = E_UNEXPECTED;

    if ( rdPtr != NULL
        && rdPtr->psl != NULL )
    {
        lnkWrkDir = (LPSTR) callRunTimeFunction(
            rdPtr,
            RFUNCTION_GETSTRINGSPACE_EX,
            0,
            MAX_PATH + 1 );

        if ( lnkWrkDir != NULL )
        {
            hres = rdPtr->psl->GetWorkingDirectory(
                lnkWrkDir,
                MAX_PATH );
            lnkWrkDir[MAX_PATH] = 0;
            if ( hres != S_OK )
            {
                lnkWrkDir[0] = 0;
            }
        }
    }

    if ( IS_ERROR( hres ) )
    {
        MMF2EXT_onError( rdPtr, hres, FALSE );
    }

    if ( lnkWrkDir == NULL )
    {
        // MMF doesn't like NULL pointer, give it something
        lnkWrkDir = "";
    }

	rdPtr->rHo.hoFlags |= HOF_STRING;
    return (long) lnkWrkDir;
}

// ============================================================================
//
// MMF Interface
// 
// ============================================================================

// ----------------------------------------------------------
// Condition / Action / Expression prototypes
// ----------------------------------------------------------

// Quick memo: content of the eventInformations arrays
// ---------------------------------------------------
// Menu ID
// String ID
// Code
// Flags
// Number_of_parameters
// Parameter_type [Number_of_parameters]
// Parameter_TitleString [Number_of_parameters]

// Definitions of parameters for each action
short actionsInfos[] =
{
    ACT_IDMN_CLEARLASTERRORCODE,
        ACT_IDS_CLEARLASTERRORCODE,
        ACT_CLEARLASTERRORCODE,
        0,
        0,

    ACT_IDMN_LOAD,
        ACT_IDS_LOAD,
        ACT_LOAD,
        0,
        1,
        PARAM_FILENAME2,
        ACT_IDP_LOAD_0,

    ACT_IDMN_SAVETO,
        ACT_IDS_SAVETO,
        ACT_SAVETO,
        0,
        1,
        PARAM_FILENAME2,
        ACT_IDP_SAVETO_0,

    ACT_IDMN_RESOLVE,
        ACT_IDS_RESOLVE,
        ACT_RESOLVE,
        0,
        0,

    ACT_IDMN_SETARGS,
        ACT_IDS_SETARGS,
        ACT_SETARGS,
        0,
        1,
        PARAM_EXPSTRING,
        ACT_IDP_SETARGS_0,

    ACT_IDMN_SETDESCRIPTION,
        ACT_IDS_SETDESCRIPTION,
        ACT_SETDESCRIPTION,
        0,
        1,
        PARAM_EXPSTRING,
        ACT_IDP_SETDESCRIPTION_0,

    ACT_IDMN_SETHOTKEY,
        ACT_IDS_SETHOTKEY,
        ACT_SETHOTKEY,
        0,
        1,
        PARAM_EXPRESSION,
        ACT_IDP_SETHOTKEY_0,

    ACT_IDMN_SETICONLOCATION,
        ACT_IDS_SETICONLOCATION,
        ACT_SETICONLOCATION,
        0,
        2,
        PARAM_FILENAME2,
        PARAM_EXPRESSION,
        ACT_IDP_SETICONLOCATION_0,
        ACT_IDP_SETICONLOCATION_1,

    ACT_IDMN_SETPATH,
        ACT_IDS_SETPATH,
        ACT_SETPATH,
        0,
        1,
        PARAM_EXPSTRING,
        ACT_IDP_SETPATH_0,

    ACT_IDMN_SETRELATIVEPATH,
        ACT_IDS_SETRELATIVEPATH,
        ACT_SETRELATIVEPATH,
        0,
        1,
        PARAM_EXPSTRING,
        ACT_IDP_SETRELATIVEPATH_0,

    ACT_IDMN_SETSHOWCMD,
        ACT_IDS_SETSHOWCMD,
        ACT_SETSHOWCMD,
        0,
        1,
        PARAM_EXPRESSION,
        ACT_IDP_SETSHOWCMD_0,

    ACT_IDMN_SETWORKINGDIRECTORY,
        ACT_IDS_SETWORKINGDIRECTORY,
        ACT_SETWORKINGDIRECTORY,
        0,
        1,
        PARAM_EXPSTRING,
        ACT_IDP_SETWORKINGDIRECTORY_0,

    ACT_IDMN_RESET,
        ACT_IDS_RESET,
        ACT_RESET,
        0,
        0,

    0 /* end of array */
};

// Definitions of parameters for each condition
short conditionsInfos[] =
{
    CND_IDMN_ONFAILURE,
        CND_IDS_ONFAILURE,
        CND_ONFAILURE,
        0,
        0,

    CND_IDMN_ONERROR,
        CND_IDS_ONERROR,
        CND_ONERROR,
        0,
        0,

    CND_IDMN_ISDIRTY,
        CND_IDS_ISDIRTY,
        CND_ISDIRTY,
        EVFLAGS_ALWAYS|EVFLAGS_NOTABLE,
        0,

    0 /* end of array */
};

// Definitions of parameters for each expression
short expressionsInfos[] =
{
    EXP_IDMN_GETLASTERRORCODE,
        EXP_IDS_GETLASTERRORCODE,
        EXP_GETLASTERRORCODE,
        0,
        0,

    EXP_IDMN_GETARGS,
        EXP_IDS_GETARGS,
        EXP_GETARGS,
        EXPFLAG_STRING,
        0,

    EXP_IDMN_GETDESCRIPTION,
        EXP_IDS_GETDESCRIPTION,
        EXP_GETDESCRIPTION,
        EXPFLAG_STRING,
        0,

    EXP_IDMN_GETHOTKEY,
        EXP_IDS_GETHOTKEY,
        EXP_GETHOTKEY,
        0,
        0,

    EXP_IDMN_GETICONLOCATIONPATH,
        EXP_IDS_GETICONLOCATIONPATH,
        EXP_GETICONLOCATIONPATH,
        EXPFLAG_STRING,
        0,

    EXP_IDMN_GETICONLOCATIONINDEX,
        EXP_IDS_GETICONLOCATIONINDEX,
        EXP_GETICONLOCATIONINDEX,
        0,
        0,

    EXP_IDMN_GETRAWPATH,
        EXP_IDS_GETRAWPATH,
        EXP_GETRAWPATH,
        EXPFLAG_STRING,
        0,

    EXP_IDMN_GETSHOWCMD,
        EXP_IDS_GETSHOWCMD,
        EXP_GETSHOWCMD,
        0,
        0,

    EXP_IDMN_GETWORKINGDIRECTORY,
        EXP_IDS_GETWORKINGDIRECTORY,
        EXP_GETWORKINGDIRECTORY,
        EXPFLAG_STRING,
        0,

    0 /* end of array */
};

// ----------------------------------------------------------
// Condition / Action / Expression jump table
// ----------------------------------------------------------
// Contains the address inside the extension of the different
// routines that handle the action, conditions and expressions.
// Located at the end of the source for convinience
// Must finish with a 0
//
short (WINAPI * ActionJumps[])(LPRDATA rdPtr, long param1, long param2) =
{
    Action_clearLastHResult,

    Action_load,
    Action_saveTo,

    Action_resolve,
    Action_setArguments,
    Action_setDescription,
    Action_setHotKey,
    Action_setIconLocation,
    Action_setPath,
    Action_setRelativePath,
    Action_setShowCmd,
    Action_setWorkingDirectory,

    Action_reset,

    NULL /* end of array */
};

long (WINAPI * ConditionJumps[])(LPRDATA rdPtr, long param1, long param2) =
{
    Condition_onFailure,
    Condition_onError,

    Condition_isDirty,
        
    NULL /* end of array */
};

long (WINAPI * ExpressionJumps[])(LPRDATA rdPtr, long param) =
{
    Expression_getLastHResult,

    Expression_getArgs,
    Expression_getDescription,
    Expression_getHotKey,
    Expression_getIconLocationPath,
    Expression_getIconLocationIndex,
    Expression_getRawPath,
    Expression_getShowCmd,
    Expression_getWorkingDirectory,


    NULL /* end of array */
};

/* END OF FILE */
