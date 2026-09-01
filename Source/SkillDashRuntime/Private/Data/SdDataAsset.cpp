// Copyright (c)  Marko Petric & Yevhenii Selivanov


#include "Data/SdDataAsset.h"

// Bomber
#include "DalSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SdDataAsset)

// Returns this Data Asset, is checked and wil crash if can't be obtained, e.g: when is not set
const USdDataAsset& USdDataAsset::Get(const UObject* OptionalWorldContext)
{
	return UDalSubsystem::GetDataAssetChecked<ThisClass>();
}