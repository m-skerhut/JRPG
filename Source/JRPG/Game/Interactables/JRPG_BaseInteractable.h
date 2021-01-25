#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <JRPG/Interfaces/JRPG_InteractInterface.h>
#include "Components/TextRenderComponent.h"
#include "JRPG_BaseInteractable.generated.h"

UCLASS()
class JRPG_API AJRPG_BaseInteractable : public AActor, public IJRPG_InteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJRPG_BaseInteractable();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactable Properties")
		FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactable Properties")
		FString InteractabbleHelpText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactable Properties")
		class UStaticMeshComponent* InteractableMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactable Properties")
		class UTextRenderComponent* TextRenderer;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
	void Interact(AActor* Caller);//prototype declaration
	virtual void Interact_Implementation(AActor* Caller) override; //actual implementation of interact method, has to be overridden in sub class!!!!

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
	void StartFocus();
	virtual void StartFocus_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
	void EndFocus();
	virtual void EndFocus_Implementation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
