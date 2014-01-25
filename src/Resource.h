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

// Used by Ext.rc

#ifndef __MMF2EXT_ISHELLLINK_RESOURCE_H__
#define __MMF2EXT_ISHELLLINK_RESOURCE_H__

#define _APS_NO_MFC             1

// About box information
#define	IDST_OBJNAME            128
#define	IDST_AUTHOR             129
#define	IDST_COPYRIGHT          130
#define	IDST_COMMENT            131
#define	IDST_HTTP               132

// Dialog Box ID
#define DB_SETUP                20003

// Help ID
#define ID_HELP                 20004

// 	Actions
//-------------------

// Identifier of actions menu
#define MN_ACTIONS                              20000

// Commands of action menu                      (25000-25990)
#define ACT_IDMN_CLEARLASTERRORCODE             (25000)

#define ACT_IDMN_LOAD                           (25010)
#define ACT_IDMN_SAVETO                         (25020)

#define ACT_IDMN_RESOLVE                        (25030)
#define ACT_IDMN_SETARGS                        (25040)
#define ACT_IDMN_SETDESCRIPTION                 (25050)
#define ACT_IDMN_SETHOTKEY                      (25060)
#define ACT_IDMN_SETICONLOCATION                (25070)
#define ACT_IDMN_SETPATH                        (25080)
#define ACT_IDMN_SETRELATIVEPATH                (25090)
#define ACT_IDMN_SETSHOWCMD                     (25100)
#define ACT_IDMN_SETWORKINGDIRECTORY            (25110)

#define ACT_IDMN_RESET                          (25120)

// Action strings                               (5000-5990)
#define ACT_IDS_CLEARLASTERRORCODE              (5000)

#define ACT_IDS_LOAD                            (5010)
#define ACT_IDS_SAVETO                          (5020)

#define ACT_IDS_RESOLVE                         (5030)
#define ACT_IDS_SETARGS                         (5040)
#define ACT_IDS_SETDESCRIPTION                  (5050)
#define ACT_IDS_SETHOTKEY                       (5060)
#define ACT_IDS_SETICONLOCATION                 (5070)
#define ACT_IDS_SETPATH                         (5080)
#define ACT_IDS_SETRELATIVEPATH                 (5090)
#define ACT_IDS_SETSHOWCMD                      (5100)
#define ACT_IDS_SETWORKINGDIRECTORY             (5110)

#define ACT_IDS_RESET                           (5120)

// Titles of action parameters                  (5xx1-5xx9)
#define ACT_IDP_LOAD_0                          (5011)
#define ACT_IDP_SAVETO_0                        (5021)
#define ACT_IDP_SETARGS_0                       (5041)
#define ACT_IDP_SETDESCRIPTION_0                (5051)
#define ACT_IDP_SETHOTKEY_0                     (5061)
#define ACT_IDP_SETICONLOCATION_0               (5071)
#define ACT_IDP_SETICONLOCATION_1               (5072)
#define ACT_IDP_SETPATH_0                       (5081)
#define ACT_IDP_SETRELATIVEPATH_0               (5091)
#define ACT_IDP_SETSHOWCMD_0                    (5101)
#define ACT_IDP_SETWORKINGDIRECTORY_0           (5111)

//	Conditions
//--------------------

// Identifier of conditions menu
#define MN_CONDITIONS                           20001

// Commands of condition menu                   (26000-26990)
#define CND_IDMN_ONFAILURE                      (26000)
#define CND_IDMN_ONERROR                        (26010)

#define CND_IDMN_ISDIRTY                        (26020)

// Condition strings                            (6000-6990)
#define CND_IDS_ONFAILURE                       (6000)
#define CND_IDS_ONERROR                         (6010)

#define CND_IDS_ISDIRTY                         (6020)

// Titles of condition parameters               (6xx1-6xx9)


//	EXPRESSIONS
//---------------------

// Identifier of expressions menu
#define MN_EXPRESSIONS                          20002

// Commands of expressions menu                 (27000-27990)
#define EXP_IDMN_GETLASTERRORCODE               (27000)

#define EXP_IDMN_GETARGS                        (27010)
#define EXP_IDMN_GETDESCRIPTION                 (27020)
#define EXP_IDMN_GETHOTKEY                      (27030)
#define EXP_IDMN_GETICONLOCATIONPATH            (27040)
#define EXP_IDMN_GETICONLOCATIONINDEX           (27050)
#define EXP_IDMN_GETRAWPATH                     (27060)
#define EXP_IDMN_GETSHOWCMD                     (27070)
#define EXP_IDMN_GETWORKINGDIRECTORY            (27080)

// Expression strings                           (7000-7990)
#define EXP_IDS_GETLASTERRORCODE                (7000)

#define EXP_IDS_GETARGS                         (7010)
#define EXP_IDS_GETDESCRIPTION                  (7020)
#define EXP_IDS_GETHOTKEY                       (7030)
#define EXP_IDS_GETICONLOCATIONPATH             (7040)
#define EXP_IDS_GETICONLOCATIONINDEX            (7050)
#define EXP_IDS_GETRAWPATH                      (7060)
#define EXP_IDS_GETSHOWCMD                      (7070)
#define EXP_IDS_GETWORKINGDIRECTORY             (7080)

// Names of expression parameters               (7xx1-7xx9)


// 	Debug
//-------------------
#if !defined(RUN_ONLY)

#define DB_IDS_LASTERRORCODE                    (8000)

#endif


// Next default values for new objects
// 
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS

#define _APS_NEXT_RESOURCE_VALUE        101
#define _APS_NEXT_COMMAND_VALUE         40001
#define _APS_NEXT_CONTROL_VALUE         1001
#define _APS_NEXT_SYMED_VALUE           101
#endif
#endif

#endif /* __MMF2EXT_ISHELLLINK_RESOURCE_H__ */

/* END OF FILE */