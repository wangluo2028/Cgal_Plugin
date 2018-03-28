//MIT License

//Copyright(c) 2018 annsar

//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:

//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.

//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.

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

