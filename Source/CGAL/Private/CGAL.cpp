#include "CGALPrivatePCH.h"
#include "Core.h"
#include "ModuleManager.h"
#include "CGAL.h"
#include "IPluginManager.h"

#define LOCTEXT_NAMESPACE "FCGALModule"

IMPLEMENT_GAME_MODULE(FCGALModule, CGAL);

FCGALModule* FCGALModule::m_Singleton(nullptr);

FCGALModule& FCGALModule::Get()
{
	if (nullptr == m_Singleton) {
		check(IsInGameThread());
		FModuleManager::LoadModuleChecked<FCGALModule>("CGAL");
	}
	check(nullptr != m_Singleton);
	return *m_Singleton;
}

void FCGALModule::StartupModule()
{
	m_Singleton = this;

	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FString CgalBaseDirRelative = IPluginManager::Get().FindPlugin("CGAL")->GetBaseDir();
	FString BaseDir = FPaths::ConvertRelativePathToFull(CgalBaseDirRelative);

	// Add on the relative location of the third party dll and load it
	FString LIBGMP,LIBMPFR;
#if PLATFORM_WINDOWS
	LIBGMP = FPaths::Combine(*BaseDir, TEXT("ThirdParty/CGAL/libraries/Win64/libgmp-10.dll"));
	LIBMPFR = FPaths::Combine(*BaseDir, TEXT("ThirdParty/CGAL/libraries/Win64/libmpfr-4.dll"));
#endif // PLATFORM_WINDOWS

	LibgmpDllHandle = !LIBGMP.IsEmpty() ? FPlatformProcess::GetDllHandle(*LIBGMP) : nullptr;
	libmpfrDllHandle = !LIBMPFR.IsEmpty() ? FPlatformProcess::GetDllHandle(*LIBMPFR) : nullptr;
}

void FCGALModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	FPlatformProcess::FreeDllHandle(LibgmpDllHandle);
	LibgmpDllHandle = nullptr;

	FPlatformProcess::FreeDllHandle(libmpfrDllHandle);
	libmpfrDllHandle = nullptr;
}

#undef LOCTEXT_NAMESPACE

