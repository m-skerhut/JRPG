#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "JRPG_WorldGameMode.generated.h"

UCLASS()
class JRPG_API AJRPG_WorldGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AJRPG_WorldGameMode();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	void LoadBattle(TArray<TSubclassOf<class AJRPG_BaseEnemy>> Enemies);
	UFUNCTION()
	void LoadLevel(FName levelName);
};
