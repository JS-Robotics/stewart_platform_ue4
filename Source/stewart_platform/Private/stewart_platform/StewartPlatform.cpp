// Fill out your copyright notice in the Description page of Project Settings.


#include "stewart_platform/StewartPlatform.h"

#include "EnvironmentQuery/EnvQueryTypes.h"

// Sets default values
AStewartPlatform::AStewartPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Get Meshes
	// UString lower_yoke_driven_path = "Meshes/LinearActuator/yoke_lower_driven.yoke_lower_cylinder";
	static ConstructorHelpers::FObjectFinder<UStaticMesh> lower_yoke_driven_asset(TEXT("/Game/Meshes/LinearActuator/yoke_lower_driven.yoke_lower_driven"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> lower_spider_asset(TEXT("/Game/Meshes/LinearActuator/spider_lower.spider_lower"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> lower_yoke_driver_asset(TEXT("/Game/Meshes/LinearActuator/yoke_lower_driver.yoke_lower_driver"));

	// Create a dummy for the root component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene component"));

	// Setting up the joints form here out
	lower_yoke_driven = CreateDefaultSubobject<UStaticMeshComponent>("LowerYokeDriven");
	lower_yoke_driven->SetStaticMesh(lower_yoke_driven_asset.Object);
	lower_yoke_driven->SetupAttachment(RootComponent);
	lower_yoke_driven->SetSimulatePhysics(false);

	lower_spider = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LowerSpider"));
	lower_spider->SetStaticMesh(lower_spider_asset.Object);
	lower_spider->SetupAttachment(lower_yoke_driven);
	lower_spider->SetSimulatePhysics(true);
	lower_spider->SetMobility(EComponentMobility::Movable);

	lower_yoke_driver = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LowerYokeDriver"));
	lower_yoke_driver->SetStaticMesh(lower_yoke_driver_asset.Object);
	lower_yoke_driver->SetupAttachment(lower_spider);
	lower_yoke_driver->SetSimulatePhysics(true);
	lower_yoke_driver->SetMobility(EComponentMobility::Movable);
	// lower_yoke_driver->SetRelativeLocation({0, 0, 200});

	// Constraint settings
	EAngularConstraintMotion _rotation_locked = ACM_Locked;
	
	lower_revolute_1_joint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("RevoluteJoint1"));
	lower_revolute_1_joint->SetupAttachment(RootComponent);
	lower_revolute_1_joint->ComponentName1.ComponentName = "LowerYokeDriven";
	lower_revolute_1_joint->ComponentName2.ComponentName = "LowerSpider";
	lower_revolute_1_joint->SetDisableCollision(true);
	// lower_revolute_1_joint->SetRelativeLocation({0, 0, 100});
	lower_revolute_1_joint->SetAngularSwing1Limit(_rotation_locked, 0);
	lower_revolute_1_joint->SetAngularSwing2Limit(_rotation_locked, 0);
	// lower_revolute_1_joint->SetAngularTwistLimit(_rotation_locked, 0);
	
	// Must be created in constructor
	lower_revolute_1_joint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("RevoluteJoint"));
	lower_revolute_1_joint->SetupAttachment(RootComponent);
	lower_revolute_1_joint->ComponentName1.ComponentName = "LowerSpider";
	lower_revolute_1_joint->ComponentName2.ComponentName = "LowerYokeDriver";
	lower_revolute_1_joint->SetDisableCollision(true);
	// lower_revolute_1_joint->SetRelativeLocation({0, 0, 100});
	
	lower_revolute_1_joint->SetAngularSwing1Limit(_rotation_locked, 0);
	// lower_revolute_1_joint->SetAngularSwing2Limit(test, 0);
	lower_revolute_1_joint->SetAngularTwistLimit(_rotation_locked, 0);
	
	
	

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

