#pragma once
#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Interface.h"
#include "GameFramework/Actor.h"
#include "UObject/ScriptMacros.h"
#include "SkeletonTypes.h"
#include "KeyedConcept.generated.h"

UINTERFACE(NotBlueprintable)
class SKELETONKEY_API UCanReady : public UInterface
{
	GENERATED_UINTERFACE_BODY()

	
};


inline UCanReady::UCanReady(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}



class SKELETONKEY_API ICanReady 
{
	GENERATED_IINTERFACE_BODY()
	
	//this generalizes the latch-forward design we use in the keycarry and elsewhere. It's turned out to not suck.
	bool IsReady = false;

	//Generally, you don't need to override this, but it's virtual just in case. the default behavior is what we've found effective.
	virtual void AttemptRegister()
	{
		if (!IsReady)
		{
			IsReady = RegistrationImplementation();
		}
		
	};
	// override this:
	virtual bool RegistrationImplementation() { return true; };
};

UINTERFACE(NotBlueprintable)
class SKELETONKEY_API UKeyedConstruct : public UCanReady
{
	GENERATED_UINTERFACE_BODY()

	
};


inline UKeyedConstruct::UKeyedConstruct(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}



class SKELETONKEY_API IKeyedConstruct : public ICanReady
{
	GENERATED_IINTERFACE_BODY()
	
	virtual FSkeletonKey GetMyKey() const
	{
		UE_LOG(
	LogTemp,
	Error,
	TEXT("Unexpected call to unimplemented GetMyKey method. While not necessarily fatal, this is always worth checking."));
		return FSkeletonKey();
	}
};

//tag interface marking out that this is a system, pillar, entity, or external that provides facts about keys or manages keys
UINTERFACE(NotBlueprintable)
class USkeletonLord : public UInterface
{
	GENERATED_UINTERFACE_BODY()

};


inline USkeletonLord::USkeletonLord(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}


//A Key Binder, Keyless Itself (These are used ONLY for subsystems or other ECS pillars that are system preconditions)
class ISkeletonLord
{
	GENERATED_IINTERFACE_BODY()
	
};