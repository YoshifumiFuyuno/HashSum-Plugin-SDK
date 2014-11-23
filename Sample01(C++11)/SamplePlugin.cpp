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
{ 0xe1614a96, 0x83a8, 0x4196, { 0x94, 0xc5, 0x9a, 0x51, 0x71, 0x1b, 0x64, 0x42 } };

static const GUID algGuidSum8 = 
{ 0xf81d3cc5, 0x1d66, 0x4a13, { 0x87, 0x36, 0x1, 0xbe, 0xa5, 0xe1, 0x1, 0x97 } };
static const GUID algGuidSum16 = 
{ 0x19fcda0, 0x6de1, 0x4f88, { 0xa0, 0x61, 0xaa, 0x61, 0xe2, 0xae, 0x1e, 0x82 } };
static const GUID algGuidSum24 = 
{ 0xa0af93b1, 0xa848, 0x4b49, { 0xa9, 0xf9, 0x6, 0x7f, 0x2e, 0x5e, 0xf6, 0xbe } };
static const GUID algGuidSum32 = 
{ 0xdfd1fea2, 0xaa8d, 0x456a, { 0x8a, 0x82, 0x35, 0x89, 0xf2, 0xe5, 0x46, 0x23 } };
static const GUID algGuidSum64 = 
{ 0x55174a3a, 0x5e51, 0x486b, { 0x8f, 0x23, 0x1a, 0xd8, 0x45, 0x16, 0xb8, 0x41 } };
static const GUID algGuidSumSysV = 
{ 0x882938eb, 0xc4e9, 0x4fe8, { 0x8e, 0xc8, 0xb4, 0xb8, 0xd6, 0xfa, 0x4b, 0x88 } };
static const GUID algGuidSumSysVDecimal = 
{ 0x6fe7378, 0xb04e, 0x447a, { 0x9d, 0x88, 0xf6, 0xa1, 0x39, 0xd6, 0x51, 0xe6 } };

#define ALGNAMESUM8W           reinterpret_cast<const HSPCHAR*>(U16SL("sum8"))
#define ALGNAMESUM16W          reinterpret_cast<const HSPCHAR*>(U16SL("sum16"))
#define ALGNAMESUM24W          reinterpret_cast<const HSPCHAR*>(U16SL("sum24"))
#define ALGNAMESUM32W          reinterpret_cast<const HSPCHAR*>(U16SL("sum32"))
#define ALGNAMESUM64W          reinterpret_cast<const HSPCHAR*>(U16SL("sum64"))
#define ALGNAMESUMSYSVW        reinterpret_cast<const HSPCHAR*>(U16SL("sumSysV"))
#define ALGNAMESUMSYSVDecimalW reinterpret_cast<const HSPCHAR*>(U16SL("sumSysV(Decimal)"))


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
  if((_luAlgInfo).tagEx) \
  { \
    (_luAlgInfo).ex->eHSPAlgFlags = HSPAlgFlags_None; \
    (_luAlgInfo).ex->pGuid = (_pGuid); \
    (_luAlgInfo).ex->szAlgFileName = (_szAlgName); \
  } \
  else \
  { \
    (_luAlgInfo).n->BlockSizeOctets = 0; \
    (_luAlgInfo).n->szAlgName = (_szAlgName); \
    (_luAlgInfo).n->DigestSize = (_DigestSize); \
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
  switch(static_cast<HSPSupportAlg>(AlgID))
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
  switch(static_cast<HSPSupportAlg>(AlgID))
  {
    case HSPSupportAlg::...:
      DialogBox(, , hWndParent, );
      return TRUE;
  }
  return FALSE;
}
*/


static HSPINLINE BOOL HSPPF_Hash_GetAlgFunctions(PHSP_AlgFunctions pAlgFunctions, uint32_t AlgID)
{
  /*
  switch(static_cast<HSPSupportAlg>(AlgID))
  {
    case HSPSupportAlg::...:
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

  if(static_cast<HSPSupportAlg>(AlgID) == HSPSupportAlg::SumSysVDecimal)
    pAlgFunctions->fpHSP_HashGetString = HSP_HashGetString;
  else
    pAlgFunctions->fpHSP_HashGetHex = HSP_HashGetHex;
  return TRUE;
}


static HSPINLINE BOOL SetluAlgInfo(uint32_t AlgID, luAlgInfo& RefLuAlgInfo)
{
  switch(static_cast<HSPSupportAlg>(AlgID))
  {
    case HSPSupportAlg::Sum8:
      SETLUALGINFO_NORMAL(RefLuAlgInfo, ALGNAMESUM8W, 1, &algGuidSum8);
    case HSPSupportAlg::Sum16:
      SETLUALGINFO_NORMAL(RefLuAlgInfo, ALGNAMESUM16W, 2, &algGuidSum16);
    case HSPSupportAlg::Sum24:
      SETLUALGINFO_NORMAL(RefLuAlgInfo, ALGNAMESUM24W, 3, &algGuidSum24);
    case HSPSupportAlg::Sum32:
      SETLUALGINFO_NORMAL(RefLuAlgInfo, ALGNAMESUM32W, 4, &algGuidSum32);
    case HSPSupportAlg::Sum64:
      SETLUALGINFO_NORMAL(RefLuAlgInfo, ALGNAMESUM64W, 8, &algGuidSum64);
    case HSPSupportAlg::SumSysV:
      SETLUALGINFO_NORMAL(RefLuAlgInfo, ALGNAMESUMSYSVW, 2, &algGuidSumSysV);
    case HSPSupportAlg::SumSysVDecimal:
      SETLUALGINFO_NORMAL(RefLuAlgInfo, ALGNAMESUMSYSVDecimalW,
        5 + 1, // UINT16_MAX:65535 = Max 5 characters + null terminator
        &algGuidSumSysVDecimal
      );
  }
  return FALSE;
}


static HSPINLINE BOOL HSPPF_Hash_GetAlgInfo(PHSP_AlgInfo pAlgInfo, uint32_t AlgID)
{
  /*
  switch(static_cast<HSPSupportAlg>(AlgID))
  {
    case HSPSupportAlg::...:
      ...
      return TRUE;
  }
  return FALSE;
  */
  luAlgInfo ai = { false, pAlgInfo };
  return SetluAlgInfo(AlgID, ai);
}


static HSPINLINE BOOL HSPPF_Hash_GetAlgInfoEx(PHSP_AlgInfoEx pAlgInfoEx, uint32_t AlgID)
{
  /*
  switch(static_cast<HSPSupportAlg>(AlgID))
  {
    case HSPSupportAlg::...:
      ...
      return TRUE;
  }
  return FALSE;
  */
  luAlgInfo ai = { true, pAlgInfoEx };
  return SetluAlgInfo(AlgID, ai);
}


// **Required Thread-safe (critical)**
LRESULT HSPCALL HSP_PluginFunc(HSPPFMsg uMsg, WPARAM wParam, LPARAM lParam)
{
  switch(uMsg)
  {
    case HSPPFMsg_General_UseSettingDlg: return FALSE;
    //HSPPF_MSG(HSPPFMsg_General_ShowSettingDlg);
    case HSPPFMsg_Hash_GetSupportAlgCount: return static_cast<LRESULT>(HSPSupportAlg::SupportAlgCount);
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

