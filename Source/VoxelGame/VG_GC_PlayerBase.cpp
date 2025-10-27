// AVG_GC_PlayerBase.cpp

#include "VG_GC_PlayerBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

AVG_GC_PlayerBase::AVG_GC_PlayerBase()
{
	// Use built-in capsule as root
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	RootComponent = GetCapsuleComponent();

	// --- Skeletal Mesh Component ---
	USkeletalMeshComponent* MeshComponent = GetMesh();
	ConstructorHelpers::FObjectFinder<USkeletalMesh> PlayerMesh(TEXT("/Game/Characters/Mannequins/Meshes/SKM_Quinn"));
	if (PlayerMesh.Succeeded())
	{
		MeshComponent->SetSkeletalMesh(PlayerMesh.Object);
		MeshComponent->SetRelativeLocation(FVector(0.f, 0.f, -90.f)); // adjust position if needed
		MeshComponent->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	}

	// --- Input Actions setup ---
	// Move
	static ConstructorHelpers::FObjectFinder<UInputAction> MoveActionFinder(TEXT("/Game/ThirdPerson/Input/Actions/IA_Move"));
	if (MoveActionFinder.Succeeded())
	{
		MoveAction = MoveActionFinder.Object;
	}
	// Look
	static ConstructorHelpers::FObjectFinder<UInputAction> LookActionFinder(TEXT("/Game/ThirdPerson/Input/Actions/IA_Look"));
	if (LookActionFinder.Succeeded())
	{
		LookAction = LookActionFinder.Object;
	}
	// Jump
	static ConstructorHelpers::FObjectFinder<UInputAction> JumpActionFinder(TEXT("/Game/ThirdPerson/Input/Actions/IA_Jump"));
	if (JumpActionFinder.Succeeded())
	{
		JumpAction = JumpActionFinder.Object;
	}
	// Zoom
	static ConstructorHelpers::FObjectFinder<UInputAction> ZoomActionFinder(TEXT("/Game/ThirdPerson/Input/Actions/IA_Zoom"));
	if (ZoomActionFinder.Succeeded())
	{
		ZoomAction = ZoomActionFinder.Object;
	}

	// --- Camera setup ---
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SetRelativeLocation(FVector(0.f, 0.f, 150.f)); ;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// --- Movement configuration ---
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	if (MoveComp)
	{
		MoveComp->bOrientRotationToMovement = true;
		MoveComp->RotationRate = FRotator(0.f, 500.f, 0.f);

		MoveComp->JumpZVelocity = 650.f;      // jump strength
		MoveComp->GravityScale = 0.85f;       // floatiness
		MoveComp->AirControl = 0.4f;          // movement control mid-air
		// MoveComp->MaxJumpCount = 1;           // single jump
		MoveComp->BrakingDecelerationFalling = 200.f;
	}
}

void AVG_GC_PlayerBase::BeginPlay()
{
	Super::BeginPlay();

	// Register input mapping context
	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
}

void AVG_GC_PlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveAction)
			EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AVG_GC_PlayerBase::Move);

		if (LookAction)
			EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &AVG_GC_PlayerBase::Look);

		if (ZoomAction)
			EnhancedInput->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &AVG_GC_PlayerBase::Zoom);

		if (JumpAction)
		{
			EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
			EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		}
	}
}

void AVG_GC_PlayerBase::Move(const FInputActionValue& Value)
{
	FVector2D MoveValue = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Forward, MoveValue.Y);
		AddMovementInput(Right, MoveValue.X);
	}
}

void AVG_GC_PlayerBase::Look(const FInputActionValue& Value)
{
	FVector2D LookValue = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		AddControllerYawInput(LookValue.X);
		AddControllerPitchInput(LookValue.Y);
	}
}

void AVG_GC_PlayerBase::Zoom(const FInputActionValue& Value)
{
	if (CameraBoom != nullptr)
	{
		float ZoomValue = Value.Get<float>();
		CameraBoom->TargetArmLength += -20.0f * ZoomValue;
	}
}
