/**
 * HashSum Plugin SDK
 *
 * Written by Yoshifumi Fuyuno <http://hp.vector.co.jp/authors/VA052754/>, 2014.
 * License: Mozilla Public License Version 2.0 <https://www.mozilla.org/MPL/2.0/>
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __HSPSDK_CRACKERS_H
#define __HSPSDK_CRACKERS_H

/****** Message crackers ****************************************************/

#define HSPPF_MSG(message)    \
    case (message): return HSPPF_##message((wParam), (lParam))


/* static HSPINLINE BOOL HSPPF_General_ShowSettingDlg(HWND hWndParent) */
#define HSPPF_HSPPFMsg_General_ShowSettingDlg(wParam, lParam) \
    (HSPPF_General_ShowSettingDlg((HWND)(wParam)))

/* static HSPINLINE BOOL HSPPF_Hash_UseSettingDlg(uint32_t AlgID) */
#define HSPPF_HSPPFMsg_Hash_UseSettingDlg(wParam, lParam) \
    (HSPPF_Hash_UseSettingDlg((uint32_t)(lParam)))

/* static HSPINLINE BOOL HSPPF_Hash_ShowSettingDlg(HWND hWndParent, uint32_t AlgID) */
#define HSPPF_HSPPFMsg_Hash_ShowSettingDlg(wParam, lParam) \
    (HSPPF_Hash_ShowSettingDlg((HWND)(wParam), (uint32_t)(lParam)))

/* static HSPINLINE BOOL HSPPF_Hash_GetAlgFunctions(PHSP_AlgFunctions pAlgFunctions, uint32_t AlgID) */
#define HSPPF_HSPPFMsg_Hash_GetAlgFunctions(wParam, lParam) \
    (HSPPF_Hash_GetAlgFunctions((PHSP_AlgFunctions)(wParam), (uint32_t)(lParam)))

/* static HSPINLINE BOOL HSPPF_Hash_GetAlgInfo(PHSP_AlgInfo pAlgInfo, uint32_t AlgID) */
#define HSPPF_HSPPFMsg_Hash_GetAlgInfo(wParam, lParam) \
    (HSPPF_Hash_GetAlgInfo((PHSP_AlgInfo)(wParam), (uint32_t)(lParam)))

/* static HSPINLINE BOOL HSPPF_Hash_GetAlgInfoEx(PHSP_AlgInfoEx pAlgInfoEx, uint32_t AlgID) */
#define HSPPF_HSPPFMsg_Hash_GetAlgInfoEx(wParam, lParam) \
    (HSPPF_Hash_GetAlgInfoEx((PHSP_AlgInfoEx)(wParam), (uint32_t)(lParam)))

/****** FPHSPProcActionHandler message APIs ****************************************/

#define HSPPAH_GetHostName(fpHandler)          ((const HSPCHAR*)((*fpHandler)(HSPPAHMsg_GetHostName, 0L, 0L)))
#define HSPPAH_GetHostVersion(fpHandler)       ((uint32_t)((*fpHandler)(HSPPAHMsg_GetHostVersion, 0L, 0L)))
#define HSPPAH_GetMainWindowHandle(fpHandler)  ((HWND)((*fpHandler)(HSPPAHMsg_GetMainWindowHandle, 0L, 0L)))
#define HSPPAH_HashNowCalc(fpHandler)          ((BOOL)((*fpHandler)(HSPPAHMsg_HashNowCalc, 0L, 0L)))

/****** Alternate porting layer macros ****************************************/

#define GET_HostMajorVersion(dwVer)            HIWORD(dwVer)
#define GET_HostMinorVersion(dwVer)            LOWORD(dwVer)

#endif // __HSPSDK_CRACKERS_H

