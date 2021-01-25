#include "JRPG_BaseInteractable.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AJRPG_BaseInteractable::AJRPG_BaseInteractable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	InteractableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	//RootComponent = InteractableMesh;
	TextRenderer = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Textrenderer"));
	//TextRenderer->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void AJRPG_BaseInteractable::BeginPlay()
{
	Super::BeginPlay();

	FString Text = InteractabbleHelpText + " " + Name;
	TextRenderer->SetText(FText::FromString(Text));
	TextRenderer->SetVisibility(false);
}

void AJRPG_BaseInteractable::Interact_Implementation(AActor* Caller)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("BaseInteractable!! YOU SHOULD NOT BE SEEING THIS!!")));
}

void AJRPG_BaseInteractable::StartFocus_Implementation()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("Focused: %s"), *this->GetFName().ToString()));

	if (TextRenderer)
	{
		if (!TextRenderer->IsVisible())
			TextRenderer->SetVisibility(true);

		//set rotation to face player
		FRotator Rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation());
		TextRenderer->SetWorldRotation(Rot);
	}
}

void AJRPG_BaseInteractable::EndFocus_Implementation()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("Unfocused: %s"), *this->GetFName().ToString()));

	if (TextRenderer)
		TextRenderer->SetVisibility(false);
}