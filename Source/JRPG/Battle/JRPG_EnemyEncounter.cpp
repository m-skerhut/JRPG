#include "JRPG_EnemyEncounter.h"
#include "Components/BoxComponent.h"
#include "../Game/JRPG_WorldGameMode.h"
#include "../Game/JRPG_PlayerCharacter.h"

#include "../Game/JRPG_GameInstance.h"

AJRPG_EnemyEncounter::AJRPG_EnemyEncounter()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxCollider->SetBoxExtent(FVector(100.f, 100.f, 100.f));
	BoxCollider->SetGenerateOverlapEvents(true);
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AJRPG_EnemyEncounter::OnOverlapBegin);
}


void AJRPG_EnemyEncounter::BeginPlay()
{
	Super::BeginPlay();

	UJRPG_GameInstance* GI = Cast<UJRPG_GameInstance>(GetGameInstance());
	if (GI)
	{
		FString objName = this->GetName();
		if (GI->interactedObjects.Find(objName))
		{
			Destroy();
		}
		//
	}
}


void AJRPG_EnemyEncounter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AJRPG_EnemyEncounter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UJRPG_GameInstance* GI = Cast<UJRPG_GameInstance>(GetGameInstance());
	if (!bTriggered)
	{
		if (OtherActor && (OtherActor != this))
		{
			//check if other actor is the player character
			AJRPG_PlayerCharacter* player = Cast<AJRPG_PlayerCharacter>(OtherActor);
			if (player)
			{
				AJRPG_WorldGameMode* GM = Cast< AJRPG_WorldGameMode>(GetWorld()->GetAuthGameMode());
				GM->LoadBattle(Enemies);

				GI->interactedObjects.Add(this->GetName(), true);
			}
		}
	}

}

