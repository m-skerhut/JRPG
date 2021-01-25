#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "../Game/JRPG_GameInstance.h"
#include "HelperFunctions.generated.h"


UCLASS()
class JRPG_API UHelperFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	static void AddItemToInventory(UJRPG_GameInstance* gameInstance, FItemDynamicData item);

	UFUNCTION(BlueprintCallable)
	static void UseItem(UJRPG_GameInstance* gameInstance, FItemDynamicData item, UJRPG_HeroProperties* hero);

	UFUNCTION(BlueprintCallable)
	static void RemoveItem(UJRPG_GameInstance* gameInstance, FItemDynamicData item);

	UFUNCTION(BlueprintCallable)
	static TArray<FItemDynamicData> GetAllItemsOfType(UJRPG_GameInstance* gameInstance, EItemType itemType);

	UFUNCTION(BlueprintCallable)
		static UJRPG_HeroProperties* CalculateNewHeroStats(UJRPG_HeroProperties* hero, FBaseItemProperties item);
};
