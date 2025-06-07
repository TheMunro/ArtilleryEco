// Copyright Hedra Group.


#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "Memory/IntraTickThreadblindAlloc.h"
#include "LocomoUtil.h"

class FLocomoModuleAPI : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};