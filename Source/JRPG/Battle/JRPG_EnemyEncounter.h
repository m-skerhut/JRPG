#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JRPG_EnemyEncounter.generated.h"

UCLASS()
class JRPG_API AJRPG_EnemyEncounter : public AActor
{
	GENERATED_BODY()
	
public:	
	AJRPG_EnemyEncounter();

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	class UBoxComponent* BoxCollider;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<class AJRPG_BaseEnemy>> Enemies;

	bool bTriggered = false;
};
