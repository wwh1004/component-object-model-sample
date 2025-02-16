//
// A class that implements IComTest.
//_______________________________________________________________________________

#include "pch.h"
#pragma hdrstop

#include <objbase.h>
#include <shlwapi.h>
#include <assert.h>
#include <IComTest_h.h>
#include "ComSampleServerGuids.h"

class
DECLSPEC_UUID("90BF1BF8-D7F6-4AF6-8FE5-6AE1A7B67E63")
CComServerTest : public RuntimeClass<RuntimeClassFlags<ClassicCom>, IComTest, FtmBase>
{
public:

    // IComTest

    IFACEMETHOD(WhoAmI)(_Out_ LPWSTR* ppwszWhoAmI)
    {
        HRESULT hr = (ppwszWhoAmI != nullptr) ? S_OK : E_INVALIDARG;
        if (SUCCEEDED(hr))
        {
            // Create a message string the contains the name of the current process.
            wchar_t wszProcessName[MAX_PATH] = {};
            DWORD dwResult = GetModuleFileNameW(nullptr, wszProcessName, ARRAYSIZE(wszProcessName));
            hr = ((dwResult < ARRAYSIZE(wszProcessName)) && (dwResult != 0)) ? S_OK : HRESULT_FROM_WIN32(GetLastError());
            if (SUCCEEDED(hr))
            {
                LPCWSTR pwszMessagePreface = L"Com Test Server running in process ";
                
                // Prepare the message.
                size_t cchMessagePlusTerminatingNul = wcslen(pwszMessagePreface) + wcslen(wszProcessName) + 1 /* For the terminating NUL character */;
                wchar_t *pwszMessage = (wchar_t *)CoTaskMemAlloc(cchMessagePlusTerminatingNul * sizeof(wchar_t));
                hr = (pwszMessage != nullptr) ? S_OK : E_OUTOFMEMORY;
                if (SUCCEEDED(hr))
                {
                    hr = StringCchPrintfW(pwszMessage, cchMessagePlusTerminatingNul, L"%s%s", pwszMessagePreface, wszProcessName);
                    if (SUCCEEDED(hr))
                    {
						wprintf(L"%s\n", pwszMessage);

                        hr = SHStrDupW(pwszMessage, ppwszWhoAmI);
                    }

                    CoTaskMemFree(pwszMessage);
                }
            }
        }
        
        return hr;
    }
};

CoCreatableClass(CComServerTest);
//_____________________________________________________________________________
