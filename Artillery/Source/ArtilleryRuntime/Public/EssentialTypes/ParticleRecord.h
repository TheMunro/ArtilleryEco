#pragma once

#include "SkeletonKey.h"

//#include "ParticleRecord.generated.h"

struct ParticleRecord
{
	TWeakObjectPtr<UNiagaraDataChannelAsset> NDCAssetPtr;
	int32 NDCIndex;
};
