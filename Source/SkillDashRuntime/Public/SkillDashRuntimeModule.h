// Copyright (c) Marko Petric & Yevhenii Selivanov

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FSkillDashRuntimeModule : public IModuleInterface
{
public:
	//~IModuleInterface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	//~End of IModuleInterface
};
