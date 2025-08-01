// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestMachineLearnCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ATestMachineLearnCharacter

ATestMachineLearnCharacter::ATestMachineLearnCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ATestMachineLearnCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

void ATestMachineLearnCharacter::SaveInputLogsToCSV()
{

	FString SavePath = FPaths::ProjectDir() + "/Saved/InputLogs.csv";
	FString CSVContent = "InputName,TimeStamp,LocationX,LocationY,LocationZ,FrameNumber\n";
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	// 기존 파일 존재 유무
	if (PlatformFile.FileExists(*SavePath))
	{

	}

	for (const FPlayerInputLog& Log : InputLogs)
	{
		CSVContent += FString::Printf(TEXT("%s,%.2f,%.2f,%.2f,%.2f,%d\n"),
			*Log.InputName,
			Log.TimeStamp,
			Log.Location.X,
			Log.Location.Y,
			Log.Location.Z,
			Log.FrameNumber);
	}

	if (FFileHelper::SaveStringToFile(CSVContent, *SavePath))
	{
		UE_LOG(LogTemplateCharacter, Log, TEXT("SavePath is %s , Player Input Log CSV SaveSuccess"), *SavePath);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Log, TEXT("SavePath is %s , Player Input Log CSV SaveFailed"), *SavePath);
	}

}

//////////////////////////////////////////////////////////////////////////
// Input

void ATestMachineLearnCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ATestMachineLearnCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATestMachineLearnCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATestMachineLearnCharacter::Look);
		EnhancedInputComponent->BindAction(SaveAction, ETriggerEvent::Started, this, &ATestMachineLearnCharacter::SaveInputLogsToCSV);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ATestMachineLearnCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);


		FPlayerInputLog Log;
		Log.InputName = TEXT("Move");
		Log.TimeStamp = GetWorld()->GetTimeSeconds();
		Log.Location = GetActorLocation();
		Log.FrameNumber = GFrameCounter;

		InputLogs.Add(Log);

		FVector PlayerLoc = GetActorLocation();

		UE_LOG(LogTemplateCharacter, Log, TEXT("Player Loc is : %f, %f, %f"), PlayerLoc.X, PlayerLoc.Y, PlayerLoc.Z);



	}
}

void ATestMachineLearnCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ATestMachineLearnCharacter::Jump()
{
	Super::Jump();
	UE_LOG(LogTemplateCharacter, Log, TEXT("Jump action triggered!"));

	FPlayerInputLog Log;
	Log.InputName = TEXT("Jump");
	Log.TimeStamp = GetWorld()->GetTimeSeconds();
	Log.Location = GetActorLocation();
	Log.FrameNumber = GFrameCounter;

	InputLogs.Add(Log);
}
