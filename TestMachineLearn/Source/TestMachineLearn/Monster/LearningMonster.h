// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LearningMonster.generated.h"

UCLASS()
class TESTMACHINELEARN_API ALearningMonster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALearningMonster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	
};
