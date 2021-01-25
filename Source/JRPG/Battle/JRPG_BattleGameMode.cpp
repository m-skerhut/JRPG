#include "JRPG_BattleGameMode.h"
#include "../Game/JRPG_GameInstance.h"
#include "../Interfaces/JRPG_BattleInterface.h"
#include "JRPG_BaseHero.h"
#include "JRPG_BaseEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "../Data/HelperFunctions.h"
#include "Blueprint/UserWidget.h"

AJRPG_BattleGameMode::AJRPG_BattleGameMode()
{
	//DefaultPawnClass = SpectatorClass;
	PrimaryActorTick.bCanEverTick = true;
}

void AJRPG_BattleGameMode::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PController = GetWorld()->GetFirstPlayerController();
	if (PController)
	{
		PController->SetInputMode(FInputModeUIOnly());
		PController->bShowMouseCursor = true;
	}
	
	GameInstance = Cast<UJRPG_GameInstance>(GetGameInstance());

	SpawnHeroes();
	SpawnEnemies();

	MainWindowWidget = CreateWidget<UUserWidget>(GetWorld(), MainWindowClass);
	if (MainWindowWidget)
	{
		MainWindowWidget->AddToViewport();
	}

	ActivateBattleActors();
}

void AJRPG_BattleGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	switch (currentBattleState)
	{
	case EBattleState::WAIT:
		if (AttackerList.Num() > 0)
		{
			currentBattleState = EBattleState::TAKE_ACTION;
		}
		break;

	case EBattleState::TAKE_ACTION:

		if (AttackerList[0]->GetClass()->ImplementsInterface(UJRPG_BattleInterface::StaticClass()))
		{
			IJRPG_BattleInterface::Execute_PerformAction(AttackerList[0]);
			ActorToFocus = AttackerList[0];
		}
		currentBattleState = EBattleState::PERFORM_ACTION;

		break;
	case EBattleState::PERFORM_ACTION:
		//idle State to wait untill performer is done

		break;
	case EBattleState::CHECK_ALIVE:
		CheckAlive();
		//reset actor to focus
		ActorToFocus = nullptr;
		break;
	case EBattleState::WIN:
		bWin = true;
		Win();
		break;
	case EBattleState::LOSE:
		bWin = false;
		Lose();
		break;
	default:
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("ERROR OCCURED!! BattleGameMode failed to switch on battlestate!!")));
		break;
	}
}

void AJRPG_BattleGameMode::SpawnHeroes()
{
	TArray<AActor*> HeroSpawns;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "HeroSpawn", HeroSpawns);


	if (GameInstance->HeroParty.Num() > 0)
	{
		for (int i = 0; i < GameInstance->HeroParty.Num(); i++)
		{
			if (HeroSpawns.IsValidIndex(i))
			{
				FActorSpawnParameters SpawnInfo;
				SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				AJRPG_BaseHero* newHero = GetWorld()->SpawnActor<AJRPG_BaseHero>(GameInstance->HeroParty[i]->HeroClass, HeroSpawns[i]->GetActorTransform(), SpawnInfo);
				if (newHero)
				{
					newHero->HeroProperties = GameInstance->HeroParty[i];
					newHero->BGM = this;
					newHero->SpawnWeapon();

					HeroesInBattle.Add(newHero);
				}
			}
		}
	}
}

void AJRPG_BattleGameMode::SpawnEnemies()
{
	TArray<AActor*> EnemySpawns;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "EnemySpawn", EnemySpawns);

	for (int i = 0; i < GameInstance->Enemies.Num(); i++)
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AJRPG_BaseEnemy* newEnemy = GetWorld()->SpawnActor<AJRPG_BaseEnemy>(GameInstance->Enemies[i], EnemySpawns[i]->GetActorTransform(), SpawnInfo);
		newEnemy->BGM = this;
		EnemiesInBattle.Add(newEnemy);
	}
}

void AJRPG_BattleGameMode::ActivateBattleActors()
{
	TArray<AActor*> battleActors;
	battleActors.Append(HeroesInBattle);
	battleActors.Append(EnemiesInBattle);
	for (AActor* battleActor : battleActors)
	{
		if (battleActor->GetClass()->ImplementsInterface(UJRPG_BattleInterface::StaticClass()))
		{
			IJRPG_BattleInterface::Execute_SetShouldTick(battleActor, true);
		}
	}
}

void AJRPG_BattleGameMode::CheckAlive()
{
	SetHelpText("");
	if (HeroesInBattle.Num() < 1)
	{
		currentBattleState = EBattleState::LOSE;
	}
	else if (EnemiesInBattle.Num() < 1)
	{
		currentBattleState = EBattleState::WIN;
	}
	else
	{
		if (AttackerList.Num() > 0)
			AttackerList.RemoveAt(0);
		currentBattleState = EBattleState::WAIT;
	}
}

void AJRPG_BattleGameMode::SetHelpText(FString text)
{
	helpText = text;
}

void AJRPG_BattleGameMode::Win()
{
	for (int i = 0; i < HeroesInBattle.Num(); i++)
	{
		if (HeroesInBattle[0]->GetClass()->ImplementsInterface(UJRPG_BattleInterface::StaticClass()))
		{
			IJRPG_BattleInterface::Execute_SetShouldTick(HeroesInBattle[0], false);
		}
	}


	RollBattleRewards();
	//create win battle widget
	MainWindowWidget->RemoveFromViewport();
	
	WinWidget = CreateWidget<UUserWidget>(GetWorld(), WinBattleWindowClass);
	if (WinWidget)
	{
		WinWidget->AddToViewport();
	}
	/*for (int i = 0; i < HeroesInBattle.Num(); i++)
	{
		AJRPG_BaseHero* hero = Cast<AJRPG_BaseHero>(HeroesInBattle[i]);
		hero->HeroProperties->AddExperience(toatalExperience);
	}*/
	currentBattleState = EBattleState::PERFORM_ACTION;//idle state
}

void AJRPG_BattleGameMode::Lose()
{
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Yellow, FString::Printf(TEXT("You Lose")));
	for (int i = 0; i < EnemiesInBattle.Num(); i++)
	{
		if (EnemiesInBattle[0]->GetClass()->ImplementsInterface(UJRPG_BattleInterface::StaticClass()))
		{
			IJRPG_BattleInterface::Execute_SetShouldTick(EnemiesInBattle[0], false);
		}
	}

	MainWindowWidget->RemoveFromViewport();

	WinWidget = CreateWidget<UUserWidget>(GetWorld(), WinBattleWindowClass);
	if (WinWidget)
	{
		WinWidget->AddToViewport();
	}
}

void AJRPG_BattleGameMode::RollBattleRewards()
{
	for (int i = 0; i < DeadEnemies.Num(); i++)
	{
		AJRPG_BaseEnemy* enemy = DeadEnemies[i];
		toatalExperience += enemy->ExperienceReward;
		totalGold += enemy->GoldReward;

		//add item to loot
		for (FItemDynamicData itemData : enemy->PossibleItems)
		{
			bool bContainsItem = false;
			int index = -1;

			//check if item is already in loot
			for (int j = 0; j < loot.Num(); j++)
			{
				if (itemData.dtRowName == loot[j].dtRowName)
				{
					bContainsItem = true;
					index = j;
					break;
				}
			}

			if (bContainsItem)
			{
				loot[index].amount += itemData.amount;
			}
			else
			{
				loot.Add(itemData);
			}

			//add to inventory
			UHelperFunctions::AddItemToInventory(GameInstance, itemData);
		}
	}
}