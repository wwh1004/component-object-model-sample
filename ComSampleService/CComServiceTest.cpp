//
// A class that implements IComTest.
//_______________________________________________________________________________

#include "pch.h"
#pragma hdrstop

#include <IComTest_h.h>
#include "ComSampleServiceGuids.h"

class
	DECLSPEC_UUID("3B6A73AC-92C2-42AC-AACA-C9A423FE0D05")
    CComServiceTest : public RuntimeClass<RuntimeClassFlags<ClassicCom>, IComTest, FtmBase>
{
public:

    // IComTest

    IFACEMETHODIMP WhoAmI(_Out_ LPWSTR *ppwszWhoAmI)
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
                LPCWSTR pwszMessagePreface = L"Com Test Service running in process ";
                
                // Prepare the message.
                size_t cchMessagePlusTerminatingNul = wcslen(pwszMessagePreface) + wcslen(wszProcessName) + 1 /* For the terminating NUL character */;
                wchar_t *pwszMessage = (wchar_t *)CoTaskMemAlloc(cchMessagePlusTerminatingNul * sizeof(wchar_t));
                hr = (pwszMessage != nullptr) ? S_OK : E_OUTOFMEMORY;
                if (SUCCEEDED(hr))
                {
                    hr = StringCchPrintfW(pwszMessage, cchMessagePlusTerminatingNul, L"%s%s", pwszMessagePreface, wszProcessName);
                    if (SUCCEEDED(hr))
                    {
                        hr = SHStrDupW(pwszMessage, ppwszWhoAmI);
                    }

                    CoTaskMemFree(pwszMessage);
                }
            }
        }
        
        return hr;
    }
};

CoCreatableClass(CComServiceTest);
//_____________________________________________________________________________
