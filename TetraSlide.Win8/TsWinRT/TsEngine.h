#pragma once

#include "TsPlayer.h"
#include "TsGameDescriptor.h"
#include "TsView.h"

#include <Engine/Interfaces.h>

#include <collection.h>

namespace TsWinRT
{

public ref class TsEngine sealed
{
public:
    TsEngine();
    Platform::String^ Version();
    void Init( ITsView^ view );
    TsGameDescriptor^ CreateGame( ITsPlayer^ home, ITsPlayer^ away );
    Platform::String^ SaveGame( TsGameDescriptor^ game );
    TsGameDescriptor^ LoadGame( Platform::String^ gameData );
    Windows::Foundation::Collections::IVector< TsGameDescriptor^ >^ GetRemoteGameDescriptors( Platform::String^ localId );
    void ResumeGame( TsGameDescriptor^ game );
private:
    Implbits::IEngine::Ptr m_engine;
};

} // TsWinRT
