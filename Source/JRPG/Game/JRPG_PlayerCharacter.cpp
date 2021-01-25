#include "JRPG_PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Blueprint/WidgetLayoutLibrary.h"

#if WITH_EDITOR
#include "UnrealEd.h"
#include "DrawDebugHelpers.h"
#endif


AJRPG_PlayerCharacter::AJRPG_PlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	//Create SceneCaptureComponent
	SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("ScenCaptureCamera"));
	SceneCapture->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); //Attach the CaptureComponent to spring arm 
	//the scene should only be captured when the battle is about to be loaded
	SceneCapture->bCaptureEveryFrame = false;
	SceneCapture->bCaptureOnMovement = false;

}


void AJRPG_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}


void AJRPG_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TraceForInteractable();
}


void AJRPG_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AJRPG_PlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AJRPG_PlayerCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AJRPG_PlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AJRPG_PlayerCharacter::LookUpAtRate);

	// open game menu
	PlayerInputComponent->BindAction("ToggleGameMenu", IE_Pressed, this, &AJRPG_PlayerCharacter::ToggleGameMenu).bExecuteWhenPaused = true;

	//Interact with focused actor
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AJRPG_PlayerCharacter::Interact);
}

void AJRPG_PlayerCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AJRPG_PlayerCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AJRPG_PlayerCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AJRPG_PlayerCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AJRPG_PlayerCharacter::ShowBattleTransition()
{
	UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());

	//capture scene
	if (SceneCapture)
	{
		SceneCapture->CaptureScene();
	}
	//display battle transition effect
	if (BattleTransitionClass)
	{
		BattleTransitionWidget = CreateWidget<UUserWidget>(GetWorld(), BattleTransitionClass);
		if(BattleTransitionWidget)
			BattleTransitionWidget->AddToViewport();
	}
}

void AJRPG_PlayerCharacter::ToggleGameMenu()
{
	APlayerController* PController = GetWorld()->GetFirstPlayerController();
	if (GameMenuWidget)
	{
		//remove from viewport
		GameMenuWidget->RemoveFromParent();
		PController->SetInputMode(FInputModeGameOnly());
		PController->SetPause(false);
		PController->bShowMouseCursor = false;
		GameMenuWidget = nullptr;
	}
	else
	{
		UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());
		//add toviewport
		GameMenuWidget = CreateWidget<UUserWidget>(GetWorld(), GameMenuClass);
		GameMenuWidget->AddToViewport();
		PController->SetInputMode(FInputModeGameAndUI());
		PController->SetPause(true);
		PController->bShowMouseCursor = true;
	}
}

void AJRPG_PlayerCharacter::Interact()
{
	if (FocusedActor)
	{
		IJRPG_InteractInterface::Execute_Interact(FocusedActor, this);
	}
}

void AJRPG_PlayerCharacter::TraceForInteractable()
{
	FVector Pos;
	FRotator Rot;
	GetController()->GetPlayerViewPoint(Pos, Rot);

	FVector Start = Pos;
	float TraceDistance = 1000;
	FVector End = Start + (Rot.Vector() * TraceDistance);

	FHitResult Hit;
	FCollisionQueryParams CQP;
	CQP.AddIgnoredActor(this);
	//DrawDebugLine(GetWorld(), Start, End, FColor::Green,0.0f,1.0f);
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, CQP);
	if (bHit)
	{
		if (Hit.GetActor()->GetClass()->ImplementsInterface(UJRPG_InteractInterface::StaticClass()))
		{
			if (FocusedActor != Hit.GetActor())
			{
				if (FocusedActor)
				{
					IJRPG_InteractInterface::Execute_EndFocus(FocusedActor);
				}
				FocusedActor = Hit.GetActor();
				IJRPG_InteractInterface::Execute_StartFocus(FocusedActor);
			}
		}
		else
		{
			if (FocusedActor)
			{
				IJRPG_InteractInterface::Execute_EndFocus(FocusedActor);
			}
			FocusedActor = nullptr;
		}
	}
	else
	{
		if (FocusedActor)
		{
			IJRPG_InteractInterface::Execute_EndFocus(FocusedActor);
			FocusedActor = nullptr;
		}
	}
}
