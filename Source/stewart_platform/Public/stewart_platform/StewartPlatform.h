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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UStaticMeshComponent* lower_yoke_driven_1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UStaticMeshComponent* lower_spider_1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UStaticMeshComponent* lower_yoke_driver_1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UStaticMeshComponent* cylinder_1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UStaticMeshComponent* piston_1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UStaticMeshComponent* upper_yoke_driven_1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UStaticMeshComponent* upper_spider_1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UStaticMeshComponent* upper_yoke_driver_1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UStaticMeshComponent* upper_revolute_disk_1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UStaticMeshComponent* fixed_frame;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UStaticMeshComponent* dynamic_frame;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UPhysicsConstraintComponent* lower_revolute_1_joint_1;
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UPhysicsConstraintComponent* lower_revolute_2_joint_1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UPhysicsConstraintComponent* upper_revolute_1_joint_1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UPhysicsConstraintComponent* upper_revolute_2_joint_1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UPhysicsConstraintComponent* yoke_driver_fixed_cylinder_1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UPhysicsConstraintComponent* yoke_driven_fixed_piston_1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UPhysicsConstraintComponent* cylinder_prismatic_piston_1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UPhysicsConstraintComponent* leg1_fixed_frame;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UPhysicsConstraintComponent* leg1_dynamic_frame;


	// LEG 2
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UStaticMeshComponent* lower_yoke_driven_2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UStaticMeshComponent* lower_spider_2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UStaticMeshComponent* lower_yoke_driver_2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UStaticMeshComponent* cylinder_2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UStaticMeshComponent* piston_2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UStaticMeshComponent* upper_yoke_driven_2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UStaticMeshComponent* upper_spider_2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UStaticMeshComponent* upper_yoke_driver_2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UStaticMeshComponent* upper_revolute_disk_2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UPhysicsConstraintComponent* lower_revolute_1_joint_2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UPhysicsConstraintComponent* upper_revolute_1_joint_2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UPhysicsConstraintComponent* upper_revolute_2_joint_2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UPhysicsConstraintComponent* yoke_driver_fixed_cylinder_2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UPhysicsConstraintComponent* yoke_driven_fixed_piston_2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UPhysicsConstraintComponent* cylinder_prismatic_piston_2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UPhysicsConstraintComponent* leg2_fixed_frame;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UPhysicsConstraintComponent* leg2_dynamic_frame;

	//LEG 3
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UStaticMeshComponent* lower_yoke_driven_3;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UStaticMeshComponent* lower_spider_3;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UStaticMeshComponent* lower_yoke_driver_3;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UStaticMeshComponent* cylinder_3;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UStaticMeshComponent* piston_3;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UStaticMeshComponent* upper_yoke_driven_3;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UStaticMeshComponent* upper_spider_3;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UStaticMeshComponent* upper_yoke_driver_3;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UStaticMeshComponent* upper_revolute_disk_3;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UPhysicsConstraintComponent* lower_revolute_1_joint_3;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UPhysicsConstraintComponent* upper_revolute_1_joint_3;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UPhysicsConstraintComponent* upper_revolute_2_joint_3;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UPhysicsConstraintComponent* yoke_driver_fixed_cylinder_3;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UPhysicsConstraintComponent* yoke_driven_fixed_piston_3;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UPhysicsConstraintComponent* cylinder_prismatic_piston_3;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UPhysicsConstraintComponent* leg3_fixed_frame;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UPhysicsConstraintComponent* leg3_dynamic_frame;

	//Leg 4
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UStaticMeshComponent* lower_yoke_driven_4;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UStaticMeshComponent* lower_spider_4;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UStaticMeshComponent* lower_yoke_driver_4;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UStaticMeshComponent* cylinder_4;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UStaticMeshComponent* piston_4;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UStaticMeshComponent* upper_yoke_driven_4;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UStaticMeshComponent* upper_spider_4;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UStaticMeshComponent* upper_yoke_driver_4;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UStaticMeshComponent* upper_revolute_disk_4;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UPhysicsConstraintComponent* lower_revolute_1_joint_4;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UPhysicsConstraintComponent* upper_revolute_1_joint_4;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UPhysicsConstraintComponent* upper_revolute_2_joint_4;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UPhysicsConstraintComponent* yoke_driver_fixed_cylinder_4;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UPhysicsConstraintComponent* yoke_driven_fixed_piston_4;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UPhysicsConstraintComponent* cylinder_prismatic_piston_4;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UPhysicsConstraintComponent* leg4_fixed_frame;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StewartPlatform") UPhysicsConstraintComponent* leg4_dynamic_frame;
	

	
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
