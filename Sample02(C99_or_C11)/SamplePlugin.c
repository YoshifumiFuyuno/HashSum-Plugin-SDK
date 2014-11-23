/**
 * HashSum Plugin SDK
 *
 * Written by Yoshifumi Fuyuno <http://hp.vector.co.jp/authors/VA052754/>, 2014.
 * License: CC0(Public Domain) <https://creativecommons.org/publicdomain/zero/1.0/>
 */

#include "..\Plugin_SDK.h"
#include "SamplePluginAlg.h"


// Generating GUIDs: uuidgen, GuidGen.exe, etc.
/*
static const GUID pluginGuid = 
{ 0x00000000, 0x0000, 0x0000, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }; // Please change to every Plugin

static const GUID algGuid = 
{ 0x00000000, 0x0000, 0x0000, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }; // Please change to every Algorithm

#define ALGNAMEW L"AlgorithmName"
*/
static const GUID pluginGuid = 
{ 0x2a818f76, 0xf50, 0x452c, { 0xa8, 0x7d, 0xe8, 0x72, 0xd6, 0xc5, 0x51, 0xee } };

static const GUID algGuidSumBSD = 
{ 0x289ca81d, 0xfee5, 0x42ab, { 0x90, 0x9b, 0xa6, 0x7d, 0xee, 0xb8, 0xb7, 0x22 } };
static const GUID algGuidSumBSDDecimal = 
{ 0x71ca6b8f, 0xe4ef, 0x4e09, { 0x90, 0xf8, 0x2f, 0x98, 0xd7, 0x30, 0xa9, 0x26 } };

#define ALGNAMESUMBSDW        ((const HSPCHAR*)U16SL("sumBSD"))
#define ALGNAMESUMBSDDecimalW ((const HSPCHAR*)U16SL("sumBSD(Decimal)"))


typedef struct _luAlgInfo
{
  bool tagEx;
  union
  {
    void *dummy;
    PHSP_AlgInfo n;
    PHSP_AlgInfoEx ex;
  };
} luAlgInfo, *luPAlgInfo;
#define SETLUALGINFO_NORMAL(_luAlgInfo, _szAlgName, _DigestSize, _pGuid) \
  if((_luAlgInfo)->tagEx) \
  { \
    (_luAlgInfo)->ex->eHSPAlgFlags = HSPAlgFlags_None; \
    (_luAlgInfo)->ex->pGuid = (_pGuid); \
    (_luAlgInfo)->ex->szAlgFileName = (_szAlgName); \
  } \
  else \
  { \
    (_luAlgInfo)->n->BlockSizeOctets = 0; \
    (_luAlgInfo)->n->szAlgName = (_szAlgName); \
    (_luAlgInfo)->n->DigestSize = (_DigestSize); \
  } \
  return TRUE;


/*
static HSPINLINE BOOL HSPPF_General_ShowSettingDlg(HWND hWndParent)
{
  DialogBox(, , hWndParent, );
  return TRUE;
}
*/


static HSPINLINE BOOL HSPPF_Hash_UseSettingDlg(uint32_t AlgID)
{
  UNREFERENCED_PARAMETER(AlgID);
  return FALSE;
  /*
  switch(AlgID)
  {
    case ...:
      return TRUE;
    default:
      return FALSE;
  }
  */
}


/*
static HSPINLINE BOOL HSPPF_Hash_ShowSettingDlg(HWND hWndParent, uint32_t AlgID)
{
  switch(AlgID)
  {
    case HSPSupportAlg_...:
      DialogBox(, , hWndParent, );
      return TRUE;
  }
  return FALSE;
}
*/


static HSPINLINE BOOL HSPPF_Hash_GetAlgFunctions(PHSP_AlgFunctions pAlgFunctions, uint32_t AlgID)
{
  /*
  switch(AlgID)
  {
    case HSPSupportAlg_...:
      ...
      return TRUE;
  }
  return FALSE;
  */
  //UNREFERENCED_PARAMETER(AlgID);

  pAlgFunctions->fpHSP_HashInitialize = HSP_HashInitialize;
  pAlgFunctions->fpHSP_HashUpdate = HSP_HashUpdate;
  pAlgFunctions->fpHSP_HashReset = HSP_HashReset;
  pAlgFunctions->fpHSP_HashFinalize = HSP_HashFinalize;
  pAlgFunctions->fpHSP_HashClone = HSP_HashClone;

  if(AlgID == HSPSupportAlg_SumBSDDecimal)
    pAlgFunctions->fpHSP_HashGetString = HSP_HashGetString;
  else
    pAlgFunctions->fpHSP_HashGetHex = HSP_HashGetHex;
  return TRUE;
}


