#include "JRPG_Chest.h"
#include <JRPG/Data/HelperFunctions.h>
#include <JRPG/Game/ObtainedItemWidget.h>
#include "Components/TextRenderComponent.h"

AJRPG_Chest::AJRPG_Chest()
{
	InteractabbleHelpText = FString("F open");
	bOpen = false;
}

void AJRPG_Chest::BeginPlay()
{
	Super::BeginPlay();

	UJRPG_GameInstance* GI = Cast<UJRPG_GameInstance>(GetGameInstance());
	if (GI)
	{
		FString objName = this->GetName();
		if (GI->interactedObjects.Find(objName))
		{
			bOpen = true;
		}
	}
	if (bOpen)
	{
		if (OpenChestMesh)
			InteractableMesh->SetStaticMesh(OpenChestMesh);

		TextRenderer->SetHiddenInGame(true);
	}
}

void AJRPG_Chest::Interact_Implementation(AActor* Caller)
{
	if (bOpen == false)
	{
		UJRPG_GameInstance* GI = Cast<UJRPG_GameInstance>(GetGameInstance());

		for (FItemDynamicData item : items)
		{
			UHelperFunctions::AddItemToInventory(GI, item);
		}

		GI->gold += gold;
		if (OpenChestMesh)
			InteractableMesh->SetStaticMesh(OpenChestMesh);
		
		TextRenderer->SetHiddenInGame(true);

		if (obtainedWidgetClass)
		{
			itemsWidget = CreateWidget<UObtainedItemWidget>(GetWorld(), obtainedWidgetClass);
			if (itemsWidget)
			{
				itemsWidget->obtainedItems = items;
				itemsWidget->AddToViewport();
			}
		}
		
		GI->interactedObjects.Add(this->GetName(), true);
		bOpen = true;
	}
}
