#include "JRPG_HeroNPC.h"
#include <JRPG/Game/JRPG_GameInstance.h>

AJRPG_HeroNPC::AJRPG_HeroNPC()
{
	InteractabbleHelpText = FString("F talk");
	//destroy static mesh because we want a skeletal mesh
	InteractableMesh->DestroyComponent();
	//create skeletalmesh
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	RootComponent = SkeletalMesh;

}

void AJRPG_HeroNPC::BeginPlay()
{
	Super::BeginPlay();

	UJRPG_GameInstance* GI = Cast<UJRPG_GameInstance>(GetGameInstance());
	if (GI)
	{
		FString objName = this->GetName();
		if (GI->interactedObjects.Find(objName))
		{
			bInteracted = true;
		}
	}
	if (bInteracted)
	{
		Destroy();
	}
}

void AJRPG_HeroNPC::Interact_Implementation(AActor* Caller)
{
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("interact")));

	//ad hero to team
	UJRPG_GameInstance* GI = Cast<UJRPG_GameInstance>(GetGameInstance());
	if (GI)
	{
		if (HeroPropClass)
		{
			UJRPG_HeroProperties* hero = NewObject<UJRPG_HeroProperties>(GI, HeroPropClass->GetFName(), RF_NoFlags, HeroPropClass.GetDefaultObject());
			if (hero)
			{
				hero->Initialize();
				GI->HeroParty.Add(hero);

				//if hero was added to team we want to destroy npc
				GI->interactedObjects.Add(this->GetName(), true);
				Destroy();
			}
		}
	}
}