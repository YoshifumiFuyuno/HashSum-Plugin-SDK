/**
 * HashSum Plugin SDK
 *
 * Written by Yoshifumi Fuyuno <http://hp.vector.co.jp/authors/VA052754/>, 2014.
 * License: Mozilla Public License Version 2.0 <https://www.mozilla.org/MPL/2.0/>
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __HSPSDK_H
#define __HSPSDK_H

#include "Plugin_SDK_Base.h"
#include <windows.h>

#if defined(__cplusplus)
extern "C" {
#endif

#include "Plugin_SDK_crackers.h"

/*
typedef struct {
  uint32_t Data1;
  uint16_t Data2;
  uint16_t Data3;
  uint8_t  Data4[8];
} GUID;  // size is 16 //same as win32 GUID
*/


#define HSP_SDK_VER (0x00010000) // HashSum Plugin SDK Ver.1.00
#define HSP_INTERFACE_VER (0x00010000)


// Plugin Flags
enum //HSPFuncFlags : uint64_t
{
  HSPFuncFlags_General    = 0x00000001ULL,
  HSPFuncFlags_Hash       = 0x00000002ULL, // exclusive
  HSPFuncFlags_Convert    = 0x00000004ULL,

  HSPFuncFlags_Reserved01 = 0x00000100ULL, // in
  HSPFuncFlags_Reserved02 = 0x00000200ULL, // out
  HSPFuncFlags_Reserved03 = 0x00000400ULL, // test
  HSPFuncFlags_Reserved04 = 0x00000800ULL, // open
  HSPFuncFlags_Reserved05 = 0x00001000ULL  // exit
};
typedef uint64_t HSPFuncFlags;


//FPHSPProcActionHandler Msg
enum //HSPPAHMsg : UINT
{
  HSPPAHMsg_GetHostName = WM_APP + 0x1000,
  HSPPAHMsg_GetHostVersion, // e.g., Ver.1.11 is returned 0x0001000B: major, minor = HIWORD(dw1), LOWORD(dw1)
  //HSPPAHMsg_GetHashSumIniPath,
  //HSPPAHMsg_GetIniDir,
  HSPPAHMsg_GetMainWindowHandle,
  HSPPAHMsg_HashNowCalc = HSPPAHMsg_GetHostName + 0x100
};
typedef UINT HSPPAHMsg;


//HSP_PluginFunc Msg
enum //HSPPFMsg : UINT
{
  HSPPFMsg_General_UseSettingDlg = WM_APP + 0x1000,
  HSPPFMsg_General_ShowSettingDlg, // wParam:hWndParent, lParam:0
  // HASHPLUGIN
  HSPPFMsg_Hash_GetSupportAlgCount = HSPPFMsg_General_UseSettingDlg + 0x500,
  HSPPFMsg_Hash_UseSettingDlg, // lParam:AlgID[0 to (GetSupportAlgCount-1)]
  HSPPFMsg_Hash_ShowSettingDlg, // wParam:hWndParent, lParam:AlgID[0 to (GetSupportAlgCount-1)]
  HSPPFMsg_Hash_GetAlgFunctions, // wParam:PHSP_AlgFunctions (structure Zero cleared), lParam:AlgID[0 to (GetSupportAlgCount-1)]
  HSPPFMsg_Hash_GetAlgInfo, // wParam:PHSP_AlgInfo, lParam:AlgID[0 to (GetSupportAlgCount-1)]
  HSPPFMsg_Hash_GetAlgInfoEx // wParam:PHSP_AlgInfoEx, lParam:AlgID[0 to (GetSupportAlgCount-1)]
};
typedef UINT HSPPFMsg;


#ifdef _MSC_VER
#pragma pack(push, 1)
#else
#pragma pack(1)
#endif

typedef struct _HSP_PluginBasicInfo
{
  const GUID   *pGuid;
  HSPFuncFlags eHSPFuncFlags; // size is 8 Octets
  uint32_t     pluginInterfaceVer;
  uint32_t     pluginSDKVer;
} HSP_PluginBasicInfo, *PHSP_PluginBasicInfo;


typedef LRESULT (HSPCALL *FPHSPProcActionHandler)(HSPPAHMsg uMsg, WPARAM wParam, LPARAM lParam);

typedef struct _HSP_InitInfo
{
  FPHSPProcActionHandler fpHandler;
  LCID userLCID;
} HSP_InitInfo;
typedef const struct _HSP_InitInfo* CPHSP_InitInfo;


