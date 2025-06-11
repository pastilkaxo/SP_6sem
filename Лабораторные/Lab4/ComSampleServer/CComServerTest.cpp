//
// A class that implements IComTest.
//_______________________________________________________________________________

#include "pch.h"
#pragma hdrstop

#include "ComSampleServerCreateInstances.h"
#include "ComSampleServerGuids.h"
#include "Dll.h"
#include <IComTest_h.h>
#include <IComTest.h>
#include <assert.h>
#include <objbase.h>
#include <shlwapi.h>


class CComServerTest : public IComTest , public ILVO {
public:
  // IUnknown

  IFACEMETHODIMP_(ULONG) AddRef() { return InterlockedIncrement(&_cRef); }

  IFACEMETHODIMP_(ULONG) Release() {
    assert(_cRef > 0);

    ULONG cRef = InterlockedDecrement(&_cRef);

    if (0 == cRef) {
      delete this;
    }

    return cRef;
  }

  IFACEMETHODIMP QueryInterface(__in REFIID riid, __out void **ppv) {
#if 0
        static const QITAB qit[] =
        {
            QITABENT(CComServerTest, IComTest),
            QITABENT(CComServiceTest, ILVO),
            { 0 },
        };

        return QISearch(this, qit, riid, ppv);
#endif
    // ANOTHER POSSIBLE IMPLEMENTATION FOR QueryInterface METHOD.
    HRESULT hr = (ppv != nullptr) ? S_OK : E_INVALIDARG;
    if (SUCCEEDED(hr)) {
      *ppv = nullptr;
      hr = E_NOINTERFACE;

      if (__uuidof(IComTest) == riid) {
        *ppv = static_cast<IComTest *>(this);
        hr = S_OK;
      }
      else if (__uuidof(ILVO) == riid)
      {
          *ppv = static_cast<ILVO*>(this);
          hr = S_OK;
      }
      else if (__uuidof(IUnknown) == riid){
          *ppv = static_cast<IUnknown*>(static_cast<IComTest*>(this));
          hr = S_OK;
      }

      if (SUCCEEDED(hr)) {
        reinterpret_cast<IUnknown *>(*ppv)->AddRef();
      }
    }

    return hr;
  }

  // IComTest
  IFACEMETHODIMP WhoAmI(_Out_ LPWSTR *ppwszWhoAmI) {
    HRESULT hr = (ppwszWhoAmI != nullptr) ? S_OK : E_INVALIDARG;
    if (SUCCEEDED(hr)) {
      // Create a message string the contains the name of the current process.
      wchar_t wszProcessName[MAX_PATH] = {};
      DWORD dwResult = GetModuleFileNameW(nullptr, wszProcessName,
                                          ARRAYSIZE(wszProcessName));
      hr = ((dwResult < ARRAYSIZE(wszProcessName)) && (dwResult != 0))
               ? S_OK
               : HRESULT_FROM_WIN32(GetLastError());
      if (SUCCEEDED(hr)) {
        LPCWSTR pwszMessagePreface = L"Com Test Server running in process ";

        // Prepare the message.
        size_t cchMessagePlusTerminatingNul =
            wcslen(pwszMessagePreface) + wcslen(wszProcessName) +
            1 /* For the terminating NUL character */;
        wchar_t *pwszMessage = (wchar_t *)CoTaskMemAlloc(
            cchMessagePlusTerminatingNul * sizeof(wchar_t));
        hr = (pwszMessage != nullptr) ? S_OK : E_OUTOFMEMORY;
        if (SUCCEEDED(hr)) {
          hr = StringCchPrintfW(pwszMessage, cchMessagePlusTerminatingNul,
                                L"%s%s", pwszMessagePreface, wszProcessName);
          if (SUCCEEDED(hr)) {
            hr = SHStrDupW(pwszMessage, ppwszWhoAmI);
          }

          CoTaskMemFree(pwszMessage);
        }
      }
    }

    return hr;
  }

 // ILVO

  IFACEMETHODIMP PowerOfN(double x,  double* x5 , double* x13) {
      if (!x5 || !x13) return E_POINTER;  // Проверка на нулевой указатель


          // Мы можем вычислить x^5 за 3 операции: x^2, x^4 и x^5
          double x2 = x * x;        // x^2
          double x4 = x2 * x2;      // x^4
          *x5 = x4 * x;         // x^5 = x^4 * x
          double x_5 = x4 * x;       // x^5 = x^4 * x
          double x8 = x4 * x4;      // x^8 = x^4 * x^4
          *x13 = x8 * x_5;        // x^13 = x^8 * x^5

      return S_OK;
  }

public:
  CComServerTest() : _cRef(1) {
    InterlockedIncrement(&g_cRefDll); // g_cRefDll++;
  }

private:
  LONG _cRef;

  ~CComServerTest(void) {
    InterlockedDecrement(&g_cRefDll); // g_cRefDll--;
  }
};
//_____________________________________________________________________________

//  Creation function
HRESULT CComServerTest_CreateInstance(__in REFIID riid, __out void **ppv) {
  HRESULT hr = E_OUTOFMEMORY;
  CComServerTest *pInstance = new CComServerTest();
  if (pInstance != nullptr) {
    hr = pInstance->QueryInterface(riid, ppv);
    pInstance->Release();
  }

  return hr;
}
//_____________________________________________________________________________