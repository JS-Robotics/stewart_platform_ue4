// Fill out your copyright notice in the Description page of Project Settings.


#include "stewart_platform/StewartPlatform.h"

// Sets default values
AStewartPlatform::AStewartPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStewartPlatform::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStewartPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

