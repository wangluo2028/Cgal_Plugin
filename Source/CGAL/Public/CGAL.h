
#pragma once

#include "ModuleManager.h"



class CGAL_API FCGALModule : public IModuleInterface
{
public:
	static FCGALModule& Get();

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	/** Handle to the test dll we will load */
	void*	LibgmpDllHandle;
	void*	libmpfrDllHandle;
private:
	static FCGALModule * m_Singleton;
};
