#include "Audio.h"

HMODULE Audio::m_hBass = nullptr;
DWORD   Audio::m_hStream = 0;
bool    Audio::m_bInitialized = false;

Audio::tBASS_StreamCreateFile    Audio::fnBASS_StreamCreateFile = nullptr;
Audio::tBASS_ChannelPlay         Audio::fnBASS_ChannelPlay = nullptr;
Audio::tBASS_ChannelSetAttribute Audio::fnBASS_ChannelSetAttribute = nullptr;
Audio::tBASS_ChannelStop         Audio::fnBASS_ChannelStop = nullptr;
Audio::tBASS_StreamFree          Audio::fnBASS_StreamFree = nullptr;

void Audio::Init()
{
    if (m_bInitialized)
        return;

    m_hBass = GetModuleHandleA("bass.dll");
    if (!m_hBass)
        m_hBass = LoadLibraryA("bass.dll");

    if (m_hBass)
    {
        fnBASS_StreamCreateFile    = (tBASS_StreamCreateFile)GetProcAddress(m_hBass, "BASS_StreamCreateFile");
        fnBASS_ChannelPlay         = (tBASS_ChannelPlay)GetProcAddress(m_hBass, "BASS_ChannelPlay");
        fnBASS_ChannelSetAttribute = (tBASS_ChannelSetAttribute)GetProcAddress(m_hBass, "BASS_ChannelSetAttribute");
        fnBASS_ChannelStop         = (tBASS_ChannelStop)GetProcAddress(m_hBass, "BASS_ChannelStop");
        fnBASS_StreamFree          = (tBASS_StreamFree)GetProcAddress(m_hBass, "BASS_StreamFree");
    }

    m_bInitialized = true;
}

void Audio::PlaySkateSound()
{
    Init();

    if (!fnBASS_StreamCreateFile || !fnBASS_ChannelPlay)
        return;

    StopSkateSound();

    const char* audioPaths[] = {
        "cleo\\cleo_audio\\skate.mp3",
        "audio\\skate.mp3",
        "skate.mp3"
    };

    const char* selectedPath = nullptr;
    for (const char* p : audioPaths)
    {
        if (GetFileAttributesA(p) != INVALID_FILE_ATTRIBUTES)
        {
            selectedPath = p;
            break;
        }
    }

    if (!selectedPath)
        selectedPath = audioPaths[0];

    // 4 = BASS_SAMPLE_LOOP
    m_hStream = fnBASS_StreamCreateFile(FALSE, selectedPath, 0, 0, 4);
    if (m_hStream)
    {
        // 2 = BASS_ATTRIB_VOL
        if (fnBASS_ChannelSetAttribute)
            fnBASS_ChannelSetAttribute(m_hStream, 2, 0.0f);

        fnBASS_ChannelPlay(m_hStream, FALSE);
    }
}

void Audio::SetVolume(float volume)
{
    if (!m_hStream || !fnBASS_ChannelSetAttribute)
        return;

    if (volume < 0.0f) volume = 0.0f;
    if (volume > 1.0f) volume = 1.0f;

    // 2 = BASS_ATTRIB_VOL
    fnBASS_ChannelSetAttribute(m_hStream, 2, volume);
}

void Audio::StopSkateSound()
{
    if (!m_hStream)
        return;

    if (fnBASS_ChannelStop)
        fnBASS_ChannelStop(m_hStream);

    if (fnBASS_StreamFree)
        fnBASS_StreamFree(m_hStream);

    m_hStream = 0;
}
