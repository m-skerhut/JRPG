#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "../Data/JRPG_HeroProperties.h"
#include "../Data/DataContainer.h"
#include "JRPG_GameInstance.generated.h"


UCLASS()
class JRPG_API UJRPG_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	virtual void Init() override;

	//the heros in team/battle
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Heroes")
	TArray<class UJRPG_HeroProperties*>HeroParty;
	//only used to instantiate HeroParty
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Heroes")
	TArray< TSubclassOf<class UJRPG_HeroProperties>> DefaultHeroPartyClasses;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<class AJRPG_BaseEnemy>> Enemies;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	int32 gold;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FItemDynamicData> inventory;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	class UDataTable* itemDataTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Game")
	FVector lastPos;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Game")
	TMap<FString, bool> interactedObjects;
protected:
	
	

private:

};
