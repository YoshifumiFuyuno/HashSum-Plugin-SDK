/**
 * HashSum Plugin SDK
 *
 * Written by Yoshifumi Fuyuno <http://hp.vector.co.jp/authors/VA052754/>, 2014.
 * License: CC0(Public Domain) <https://creativecommons.org/publicdomain/zero/1.0/>
 */

#ifndef __SAMPLEPLUGIN_H
#define __SAMPLEPLUGIN_H

#include "..\Plugin_SDK_Base.h"

#if defined(__cplusplus)
extern "C" {
#endif

enum HSPSupportAlg
{
  HSPSupportAlg_SumBSD,
  HSPSupportAlg_SumBSDDecimal,
  HSPSupportAlg_SupportAlgCount
};


// **All Functions Required Thread-safe (critical)**
void* HSPCALL HSP_HashInitialize(uint32_t AlgID);
void  HSPCALL HSP_HashUpdate(void *state, const uint8_t *data, rsize_t dataOctets);
void  HSPCALL HSP_HashGetHex(void *state, uint8_t *digest, rsize_t getOctets);
void  HSPCALL HSP_HashGetString(void *state, HSPCHAR *szDigest, rsize_t getLen, bool bUppercase);
void  HSPCALL HSP_HashReset(void *state);
void  HSPCALL HSP_HashFinalize(void *state);
void* HSPCALL HSP_HashClone(void *state);

#if defined(__cplusplus)
}
#endif

#endif // __SAMPLEPLUGIN_H
