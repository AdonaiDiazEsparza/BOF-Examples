#include <Windows.h>
#include "base\helpers.h"


#ifdef _DEBUG

// For debug include missing libraries
#pragma comment(lib, "Advapi32.lib")

#include "base\mock.h"

#undef DECLSPEC_IMPORT
#define DECLSPEC_IMPORT
#endif

extern "C" {
#include "beacon.h"

    DFR(ADVAPI32, GetUserNameA);
    #define GetUserNameA ADVAPI32$GetUserNameA

    DFR(KERNEL32, GetLastError);
    #define GetLastError KERNEL32$GetLastError

    DFR(KERNEL32, GetCurrentProcess);
    #define GetCurrentProcess KERNEL32$GetCurrentProcess

    DFR(ADVAPI32, OpenProcessToken);
    #define OpenProcessToken ADVAPI32$OpenProcessToken

    DFR(ADVAPI32, GetTokenInformation);
    #define GetTokenInformation ADVAPI32$GetTokenInformation

    DFR(ADVAPI32, LookupPrivilegeNameA);
    #define LookupPrivilegeNameA ADVAPI32$LookupPrivilegeNameA

    DFR(KERNEL32, CloseHandle);
    #define CloseHandle KERNEL32$CloseHandle

    DFR(MSVCRT, malloc);
    #define malloc MSVCRT$malloc

    DFR(MSVCRT, free);
    #define free MSVCRT$free

    // Entry point
    void go(char* args, int len);
}

bool EnumeratePrivileges()
{
    HANDLE hToken = NULL;
    DWORD dwLength = 0;

    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
    {
        BeaconPrintf(CALLBACK_ERROR, "OpenProcessToken failed: %lu\n", GetLastError());
        return false;
    }

    GetTokenInformation(hToken, TokenPrivileges, NULL, 0, &dwLength);

    if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
    {
        BeaconPrintf(CALLBACK_ERROR, "GetTokenInformation (size) failed: %lu\n", GetLastError());
        CloseHandle(hToken);
        return false;
    }

    PTOKEN_PRIVILEGES pPrivileges = (PTOKEN_PRIVILEGES)malloc(dwLength);
    if (pPrivileges == NULL)
    {
        BeaconPrintf(CALLBACK_ERROR, "malloc failed\n");
        CloseHandle(hToken);
        return false;
    }

    if (!GetTokenInformation(hToken, TokenPrivileges, pPrivileges, dwLength, &dwLength))
    {
        BeaconPrintf(CALLBACK_ERROR, "GetTokenInformation failed: %lu\n", GetLastError());
        free(pPrivileges);
        CloseHandle(hToken);
        return false;
    }

    BeaconPrintf(CALLBACK_OUTPUT, "%-40s %s\n", "Privilege Name", "State");
    BeaconPrintf(CALLBACK_OUTPUT, "----------------------------------------------------------\n");

    for (DWORD i = 0; i < pPrivileges->PrivilegeCount; i++)
    {
        char privilegeName[128];
        DWORD nameLength = sizeof(privilegeName);   // 128 bytes

        LUID  luid = pPrivileges->Privileges[i].Luid;
        DWORD attributes = pPrivileges->Privileges[i].Attributes;

        if (LookupPrivilegeNameA(NULL, &luid, privilegeName, &nameLength))
        {
            const char* state = (attributes & SE_PRIVILEGE_ENABLED) ? "Enabled" : "Disabled";

            BeaconPrintf(CALLBACK_OUTPUT, "%-40s %s\n", privilegeName, state);
        }
        else
        {
            BeaconPrintf(CALLBACK_ERROR, "LookupPrivilegeNameA failed: %lu\n", GetLastError());
        }
    }

    free(pPrivileges);
    CloseHandle(hToken);
    return true;
}

// Funcion para conseguir el nombre de usuario
bool Whoami()
{

    char username[256];
    DWORD usernameLen = sizeof(username);

    if (!GetUserNameA(username, &usernameLen)) {
        BeaconPrintf(CALLBACK_ERROR, "GetUserNameA failed: %d\n", GetLastError());
        return false;
    }

    BeaconPrintf(CALLBACK_OUTPUT, "%s\n", username);

    return true;
}


void go(char* args, int len) {
    BeaconPrintf(CALLBACK_OUTPUT, "BoF_Whoami_Privs\n");

    if (!Whoami()) {
        BeaconPrintf(CALLBACK_ERROR, "Failed to get username\n");

    }

    
    if (!EnumeratePrivileges()){
            BeaconPrintf(CALLBACK_ERROR, "Failed to enumerate privileges\n");
    }

}

// Define a main function for the bebug build
#if defined(_DEBUG) && !defined(_GTEST)

int main(int argc, char* argv[]) {
    // Run BOF's entrypoint
    // To pack arguments for the bof use e.g.: bof::runMocked<int, short, const char*>(go, 6502, 42, "foobar");
    bof::runMocked<>(go);
    return 0;
}

// Define unit tests
#elif defined(_GTEST)
#include <gtest\gtest.h>

TEST(BofTest, Test1) {
    std::vector<bof::output::OutputEntry> got =
        bof::runMocked<>(go);
    std::vector<bof::output::OutputEntry> expected = {
        { CALLBACK_OUTPUT, "System Directory: C:\\Windows\\system32" }
    };
    // It is possible to compare the OutputEntry vectors, like directly
    // ASSERT_EQ(expected, got);
    // However, in this case, we want to compare the output, ignoring the case.
    ASSERT_EQ(expected.size(), got.size());
    ASSERT_STRCASEEQ(expected[0].output.c_str(), got[0].output.c_str());
}
#endif