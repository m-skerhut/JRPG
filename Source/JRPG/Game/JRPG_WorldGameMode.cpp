#include "JRPG_WorldGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "JRPG_PlayerCharacter.h"
#include "JRPG_GameInstance.h"

AJRPG_WorldGameMode::AJRPG_WorldGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_Player"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	PrimaryActorTick.bCanEverTick = true;
}

void AJRPG_WorldGameMode::BeginPlay()
{
	APlayerController* PController = GetWorld()->GetFirstPlayerController();
	if (PController)
	{
		PController->SetInputMode(FInputModeGameOnly());
		PController->bShowMouseCursor = false;
		UJRPG_GameInstance* GI = Cast<UJRPG_GameInstance>(GetGameInstance());
		if (GI && !GI->lastPos.IsZero())
		{
			GetWorld()->GetFirstPlayerController()->GetCharacter()->SetActorLocation(GI->lastPos, false, nullptr, ETeleportType::None);
		}
	}
}

void AJRPG_WorldGameMode::Tick(float DeltaSeconds)
{
}

void AJRPG_WorldGameMode::LoadBattle(TArray<TSubclassOf<class AJRPG_BaseEnemy>> Enemies)
{
	//set enemies to battle
	UJRPG_GameInstance* GI = Cast<UJRPG_GameInstance>(GetGameInstance());
	if (GI)
		GI->Enemies = Enemies;
	//get player
	AJRPG_PlayerCharacter* PlayerChar = Cast<AJRPG_PlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (PlayerChar)
	{
		PlayerChar->ShowBattleTransition();
		GI->lastPos = PlayerChar->GetActorLocation();
	}
	
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDel;
	//Binding the function with specific values
	TimerDel.BindUFunction(this, "LoadLevel");
	//Calling Move Function after animation without looping
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, 1.0f, false);

}

void AJRPG_WorldGameMode::LoadLevel(FName levelName)
{
	UGameplayStatics::OpenLevel(GetWorld(), "BattleMap", true);
}
