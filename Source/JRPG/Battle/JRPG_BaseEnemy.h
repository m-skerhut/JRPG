#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Interfaces/JRPG_BattleInterface.h"
#include "JRPG_BattleGameMode.h"

#include "JRPG_BaseEnemy.generated.h"



UENUM(BlueprintType)
enum class EEnemySate : uint8
{
	PROCESSING,
	CHOOSE_ACTION,
	WAITING,
	PERFORM_ACTION,
	DEAD
};


UCLASS()
class JRPG_API AJRPG_BaseEnemy : public ACharacter, public IJRPG_BattleInterface
{
	GENERATED_BODY()

public:

	AJRPG_BaseEnemy();

	class AJRPG_BattleGameMode* BGM;

	EEnemySate enemyState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyProperties")
	FString enemyName;
	
	FTurnInfo myAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float attackOffset;

	//Attack Animation
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
	class UAnimMontage* AttackAnimMontage;
	//Magic / range Attack animation 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
	class UAnimMontage* MagicAnimMontage;
	//hit reaction animation
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
	class UAnimMontage* HitAnimMontage;
	//xp reward for killing this enemy
	UPROPERTY(EditAnywhere, Category = "Loot")
	float ExperienceReward;
	//gold reward for killing this enemy
	UPROPERTY(EditAnywhere, Category = "Loot")
	uint16 GoldReward;
	//possible items the enemy can drop
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	TArray<FItemDynamicData>PossibleItems;

	UPROPERTY(BlueprintReadWrite)
	class UUserWidget* targetButton;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	//Interface functions
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BattleInterface")
	void PerformAction();//prototype declaration
	virtual void PerformAction_Implementation() override; //actual implementation of perform action method
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BattleInterface")
	void MoveComplete();
	virtual void MoveComplete_Implementation();//actual implementation
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BattleInterface")
	void SetShouldTick(bool bTick);
	virtual void SetShouldTick_Implementation(bool bTick);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BattleInterface")
	FVector GetAttackPos();
	virtual FVector GetAttackPos_Implementation();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BattleInterface")
		void HealthChanged(float healthAmount, EMagicType magicType, EElementalType attackerType);
	virtual void HealthChanged_Implementation(float healthAmount, EMagicType magicType, EElementalType attackerType);



protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyProperties")
	float maxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyProperties")
	float currentHealth;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyProperties")
	float physicalAttack;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyProperties")
	float physicalDefense;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyProperties")
	float magicAttack;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyProperties")
	float magicDefense;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyProperties")
	EElementalType elementalType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyProperties")
	float maxCooldown;
	float currentCooldown;

	UFUNCTION(BlueprintNativeEvent, Category = "StringActor")
	void PlayDissolveEffect();

	virtual void PlayDissolveEffect_Implementation();

private:
	UFUNCTION()
	void ChooseAttack();
	UFUNCTION()
	void MoveToPosition(FVector position);
	UFUNCTION()
	void Die();
	UFUNCTION()
	float CalculateDamage();

	//class ABattleAIController* battleAICont;

	bool bShouldTick = false;
	bool bStartAction;

	FVector startPos;
	FVector turnPos;
	FRotator startRotation;
	FVector attackPos;
	//FVector attackOffset;
};
