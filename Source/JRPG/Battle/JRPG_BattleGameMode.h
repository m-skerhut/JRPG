#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "../Data/DataContainer.h"
#include "JRPG_BattleGameMode.generated.h"


UENUM(BlueprintType)
enum class EActionRange : uint8
{
	MELEE,
	DISTANCE
};

UENUM(BlueprintType)
enum class EActionType : uint8
{
	ATTACK,
	ABILITY,
	ITEM
};

USTRUCT(BlueprintType)
struct FTurnInfo
{
	GENERATED_BODY()

	class AActor* attacker = nullptr;
	class AActor* target = nullptr;
	EActionType turnType;
	class UJRPG_BaseAbility* Ability = nullptr;
	FItemDynamicData item;
};

UENUM(BlueprintType)
enum class EBattleState : uint8
{
	WAIT,
	TAKE_ACTION,
	PERFORM_ACTION,
	CHECK_ALIVE,
	WIN,
	LOSE
};

UCLASS()
class JRPG_API AJRPG_BattleGameMode : public AGameModeBase
{
	GENERATED_BODY()
	

public:

	AJRPG_BattleGameMode();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	void SetHelpText(FString text);

	EBattleState currentBattleState;
	//turn list
	TArray<AActor*> AttackerList;
	//Actors in Battle
	UPROPERTY(BlueprintReadOnly)
	TArray<AActor*> HeroesInBattle;
	UPROPERTY(BlueprintReadOnly)
	TArray<AActor*> EnemiesInBattle;
	UPROPERTY(BlueprintReadOnly)
	TArray<class AJRPG_BaseEnemy*> DeadEnemies;
	//the actor the camera should focus on
	UPROPERTY(BlueprintReadOnly)
	AActor* ActorToFocus;

	//widgets
	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UUserWidget> MainWindowClass;
	UPROPERTY()
	UUserWidget* MainWindowWidget;
	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UUserWidget> HeroBarWidgetClass;
	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UUserWidget> WinBattleWindowClass;
	UPROPERTY()
	UUserWidget* WinWidget;
	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	bool bWin;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widgets")
	FString helpText = "";

	//Battlerewards
	UPROPERTY(BlueprintReadOnly)
	float toatalExperience;
	UPROPERTY(BlueprintReadOnly)
	int32 totalGold;
	UPROPERTY(BlueprintReadOnly)
	TArray<FItemDynamicData> loot;
protected:


private:

	UPROPERTY()
	class UJRPG_GameInstance* GameInstance;

	void SpawnHeroes();
	void SpawnEnemies();
	void ActivateBattleActors();
	void CheckAlive();
	void Win();
	void Lose();
	void RollBattleRewards();
};
