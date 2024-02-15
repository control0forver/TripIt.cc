#include <pch.hpp>

#include <hooks/input/wndproc/wndproc_hooks.hpp>

#include <utils/sound/sound.hpp>
#include <utils/sound/wavreader/wavreader.hpp>

#include <utils/logger/logger.hpp>

#include <cstdio>  // FILE

#pragma comment(lib, "Dsound.lib")

void CSound::InitializeDirectSound(HWND window) {
    if (DirectSoundCreate8(NULL, &m_hSound, NULL) != DS_OK) {
        CLogger::Get().Log2("[sound] Failed to create DirectSound object");
        return;
    }

    if (m_hSound->SetCooperativeLevel(CWndProcHooks::Get().GetWindow(), DSSCL_NORMAL) != DS_OK) {
        CLogger::Get().Log2("[sound] Failed to set DirectSound cooperative level");
        return;
    }
}

void CSound::ReleaseDirectSound() {
    if (m_hSound) {
        m_hSound->SetCooperativeLevel(CWndProcHooks::Get().GetWindow(), NULL);
        m_hSound->Release();
        m_hSound = NULL;
    }
}

CSound::~CSound() {
    FreeAllSounds();
    ReleaseDirectSound();
}

void CSound::Initialize(HWND window) { InitializeDirectSound(window); }

void CSound::Shutdown() {
    FreeAllSounds();
    ReleaseDirectSound();
}

bool CSound::LoadSoundFromMemory(SoundController& controller, SoundID id, const void* data, size_t dataSize) {
    auto readWav = LightweightWaveReader::WaveReader(static_cast<const uint8_t*>(data), dataSize);
    if (!readWav.readHeader()) {
        CLogger::Get().Log2("[sound] '{}' load failed, invalid wav data header", id);
        return false;
    }
    auto wavFormat = readWav.getFormat();

    SoundData& soundData = m_GlobalSounds[id];
    if (soundData.size()) {
        if (IsSoundExists(controller.GetSounds(), id)) {
            CLogger::Get().Log2("[sound] '{}' load failed, already exists", id);
            return false;
        }

        controller.AddSound(id, soundData);
        return true;
    }

    for (size_t i = 0; i < controller.GetBufferCopysPerSound(); i++) {
        DSBUFFERDESC dsbd;
        ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
        dsbd.dwSize = sizeof(DSBUFFERDESC);
        dsbd.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_STATIC;
        dsbd.dwBufferBytes = wavFormat.BytesRate;
        dsbd.lpwfxFormat = reinterpret_cast<WAVEFORMATEX*>(&wavFormat);
        dsbd.guid3DAlgorithm = GUID_NULL;

        LPDIRECTSOUNDBUFFER lpDSB;
        if (m_hSound->CreateSoundBuffer(&dsbd, &lpDSB, NULL) != DS_OK) {
            CLogger::Get().Log2("[sound] '{}' load failed, cannot create dsound buffer", id);
            return false;
        }

        void* lpvPtr1;
        DWORD dwSize1;
        if (lpDSB->Lock(0, (DWORD)wavFormat.BytesRate, &lpvPtr1, &dwSize1, NULL, NULL, 0) != DS_OK) {
            CLogger::Get().Log2("[sound] '{}' load failed, cannot lock dsound buffer", id);
            lpDSB->Release();
            return false;
        }

        memcpy(lpvPtr1, data, wavFormat.BytesRate);

        if (lpDSB->Unlock(lpvPtr1, dwSize1, NULL, 0) != DS_OK) {
            CLogger::Get().Log2("[sound] '{}' load failed, cannot unlock dsound buffer", id);
            lpDSB->Release();
            return false;
        }

        soundData.push_back(lpDSB);
    }
    controller.AddSound(id, soundData);

    return true;
}