static HSPINLINE BOOL SetluAlgInfo(uint32_t AlgID, luPAlgInfo pLuAlgInfo)
{
  switch(AlgID)
  {
    case HSPSupportAlg_SumBSD:
      SETLUALGINFO_NORMAL(pLuAlgInfo, ALGNAMESUMBSDW, 2, &algGuidSumBSD);
    case HSPSupportAlg_SumBSDDecimal:
      SETLUALGINFO_NORMAL(pLuAlgInfo, ALGNAMESUMBSDDecimalW,
        5 + 1, // UINT16_MAX:65535 = Max 5 characters + null terminator
        &algGuidSumBSDDecimal
      );
  }
  return FALSE;
}


static HSPINLINE BOOL HSPPF_Hash_GetAlgInfo(PHSP_AlgInfo pAlgInfo, uint32_t AlgID)
{
  /*
  switch(AlgID)
  {
    case ...:
      ...
      return TRUE;
  }
  return FALSE;
  */
  luAlgInfo ai = { false, { pAlgInfo } };
  return SetluAlgInfo(AlgID, &ai);
}


static HSPINLINE BOOL HSPPF_Hash_GetAlgInfoEx(PHSP_AlgInfoEx pAlgInfoEx, uint32_t AlgID)
{
  /*
  switch(AlgID)
  {
    case ...:
      ...
      return TRUE;
  }
  return FALSE;
  */
  luAlgInfo ai = { true, { pAlgInfoEx } };
  return SetluAlgInfo(AlgID, &ai);
}


// **Required Thread-safe (critical)**
LRESULT HSPCALL HSP_PluginFunc(HSPPFMsg uMsg, WPARAM wParam, LPARAM lParam)
{
  switch(uMsg)
  {
    case HSPPFMsg_General_UseSettingDlg: return FALSE;
    //HSPPF_MSG(HSPPFMsg_General_ShowSettingDlg);
    case HSPPFMsg_Hash_GetSupportAlgCount: return HSPSupportAlg_SupportAlgCount;
    HSPPF_MSG(HSPPFMsg_Hash_UseSettingDlg);
    //HSPPF_MSG(HSPPFMsg_Hash_ShowSettingDlg);
    HSPPF_MSG(HSPPFMsg_Hash_GetAlgFunctions);
    HSPPF_MSG(HSPPFMsg_Hash_GetAlgInfo);
    HSPPF_MSG(HSPPFMsg_Hash_GetAlgInfoEx);
  }
  return 0;
}


BOOL HSPCALL HSP_Initialize(CPHSP_InitInfo cpInitInfo, PHSP_PluginBasicInfo pPluginBasicInfo)
{
  UNREFERENCED_PARAMETER(cpInitInfo);

  pPluginBasicInfo->pGuid = &pluginGuid;
  pPluginBasicInfo->eHSPFuncFlags = HSPFuncFlags_Hash;

  pPluginBasicInfo->pluginInterfaceVer = HSP_INTERFACE_VER; // Don't touch
  pPluginBasicInfo->pluginSDKVer = HSP_SDK_VER; // Don't touch
  return TRUE; // return FALSE is Disable Plugin.
}


BOOL WINAPI DllMain(HINSTANCE hinstDll, DWORD dwReason, LPVOID lpReserved)
{
  UNREFERENCED_PARAMETER(lpReserved);
  switch(dwReason)
  {
    case DLL_PROCESS_ATTACH:
      #if defined(_DLL) // http://support.microsoft.com/kb/555563
      DisableThreadLibraryCalls(hinstDll);
      #else
      UNREFERENCED_PARAMETER(hinstDll);
      #endif  // _DLL
      break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
      break;
  }
  return TRUE;
}
