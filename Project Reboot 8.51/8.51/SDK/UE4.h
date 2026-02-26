#pragma once

// Main UE4 SDK header - includes the generated SDK
#include "SDK.hpp"

// Forward declarations and common types
namespace SDK {
    // UObjectArray for GObjects - defined in Basic.hpp
    class TUObjectArray;
}

// GObjects accessor - returns the global object array
inline SDK::TUObjectArray* GetGObjects() {
    return (SDK::TUObjectArray*)(uintptr_t(GetModuleHandleW(0)) + 0x7A5B0A0);
}

// Engine singleton
inline SDK::UEngine* GetEngine() {
    return SDK::UEngine::GetEngine();
}

// World accessor
inline SDK::UWorld* GetWorld() {
    return SDK::UWorld::GetWorld();
}

// UObject::GObjects compatibility - redirect to GetGObjects
namespace SDK {
    inline TUObjectArray* GObjectsPtr = nullptr;
}

#define UObject__GObjects (SDK::GObjectsPtr ? SDK::GObjectsPtr : (SDK::GObjectsPtr = GetGObjects()))
