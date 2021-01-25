#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Interfaces/JRPG_BattleInterface.h"
#include "JRPG_BattleGameMode.h"
#include "JRPG_BaseHero.generated.h"


UENUM(BlueprintType)
enum class EHeroState : uint8
{
	PROCESSING,
	CHOOSE_ACTION,
	INPUT,
	WAITING,
	PERFORM_ACTION,
	DEAD
};

UCLASS(Abstract)
class JRPG_API AJRPG_BaseHero : public ACharacter , public IJRPG_BattleInterface
{
	GENERATED_BODY()

public:

	AJRPG_BaseHero();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	//called by widget blueprint to set up attackinfo
	UFUNCTION(BlueprintCallable)
	void SetAttack(AActor*Target, EActionType actionType, class UJRPG_BaseAbility* ability, FItemDynamicData item);
	//called to spawn equipped weapon
	UFUNCTION(BlueprintCallable)
	void SpawnWeapon();

	class AJRPG_BattleGameMode* BGM;
	EHeroState heroState;

	UPROPERTY(BlueprintReadOnly)
	class UJRPG_HeroProperties* HeroProperties;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float maxCooldown;
	UPROPERTY(BlueprintReadOnly)
	float currentCooldown = 0;
	UPROPERTY(BlueprintReadOnly)
	bool bButtonEnabled = false;
	UPROPERTY(BlueprintReadWrite)
	class UUserWidget* TargetButton;


	//Interface functions
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BattleInterface")
	void PerformAction();//prototype declaration
	virtual void PerformAction_Implementation() override; //actual implementation of interact method
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BattleInterface")
	void MoveComplete();
	virtual void MoveComplete_Implementation();
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
	//Attack Animation
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UAnimMontage* AttackAnimMontage;
	//mele ability
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UAnimMontage* MeleAbilityAnimMontage;
	//cast Magic animation 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UAnimMontage* MagicAnimMontage;
	//use Item animation 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UAnimMontage* ItemAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UAnimMontage* HitAnimMontage;


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* weaponMesh;

	UPROPERTY(BlueprintReadOnly)
		bool bDead = false;

private:	

	UFUNCTION()
	void MoveToPosition(FVector position);
	UFUNCTION()
	float PlayAnimations();

	UFUNCTION()
	float CalculateDamage();

	void Die();

	UPROPERTY()
	class UAnimMontage* AnimMontageToPlay;

	FTurnInfo myAttack;
	bool bShouldTick = false;
	bool bStartAction;
	


	FVector attackPos;

	FVector startPos;
	FVector meleAttackPos;
	FVector rangeAttackPos;
	FVector turnPos;
	FRotator startRotation;
};
