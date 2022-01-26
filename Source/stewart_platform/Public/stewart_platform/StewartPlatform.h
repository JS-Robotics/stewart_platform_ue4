// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "StewartPlatform.generated.h"

UCLASS()
class STEWART_PLATFORM_API AStewartPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStewartPlatform();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UStaticMeshComponent* lower_yoke_driven;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UStaticMeshComponent* lower_spider;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UStaticMeshComponent* lower_yoke_driver;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UPhysicsConstraintComponent* lower_revolute_1_joint;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UPhysicsConstraintComponent* lower_revolute_2_joint;

	
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