bool CSound::LoadSoundFromFile(SoundController& controller, SoundID id, const char* file) {
    FILE* pFile;
    auto errFileOpen = fopen_s(&pFile, file, "r");
    if (errFileOpen != 0) {
        CLogger::Get().Log2("[sound] '{}' load failed, cannot open file for file \"{}\" ({})", id, file, errFileOpen);
        return false;
    }

    fseek(pFile, 0, SEEK_END);
    auto fileSize = ftell(pFile);
    fseek(pFile, 0, SEEK_SET);

    char* fileBuffer = (char*)malloc(fileSize);
    if (!fileBuffer) {
        CLogger::Get().Log2("[sound] '{}' load failed, cannot allocate memory for file \"{}\" ({} bytes)", id, file, fileSize);
        fclose(pFile);
        return false;
    }

    size_t bytesRead = fread_s(fileBuffer, fileSize, 1, fileSize, pFile);
    if (bytesRead != fileSize) {
        CLogger::Get().Log2("[sound] '{}' load failed, cannot read entire file \"{}\"", id, file);
        fclose(pFile);
        free(fileBuffer);
        return false;
    }
    fclose(pFile);

    bool r = LoadSoundFromMemory(controller, id, fileBuffer, fileSize);

    free(fileBuffer);
    return r;
}

void CSound::SoundController::QueuePlayAll() {
    while (!empty()) {
        auto resc = pop();
        if (!resc) continue;

        resc->Stop();
        resc->Restore();
        resc->Play(0, 0, NULL);
    }
    return;
}

void CSound::SoundController::QueuePopSound(size_t pops, bool waitForEmpty) {
    for (size_t i = 0; i < pops; i++) {
        pop(waitForEmpty);
    }
}

void CSound::SoundController::QueuePushSound(SoundID id, bool waitForFull) {
    auto it = m_Sounds.find(id);
    if (it == m_Sounds.end()) {
        CLogger::Get().Log2("[sound] Cannot play sound '{}' not found", id);
        return;
    }

    push(it->second[m_Index], waitForFull);
    m_Index++;
    if (m_Index >= BufferCopysPerSound) m_Index = 0;

    // TOOOOOOOOO slow
    /*
    auto buffers = it->second;
    for (size_t i = 0; i < buffers.size(); i++) {
        const auto sb = buffers[i];

        DWORD _s{};
        if (FAILED(sb->GetStatus(&_s))) continue;
        if ((_s & DSBSTATUS_PLAYING) == 1) continue;

        push(sb, waitForFull);
    }
    */

    /*if (rPlay != DS_OK) {
        CLogger::Get().Log2("[sound] Cannot play sound '{}' ({})", name, rPlay);
        return;
    }*/
}

void CSound::SoundController::AddSound(SoundID id, const SoundData& data) { 
    SoundData& sd = m_Sounds[id];
    if (sd.size()) {
        CLogger::Get().Log2("[sound] '{}' add failed, already exists", id);
        return;
    }

    sd = data;
}

void CSound::SoundController::RemoveSound(SoundID id) { m_Sounds[id] = {}; }

// void CSound::SoundStop(SoundID name) {
//     auto it = m_Sounds.find(name);
//     if (it == m_Sounds.end()) {
//         CLogger::Get().Log2("[sound] Cannot stop sound '{}' not found", name);
//         return;
//     }
//
//     auto rPlay = it->second->Stop();
// }

void CSound::FreeSound(SoundID id) {
    auto it = m_GlobalSounds.find(id);
    if (it != m_GlobalSounds.end()) {
        for (const SoundResc& iit : it->second) {
            iit->Stop();
            iit->Release();
        }
        m_GlobalSounds.erase(it);
    }
}

void CSound::FreeAllSounds() {
    for (auto& pair : m_GlobalSounds) {
        for (const SoundResc& iit : pair.second) {
            iit->Stop();
            iit->Release();
        }
    }
    m_GlobalSounds.clear();
}

const CSound::SoundHinstance& CSound::GetSoundHinstance() { return m_hSound; }

const CSound::SoundsCollection& CSound::GetGlobalSounds() { return m_GlobalSounds; }
