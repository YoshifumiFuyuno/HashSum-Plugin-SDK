/**
 * HashSum Plugin SDK
 *
 * Written by Yoshifumi Fuyuno <http://hp.vector.co.jp/authors/VA052754/>, 2014.
 * License: CC0(Public Domain) <https://creativecommons.org/publicdomain/zero/1.0/>
 */

#include "SamplePluginAlg.h"
#include <stdlib.h>
#include <wchar.h> //snwprintf
#include <windows.h>


// **All Functions Required Thread-safe (critical)**


void* HSPCALL HSP_HashInitialize(uint32_t AlgID)
{
  UNREFERENCED_PARAMETER(AlgID);
  return calloc(1, sizeof(uint_fast16_t));
}


void HSPCALL HSP_HashUpdate(void *state, const uint8_t *data, rsize_t dataOctets)
{
  uint_fast16_t* const pSum = (uint_fast16_t*)state;
  while(dataOctets--)
    //*pSum = ( ((*pSum >> 1) | (*pSum << 15)) + *data++) & 0xFFFF;
    *pSum = ((*pSum >> 1) + (*pSum << 15) + *data++) & 0xFFFF; // use ROR instruction (MinGW (GCC 4.8.1))
}


void HSPCALL HSP_HashGetHex(void *state, uint8_t *digest, rsize_t getOctets)
{
  for(rsize_t i = 0; i < getOctets; ++i)
    digest[i] = *(const uint_fast16_t*)state >> 8 * (1 - i) & 0xFF;
}


void HSPCALL HSP_HashGetString(void *state, HSPCHAR *szDigest, rsize_t getLen, bool bUppercase)
{
  UNREFERENCED_PARAMETER(bUppercase);
  _snwprintf((APICHAR*)szDigest, getLen, U16SLAC("%u"), *(const uint_fast16_t*)state);
  *(szDigest + getLen - 1) = U16SL('\0');
}


void HSPCALL HSP_HashReset(void *state)
{
  *(uint_fast16_t*)state = 0;
}


void HSPCALL HSP_HashFinalize(void *state)
{
  free(state);
}


void* HSPCALL HSP_HashClone(void *state)
{
  uint_fast16_t* const pClone = malloc(sizeof(uint_fast16_t));
  if(pClone)
    *pClone = *(uint_fast16_t*)state;
  return pClone;
}
