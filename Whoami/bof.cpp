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


    // Entry point
    void go(char* args, int len);
}


// Funcion para conseguir el nombre de usuario
bool Whoami()
{

    char username[256];
    DWORD usernameLen = sizeof(username);

    if (!GetUserNameA(username, &usernameLen)) {
        BeaconPrintf(CALLBACK_ERROR, "GetUserNameA failed: %d", GetLastError());
        return false;
    }

    BeaconPrintf(CALLBACK_OUTPUT, "%s", username);

    return true;
}

void go(char* args, int len) {
    BeaconPrintf(CALLBACK_OUTPUT, "BoF_Whoami");

    if(!Whoami())
    {
        // Si algo estuvo mal imprime el error
        BeaconPrintf(CALLBACK_ERROR, "Failed to get username");
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