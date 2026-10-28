#pragma once

#include <Windows.h>

class Audio
{
private:
    static HMODULE m_hBass;
    static DWORD   m_hStream;
    static bool    m_bInitialized;

    typedef DWORD (WINAPI *tBASS_StreamCreateFile)(BOOL mem, const void* file, unsigned __int64 offset, unsigned __int64 length, DWORD flags);
    typedef BOOL  (WINAPI *tBASS_ChannelPlay)(DWORD handle, BOOL restart);
    typedef BOOL  (WINAPI *tBASS_ChannelSetAttribute)(DWORD handle, DWORD attrib, float value);
    typedef BOOL  (WINAPI *tBASS_ChannelStop)(DWORD handle);
    typedef BOOL  (WINAPI *tBASS_StreamFree)(DWORD handle);

    static tBASS_StreamCreateFile fnBASS_StreamCreateFile;
    static tBASS_ChannelPlay      fnBASS_ChannelPlay;
    static tBASS_ChannelSetAttribute fnBASS_ChannelSetAttribute;
    static tBASS_ChannelStop      fnBASS_ChannelStop;
    static tBASS_StreamFree       fnBASS_StreamFree;

public:
    static void Init();
    static void PlaySkateSound();
    static void SetVolume(float volume);
    static void StopSkateSound();
};
