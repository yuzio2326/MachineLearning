// Fill out your copyright notice in the Description page of Project Settings.


#include "../Monster/LearningMonster.h"


// Sets default values
ALearningMonster::ALearningMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALearningMonster::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALearningMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALearningMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

