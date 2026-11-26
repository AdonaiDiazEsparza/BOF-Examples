#include <Windows.h>
#include "base\helpers.h"


#ifdef _DEBUG
#include "base\mock.h"
#pragma comment(lib, "User32.lib")
#undef DECLSPEC_IMPORT
#define DECLSPEC_IMPORT
#endif

extern "C" {
    #include "beacon.h"

    DFR(USER32, MessageBoxA);
    #define MessageBoxA USER32$MessageBoxA

    // Entry point
    void go(char* args, int len);
}

void go(char* args, int len) {
    
    BeaconPrintf(CALLBACK_OUTPUT, "BoF_MessageBox\n");

    char text[19] = { 'H','o','l','a',' ','d','e','s','d','e',' ','u','n',' ','B','O','F','!',0 };
    char title[11] = { 'B','e','a','c','o','n',' ','B','O','F',0 };

    MessageBoxA(
        NULL,                       
        text,       
        title,               
        MB_OK | MB_ICONINFORMATION  
    );
}

// Define a main function for the bebug build
#if defined(_DEBUG) && !defined(_GTEST)

int main(int argc, char* argv[]) {
    // Run BOF's entrypoint
    // To pack arguments for the bof use e.g.: bof::runMocked<int, short, const char*>(go, 6502, 42, "foobar");
    bof::runMocked<>(go);
    return 0;
}

#endif