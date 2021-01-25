#pragma once

#include "CoreMinimal.h"
#include "JRPG_BaseInteractable.h"
#include "JRPG_HeroNPC.generated.h"

UCLASS()
class JRPG_API AJRPG_HeroNPC : public AJRPG_BaseInteractable
{
	GENERATED_BODY()
	
public:

	AJRPG_HeroNPC();

	void Interact(AActor* Caller);//prototype declaration
	virtual void Interact_Implementation(AActor* Caller) override; //actual implementation of interact method

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
		TSubclassOf<class UJRPG_HeroProperties> HeroPropClass;

protected:

	virtual void BeginPlay() override;

private:
	bool bInteracted;
};
