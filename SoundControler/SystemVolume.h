#pragma once
#include "std.h"
class SystemVolume
{
private:
    float m_initVolume;
    float m_currentVolume;
    IAudioEndpointVolume* m_endpointVolume = NULL;
public:
    float GetVolume();
    bool ChangeVolume(double nVolume, bool bScalar);
    bool ChangeVolumeUp();
    bool ChangeVolumeDown();

    ~SystemVolume();
    SystemVolume();
};

