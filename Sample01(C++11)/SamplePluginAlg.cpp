/**
 * HashSum Plugin SDK
 *
 * Written by Yoshifumi Fuyuno <http://hp.vector.co.jp/authors/VA052754/>, 2014.
 * License: CC0(Public Domain) <https://creativecommons.org/publicdomain/zero/1.0/>
 */

#include "SamplePluginAlg.h"
#include <new>
#include <wchar.h> //snwprintf, swprintf
#include <windows.h>


// **All Functions Required Thread-safe (critical)**


class IAlgBase
{
public:
  virtual ~IAlgBase(){}
  virtual IAlgBase* Clone() const = 0;

  virtual void Update(const uint8_t *data, rsize_t dataOctets) = 0;
  virtual void GetHex(uint8_t *digest, rsize_t getOctets) const = 0;
  virtual void Reset() = 0;
};

template <class _Ty, rsize_t digestOctets = sizeof(_Ty)> class CSum : public IAlgBase
{
private:
  _Ty m_sum;
protected:
  _Ty GetSum() const {return m_sum;}
public:
  CSum() : m_sum(0) {}
  CSum* Clone() const override {return new CSum(*this);}
  void Update(const uint8_t *data, rsize_t dataOctets) override
  {
    while(dataOctets--)
      m_sum += *data++;
  }
  void GetHex(uint8_t *digest, rsize_t getOctets) const override
  {
    for(rsize_t i = 0; i < getOctets; ++i)
      /*
      digest[i] = ((const uint8_t *)&m_sum)[digestOctets - 1 - i];
      /*/
      digest[i] = m_sum >> 8 * (digestOctets - 1 - i) & 0xFF;
      //*/
  }
  void Reset() override {m_sum = 0;}
};

class CSumSysV : public CSum<uint_fast32_t>
{
private:
  uint16_t GetSYSVchecksum() const
  {
    const uint32_t r = (GetSum() & 0xFFFF) + ((GetSum() >> 16) & 0xFFFF);
    return (r & 0xFFFF) + (r >> 16);
  }
public:
  CSumSysV* Clone() const override {return new CSumSysV(*this);}
  void GetHex(uint8_t *digest, rsize_t getOctets) const override
  {
    const uint16_t checksum = GetSYSVchecksum();
    for(rsize_t i = 0; i < getOctets; ++i)
      /*
      digest[i] = ((const uint8_t *)&checksum)[sizeof(checksum) - 1 - i];
      /*/
      digest[i] = checksum >> 8 * (sizeof(checksum) - 1 - i) & 0xFF;
      //*/
  }
  void GetString(HSPCHAR *szDigest, rsize_t getLen, bool bUppercase) const
  {
    UNREFERENCED_PARAMETER(bUppercase);
    _snwprintf((APICHAR*)szDigest, getLen, U16SLAC("%u"), GetSYSVchecksum()); //_snwprintf(), swprintf()
    *(szDigest + getLen - 1) = U16SL('\0');
  }
};

typedef CSum<uint_fast8_t, 1> CSum8;
typedef CSum<uint_fast16_t, 2> CSum16;
typedef CSum<uint_fast32_t, 3> CSum24;
typedef CSum<uint_fast32_t, 4> CSum32;
typedef CSum<uint_fast64_t, 8> CSum64;


void* HSPCALL HSP_HashInitialize(uint32_t AlgID)
{
  try
  {
    switch(static_cast<HSPSupportAlg>(AlgID))
    {
      case HSPSupportAlg::Sum8:
        return new CSum8;
      case HSPSupportAlg::Sum16:
        return new CSum16;
      case HSPSupportAlg::Sum24:
        return new CSum24;
      case HSPSupportAlg::Sum32:
        return new CSum32;
      case HSPSupportAlg::Sum64:
        return new CSum64;
      case HSPSupportAlg::SumSysV:
        return new CSumSysV;
      case HSPSupportAlg::SumSysVDecimal:
        return new CSumSysV;
    }
  }
  catch(std::bad_alloc&){}
  return nullptr;
}


void HSPCALL HSP_HashUpdate(void *state, const uint8_t *data, rsize_t dataOctets)
{
  static_cast<IAlgBase*>(state)->Update(data, dataOctets);
}


void HSPCALL HSP_HashGetHex(void *state, uint8_t *digest, rsize_t getOctets)
{
  static_cast<IAlgBase*>(state)->GetHex(digest, getOctets);
}


void  HSPCALL HSP_HashGetString(void *state, HSPCHAR *szDigest, rsize_t getLen, bool bUppercase)
{
  static_cast<CSumSysV*>(state)->GetString(szDigest, getLen, bUppercase);
}


void HSPCALL HSP_HashReset(void *state)
{
  static_cast<IAlgBase*>(state)->Reset();
}


void HSPCALL HSP_HashFinalize(void *state)
{
  delete static_cast<IAlgBase*>(state);
}


void* HSPCALL HSP_HashClone(void *state)
{
  try
  {
    return static_cast<IAlgBase*>(state)->Clone();
  }
  catch(std::bad_alloc&)
  {
    return nullptr;
  }
}