typedef struct _HSP_FileInfo
{
  const HSPCHAR *szDir;        // terminal char is u'\\' // not file is nullptr
  const HSPCHAR *szFilename;   // not file is nullptr
  rsize_t       szDirLen;      // excluding the terminal NULL
  rsize_t       szFilenameLen; // excluding the terminal NULL
  //uint64_t       Reserved01;
} HSP_FileInfo;
typedef const struct _HSP_FileInfo* CPHSP_FileInfo;

typedef void*   (HSPCALL *FPHSP_HashInitialize)(uint32_t AlgID);
typedef void    (HSPCALL *FPHSP_HashUpdate)(void *state, const uint8_t *data, rsize_t dataOctets);
typedef void    (HSPCALL *FPHSP_HashGetHex)(void *state, uint8_t *digest, rsize_t getOctets);
typedef void    (HSPCALL *FPHSP_HashGetString)(void *state, HSPCHAR *szDigest, rsize_t getLen, bool bUppercase);
typedef void    (HSPCALL *FPHSP_HashReset)(void *state);
typedef void    (HSPCALL *FPHSP_HashFinalize)(void *state);
typedef void*   (HSPCALL *FPHSP_HashClone)(void *state);
typedef void    (HSPCALL *FPHSP_HashSetFileInfo)(CPHSP_FileInfo cpFileInfo);
typedef int32_t (HSPCALL *FPHSP_HashCompare)(const HSPCHAR *szDigestA, const HSPCHAR *szDigestB);

typedef struct _HSP_AlgFunctions // **All Functions Required Thread-safe (critical)**
{
  FPHSP_HashInitialize  fpHSP_HashInitialize;
  FPHSP_HashUpdate      fpHSP_HashUpdate;
  FPHSP_HashGetHex      fpHSP_HashGetHex; // DigestIsString is nullptr
  FPHSP_HashReset       fpHSP_HashReset;
  FPHSP_HashFinalize    fpHSP_HashFinalize;
  FPHSP_HashGetString   fpHSP_HashGetString; // DigestIsHex is nullptr
  FPHSP_HashClone       fpHSP_HashClone; // nullptr is NOT RECOMMENDED
  FPHSP_HashCompare     fpHSP_HashCompare; // usually: nullptr
  FPHSP_HashSetFileInfo fpHSP_HashSetFileInfo; // usually: nullptr // target not file is this func no call
} HSP_AlgFunctions, *PHSP_AlgFunctions;

typedef struct _HSP_AlgInfo
{
  const HSPCHAR *szAlgName;
  rsize_t       DigestSize; // DigestIsHex is HexOctets, DigestIsString is StringLen(include null terminator)
  rsize_t       BlockSizeOctets;
} HSP_AlgInfo, *PHSP_AlgInfo;

enum //HSPAlgFlags : uint64_t
{
  HSPAlgFlags_None,
  HSPAlgFlags_DigestSizeArbitrary   = 0x00000001ULL,
  HSPAlgFlags_CanUpdateAfterGetHash = 0x00000002ULL, // HSP_HashGetHex() / HSP_HashGetString() function can be performed in the middle of HSP_HashUpdate() function
  HSPAlgFlags_GetDigestNotConvert   = 0x00000100ULL,
  HSPAlgFlags_GetDigestIsHMAC       = 0x00000200ULL, // HSP_HashGetHex() / HSP_HashGetString() return HMAC Hex/String.
/*
  HSPAlgFlags_OriginalCompare       = 0x00000004ULL,
*/
};
typedef uint64_t HSPAlgFlags;

typedef struct _HSP_AlgInfoEx
{
  const GUID    *pGuid;
  const HSPCHAR *szAlgFileName; // use by FileName & FileAssociation & CmdLineOpt
  HSPAlgFlags   eHSPAlgFlags; // size is 8 Octets
} HSP_AlgInfoEx, *PHSP_AlgInfoEx;

#ifdef _MSC_VER
#pragma pack(pop)
#else
#pragma pack()
#endif

BOOL HSPCALL HSP_Initialize(CPHSP_InitInfo cpInitInfo, PHSP_PluginBasicInfo pPluginBasicInfo);
LRESULT HSPCALL HSP_PluginFunc(HSPPFMsg uMsg, WPARAM wParam, LPARAM lParam); // **Required Thread-safe (critical)**

#if defined(__cplusplus)
}
#endif

#endif // __HSPSDK_H
