#include "SystemVolume.h"
float SystemVolume::GetVolume()
{
    HRESULT hr;
    hr = m_endpointVolume->GetMasterVolumeLevelScalar(&m_currentVolume);
    return m_currentVolume;
}
bool SystemVolume::ChangeVolume(double nVolume, bool bScalar)
{

    double newVolume = nVolume;
    HRESULT hr;

    // -------------------------
    //m_endpointVolume->GetMasterVolumeLevel(&m_currentVolume);
    //printf("Current volume in dB is: %f\n", m_currentVolume);

    hr = m_endpointVolume->GetMasterVolumeLevelScalar(&m_currentVolume);
    //CString strCur=L"";
    //strCur.Format(L"%f",m_currentVolume);
    //AfxMessageBox(strCur);

    // printf("Current volume as a scalar is: %f\n", m_currentVolume);
    if (bScalar == false)
    {
        hr = m_endpointVolume->SetMasterVolumeLevel((float)newVolume, NULL);
    }
    else if (bScalar == true)
    {
        //get ÀÖÀ½
        hr = m_endpointVolume->SetMasterVolumeLevelScalar((float)newVolume, NULL);
    }
    return true;
}
bool SystemVolume::ChangeVolumeUp()
{

    float newVolume;
    HRESULT hr;

    // ÇöÀç º¼·ý Ãëµæ
    hr = m_endpointVolume->GetMasterVolumeLevelScalar(&m_currentVolume);
    // 1¾¿ Á¶Àý
    newVolume = m_currentVolume + 0.01f;
    if (newVolume >= 1.0f)
    {
        newVolume = 1.0f;
    }
    m_currentVolume = newVolume;
    // º¼·ý Àû¿ë
    hr = m_endpointVolume->SetMasterVolumeLevelScalar(newVolume, NULL);
    return true;
}

bool SystemVolume::ChangeVolumeDown()
{

    float newVolume;
    HRESULT hr;

    // ÇöÀç º¼·ý Ãëµæ
    hr = m_endpointVolume->GetMasterVolumeLevelScalar(&m_currentVolume);
    // 1¾¿ Á¶Àý
    newVolume = m_currentVolume - 0.01f;
    if (newVolume <= 0.0f)
    {
        newVolume = 0.0f;
    }
    m_currentVolume = newVolume;
    // º¼·ý Àû¿ë
    hr = m_endpointVolume->SetMasterVolumeLevelScalar(newVolume, NULL);
    return true;
}
SystemVolume::~SystemVolume()
{
    m_endpointVolume->Release();
    CoUninitialize();
}
SystemVolume::SystemVolume()
{
    HRESULT hr = NULL;
    bool decibels = false;
    bool scalar = false;

    CoInitialize(NULL);

    IMMDeviceEnumerator* deviceEnumerator = NULL;
    hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER,
        __uuidof(IMMDeviceEnumerator), (LPVOID*)&deviceEnumerator);
    IMMDevice* defaultDevice = NULL;

    hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
    deviceEnumerator->Release();
    deviceEnumerator = NULL;

    hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume),
        CLSCTX_INPROC_SERVER, NULL, (LPVOID*)&m_endpointVolume);
    defaultDevice->Release();
    defaultDevice = NULL;

    m_initVolume = GetVolume();
}