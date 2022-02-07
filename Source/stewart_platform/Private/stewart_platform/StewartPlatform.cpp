// Fill out your copyright notice in the Description page of Project Settings.


#include "stewart_platform/StewartPlatform.h"


#include "EnvironmentQuery/EnvQueryTypes.h"

// Sets default values
AStewartPlatform::AStewartPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Get Meshes
	static ConstructorHelpers::FObjectFinder<UStaticMesh> lower_yoke_driven_asset(TEXT("/Game/Meshes/LinearActuator/lower_yoke_driven.lower_yoke_driven"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> lower_spider_asset(TEXT("/Game/Meshes/LinearActuator/lower_spider.lower_spider"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> lower_yoke_driver_asset(TEXT("/Game/Meshes/LinearActuator/lower_yoke_driver.lower_yoke_driver"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> cylinder_asset(TEXT("/Game/Meshes/LinearActuator/cylinder.cylinder"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> piston_asset(TEXT("/Game/Meshes/LinearActuator/piston.piston"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> fixed_frame_asset(TEXT("/Game/Meshes/LinearActuator/fixed_frame.fixed_frame"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> dynamic_frame_asset(TEXT("/Game/Meshes/LinearActuator/dynamic_frame.dynamic_frame"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> upper_yoke_driven_asset(TEXT("/Game/Meshes/LinearActuator/upper_yoke_driven.upper_yoke_driven"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> upper_spider_asset(TEXT("/Game/Meshes/LinearActuator/upper_spider.upper_spider"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> upper_yoke_driver_asset(TEXT("/Game/Meshes/LinearActuator/upper_yoke_driver.upper_yoke_driver"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> upper_revolute_disk_asset(TEXT("/Game/Meshes/LinearActuator/upper_revolute_disk.upper_revolute_disk"));

	// Create a dummy for the root component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene component"));

	// Setting up the joints form here out
	fixed_frame = CreateDefaultSubobject<UStaticMeshComponent>("FixedFrame");
	fixed_frame->SetStaticMesh(fixed_frame_asset.Object);
	fixed_frame->SetupAttachment(RootComponent);
	fixed_frame->SetSimulatePhysics(false);
	fixed_frame->SetRelativeLocation({0,0,2.0f});
	
	dynamic_frame = CreateDefaultSubobject<UStaticMeshComponent>("DynamicFrame");
	dynamic_frame->SetStaticMesh(dynamic_frame_asset.Object);
	dynamic_frame->SetupAttachment(RootComponent);
	dynamic_frame->SetSimulatePhysics(false);
	dynamic_frame->SetRelativeLocation({0, 0, 93.0f});
	dynamic_frame->SetRelativeRotation({0,60.0f,0});

	// // // Leg 1 start // // //
	lower_yoke_driven_1 = CreateDefaultSubobject<UStaticMeshComponent>("LowerYokeDriven");
	lower_yoke_driven_1->SetStaticMesh(lower_yoke_driven_asset.Object);
	lower_yoke_driven_1->SetupAttachment(fixed_frame);
	lower_yoke_driven_1->SetSimulatePhysics(true);
	lower_yoke_driven_1->SetRelativeLocation({18.556623f,-55.235027f,2.6f});
	lower_yoke_driven_1->SetRelativeRotation({0.0f,150.0f,0});
	// FBodyInstance* BodyInst = lower_yoke_driven_1->GetBodyInstance();
	// BodyInst->SetMassScale(1222.0f);
	// BodyInst->UpdateMassProperties();
	// lower_yoke_driven_1->SetMassOverrideInKg("LowerYokeDriven", 1.0f);
	lower_yoke_driven_1->SetMassOverrideInKg(NAME_None, 1.0f);

	lower_spider_1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LowerSpider"));
	lower_spider_1->SetStaticMesh(lower_spider_asset.Object);
	lower_spider_1->SetupAttachment(lower_yoke_driven_1);
	lower_spider_1->SetSimulatePhysics(false);
	lower_spider_1->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	lower_spider_1->SetRelativeRotation({0,0,-16.5f}); // Same as lower yoke start roll
	
	lower_yoke_driver_1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LowerYokeDriver"));
	lower_yoke_driver_1->SetStaticMesh(lower_yoke_driver_asset.Object);
	lower_yoke_driver_1->SetupAttachment(lower_yoke_driven_1);
	lower_yoke_driver_1->SetSimulatePhysics(true);
	lower_yoke_driver_1->SetMobility(EComponentMobility::Movable);
	lower_yoke_driver_1->SetRelativeRotation({1.95f,0, -16.5f});
	lower_yoke_driver_1->SetMassOverrideInKg(NAME_None, 1.0f);

	// Constraint settings
	constexpr   EAngularConstraintMotion _rotation_locked = ACM_Locked;
	
	// Must be created in constructor
	lower_revolute_1_joint_1 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("RevoluteJoint"));
	lower_revolute_1_joint_1->SetupAttachment(lower_yoke_driven_1);
	lower_revolute_1_joint_1->ComponentName1.ComponentName = "LowerYokeDriven";
	lower_revolute_1_joint_1->ComponentName2.ComponentName = "LowerYokeDriver";
	lower_revolute_1_joint_1->SetDisableCollision(true);
	lower_revolute_1_joint_1->SetAngularSwing1Limit(_rotation_locked, 0);  // This one must be locked
	
	cylinder_1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cylinder"));
	cylinder_1->SetStaticMesh(cylinder_asset.Object);
	cylinder_1->SetupAttachment(lower_yoke_driver_1);
	cylinder_1->SetSimulatePhysics(true);
	cylinder_1->SetMobility(EComponentMobility::Movable);
	cylinder_1->SetRelativeLocation({0,0,40.35f});
	cylinder_1->SetMassOverrideInKg(NAME_None, 1.0f);
		
	yoke_driver_fixed_cylinder_1 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("FixedJointYokeCylinder"));
	yoke_driver_fixed_cylinder_1->SetupAttachment(lower_yoke_driver_1);
	yoke_driver_fixed_cylinder_1->ComponentName1.ComponentName = "LowerYokeDriver";
	yoke_driver_fixed_cylinder_1->ComponentName2.ComponentName = "Cylinder";
	yoke_driver_fixed_cylinder_1->SetDisableCollision(true);
	// yoke_driven_fixed_cylinder->ConstraintInstance = FConstraintInstance // Figure out how to set theses up
	yoke_driver_fixed_cylinder_1->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0);
	yoke_driver_fixed_cylinder_1->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);
	yoke_driver_fixed_cylinder_1->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);
	yoke_driver_fixed_cylinder_1->SetRelativeLocation({0,0,2.5});

	piston_1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Piston"));
	piston_1->SetStaticMesh(piston_asset.Object);
	piston_1->SetupAttachment(cylinder_1);
	piston_1->SetSimulatePhysics(true);
	piston_1->SetMobility(EComponentMobility::Movable);
	float piston_start = 6.0;
	piston_1->SetRelativeLocation({0,0,piston_start});  // {0, 0, 40.0f}
	piston_1->SetRelativeRotation({0,0.0f,0});
	piston_1->SetMassOverrideInKg(NAME_None, 1.0f);

	cylinder_prismatic_piston_1 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PrismaticCylinderPiston"));
	cylinder_prismatic_piston_1->SetupAttachment(cylinder_1);
	cylinder_prismatic_piston_1->ComponentName1.ComponentName = "Cylinder";
	cylinder_prismatic_piston_1->ComponentName2.ComponentName = "Piston";
	cylinder_prismatic_piston_1->SetDisableCollision(true);
	cylinder_prismatic_piston_1->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0);
	cylinder_prismatic_piston_1->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);
	cylinder_prismatic_piston_1->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);
	cylinder_prismatic_piston_1->SetLinearZLimit(ELinearConstraintMotion::LCM_Limited, 35.0f);

	upper_yoke_driven_1 = CreateDefaultSubobject<UStaticMeshComponent>("UpperYokeDriven");
	upper_yoke_driven_1->SetStaticMesh(upper_yoke_driven_asset.Object);
	upper_yoke_driven_1->SetupAttachment(piston_1);
	upper_yoke_driven_1->SetSimulatePhysics(true);
	upper_yoke_driven_1->SetMobility(EComponentMobility::Movable);
	upper_yoke_driven_1->SetRelativeLocation({0,0,42.6f});
	upper_yoke_driven_1->SetMassOverrideInKg(NAME_None, 1.0f);
	
	yoke_driven_fixed_piston_1 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("YokeDrivenFixedPiston1"));
	yoke_driven_fixed_piston_1->SetupAttachment(piston_1);
	yoke_driven_fixed_piston_1->ComponentName1.ComponentName = "Piston";
	yoke_driven_fixed_piston_1->ComponentName2.ComponentName = "UpperYokeDriven";
	yoke_driven_fixed_piston_1->SetDisableCollision(true);
	yoke_driven_fixed_piston_1->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0);
	yoke_driven_fixed_piston_1->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);
	yoke_driven_fixed_piston_1->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);
	yoke_driven_fixed_piston_1->SetRelativeLocation({0,0,42.6f});
	
	upper_yoke_driver_1 = CreateDefaultSubobject<UStaticMeshComponent>("UpperYokeDriver");
	upper_yoke_driver_1->SetStaticMesh(upper_yoke_driver_asset.Object);
	upper_yoke_driver_1->SetupAttachment(upper_yoke_driven_1);
	upper_yoke_driver_1->SetSimulatePhysics(true);
	upper_yoke_driver_1->SetMobility(EComponentMobility::Movable);
	upper_yoke_driver_1->SetRelativeRotation({-1.95f,0, 16.5f});
	upper_yoke_driver_1->SetMassOverrideInKg(NAME_None, 1.0f);

	upper_revolute_1_joint_1 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("UpperRevolute1Joint1"));
	upper_revolute_1_joint_1->SetupAttachment(upper_yoke_driven_1);
	upper_revolute_1_joint_1->ComponentName1.ComponentName = "UpperYokeDriven";
	upper_revolute_1_joint_1->ComponentName2.ComponentName = "UpperYokeDriver";
	upper_revolute_1_joint_1->SetDisableCollision(true);
	upper_revolute_1_joint_1->SetAngularSwing1Limit(_rotation_locked, 0);  // This one must be locked

	upper_spider_1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UpperSpider"));
	upper_spider_1->SetStaticMesh(lower_spider_asset.Object);
	upper_spider_1->SetupAttachment(upper_yoke_driven_1);
	upper_spider_1->SetSimulatePhysics(false);
	upper_spider_1->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	upper_spider_1->SetMobility(EComponentMobility::Movable);
	upper_spider_1->SetRelativeRotation({0, 0, 16.5f});

	upper_revolute_disk_1 = CreateDefaultSubobject<UStaticMeshComponent>("UpperRevoluteDisk");
	upper_revolute_disk_1->SetStaticMesh(upper_revolute_disk_asset.Object);
	upper_revolute_disk_1->SetupAttachment(upper_yoke_driver_1);
	upper_revolute_disk_1->SetSimulatePhysics(true);
	upper_revolute_disk_1->SetMobility(EComponentMobility::Movable);
	upper_revolute_disk_1->SetRelativeLocation({0, 0, 2.5f});
	upper_revolute_disk_1->SetMassOverrideInKg(NAME_None, 1.0f);

	upper_revolute_2_joint_1 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("UpperRevolute2Joint1"));
	upper_revolute_2_joint_1->SetupAttachment(upper_yoke_driver_1);
	upper_revolute_2_joint_1->ComponentName1.ComponentName = "UpperYokeDriver";
	upper_revolute_2_joint_1->ComponentName2.ComponentName = "UpperRevoluteDisk";
	upper_revolute_2_joint_1->SetDisableCollision(true);
	upper_revolute_2_joint_1->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);  // This one must be locked
	upper_revolute_2_joint_1->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);  // This one must be locked
	upper_revolute_2_joint_1->SetRelativeLocation({0,0,2.5f});
	// Leg 1 End // // // // // // // // // // // // // // // // // // // // // // // //

	// Leg 2 Start // // // // // // // // // // // // // // // // // // // // // // // //
	lower_yoke_driven_2 = CreateDefaultSubobject<UStaticMeshComponent>("LowerYokeDriven_2");
	lower_yoke_driven_2->SetStaticMesh(lower_yoke_driven_asset.Object);
	lower_yoke_driven_2->SetupAttachment(fixed_frame);
	lower_yoke_driven_2->SetSimulatePhysics(true);
	lower_yoke_driven_2->SetRelativeLocation({57.113251f,11.547006f,2.6f});
	lower_yoke_driven_2->SetRelativeRotation({0.0f,150.0f,0});
	lower_yoke_driven_2->SetMassOverrideInKg(NAME_None, 1.0f);

	lower_spider_2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LowerSpider_2"));
	lower_spider_2->SetStaticMesh(lower_spider_asset.Object);
	lower_spider_2->SetupAttachment(lower_yoke_driven_2);
	lower_spider_2->SetSimulatePhysics(false);
	lower_spider_2->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	lower_spider_2->SetRelativeRotation({0,0,16.5f}); // Same as lower yoke start roll

	lower_yoke_driver_2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LowerYokeDriver_2"));
	lower_yoke_driver_2->SetStaticMesh(lower_yoke_driver_asset.Object);
	lower_yoke_driver_2->SetupAttachment(lower_yoke_driven_2);
	lower_yoke_driver_2->SetSimulatePhysics(true);
	lower_yoke_driver_2->SetMobility(EComponentMobility::Movable);
	lower_yoke_driver_2->SetRelativeRotation({1.95f,0, 16.5f});
	lower_yoke_driver_2->SetMassOverrideInKg(NAME_None, 1.0f);
	
	// Must be created in constructor
	lower_revolute_1_joint_2 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("RevoluteJoint_2"));
	lower_revolute_1_joint_2->SetupAttachment(lower_yoke_driven_2);
	lower_revolute_1_joint_2->ComponentName1.ComponentName = "LowerYokeDriven_2";
	lower_revolute_1_joint_2->ComponentName2.ComponentName = "LowerYokeDriver_2";
	lower_revolute_1_joint_2->SetDisableCollision(true);
	lower_revolute_1_joint_2->SetAngularSwing1Limit(_rotation_locked, 0);  // This one must be locked
	
	cylinder_2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cylinder_2"));
	cylinder_2->SetStaticMesh(cylinder_asset.Object);
	cylinder_2->SetupAttachment(lower_yoke_driver_2);
	cylinder_2->SetSimulatePhysics(true);
	cylinder_2->SetMobility(EComponentMobility::Movable);
	cylinder_2->SetRelativeLocation({0,0,40.35f});
	cylinder_2->SetMassOverrideInKg(NAME_None, 1.0f);
		
	yoke_driver_fixed_cylinder_2 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("FixedJointYokeCylinder_2"));
	yoke_driver_fixed_cylinder_2->SetupAttachment(lower_yoke_driver_2);
	yoke_driver_fixed_cylinder_2->ComponentName1.ComponentName = "LowerYokeDriver_2";
	yoke_driver_fixed_cylinder_2->ComponentName2.ComponentName = "Cylinder_2";
	yoke_driver_fixed_cylinder_2->SetDisableCollision(true);
	yoke_driver_fixed_cylinder_2->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0);
	yoke_driver_fixed_cylinder_2->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);
	yoke_driver_fixed_cylinder_2->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);
	yoke_driver_fixed_cylinder_2->SetRelativeLocation({0,0,2.5});

	piston_2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Piston_2"));
	piston_2->SetStaticMesh(piston_asset.Object);
	piston_2->SetupAttachment(cylinder_2);
	piston_2->SetSimulatePhysics(true);
	piston_2->SetMobility(EComponentMobility::Movable);
	piston_2->SetRelativeLocation({0,0,piston_start});  // {0, 0, 40.0f}
	piston_2->SetRelativeRotation({0,0.0f,0});
	piston_2->SetMassOverrideInKg(NAME_None, 1.0f);

	cylinder_prismatic_piston_2 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PrismaticCylinderPiston_2"));
	cylinder_prismatic_piston_2->SetupAttachment(cylinder_2);
	cylinder_prismatic_piston_2->ComponentName1.ComponentName = "Cylinder_2";
	cylinder_prismatic_piston_2->ComponentName2.ComponentName = "Piston_2";
	cylinder_prismatic_piston_2->SetDisableCollision(true);
	cylinder_prismatic_piston_2->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0);
	cylinder_prismatic_piston_2->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);
	cylinder_prismatic_piston_2->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);
	cylinder_prismatic_piston_2->SetLinearZLimit(ELinearConstraintMotion::LCM_Limited, 35.0f);

	upper_yoke_driven_2 = CreateDefaultSubobject<UStaticMeshComponent>("UpperYokeDriven_2");
	upper_yoke_driven_2->SetStaticMesh(upper_yoke_driven_asset.Object);
	upper_yoke_driven_2->SetupAttachment(piston_2);
	upper_yoke_driven_2->SetSimulatePhysics(true);
	upper_yoke_driven_2->SetMobility(EComponentMobility::Movable);
	upper_yoke_driven_2->SetRelativeLocation({0,0,42.6f});
	upper_yoke_driven_2->SetMassOverrideInKg(NAME_None, 1.0f);
	
	yoke_driven_fixed_piston_2 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("YokeDrivenFixedPiston1_2"));
	yoke_driven_fixed_piston_2->SetupAttachment(piston_2);
	yoke_driven_fixed_piston_2->ComponentName1.ComponentName = "Piston_2";
	yoke_driven_fixed_piston_2->ComponentName2.ComponentName = "UpperYokeDriven_2";
	yoke_driven_fixed_piston_2->SetDisableCollision(true);
	yoke_driven_fixed_piston_2->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0);
	yoke_driven_fixed_piston_2->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);
	yoke_driven_fixed_piston_2->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);
	yoke_driven_fixed_piston_2->SetRelativeLocation({0,0,42.6f});
	
	upper_yoke_driver_2 = CreateDefaultSubobject<UStaticMeshComponent>("UpperYokeDriver_2");
	upper_yoke_driver_2->SetStaticMesh(upper_yoke_driver_asset.Object);
	upper_yoke_driver_2->SetupAttachment(upper_yoke_driven_2);
	upper_yoke_driver_2->SetSimulatePhysics(true);
	upper_yoke_driver_2->SetMobility(EComponentMobility::Movable);
	upper_yoke_driver_2->SetRelativeRotation({-1.95f,0, -16.5f});
	upper_yoke_driver_2->SetMassOverrideInKg(NAME_None, 1.0f);

	upper_revolute_1_joint_2 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("UpperRevolute1Joint1_2"));
	upper_revolute_1_joint_2->SetupAttachment(upper_yoke_driven_2);
	upper_revolute_1_joint_2->ComponentName1.ComponentName = "UpperYokeDriven_2";
	upper_revolute_1_joint_2->ComponentName2.ComponentName = "UpperYokeDriver_2";
	upper_revolute_1_joint_2->SetDisableCollision(true);
	upper_revolute_1_joint_2->SetAngularSwing1Limit(_rotation_locked, 0);  // This one must be locked

	upper_spider_2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UpperSpider_2"));
	upper_spider_2->SetStaticMesh(lower_spider_asset.Object);
	upper_spider_2->SetupAttachment(upper_yoke_driven_2);
	upper_spider_2->SetSimulatePhysics(false);
	upper_spider_2->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	upper_spider_2->SetMobility(EComponentMobility::Movable);
	upper_spider_2->SetRelativeRotation({0, 0, -16.5f});

	upper_revolute_disk_2 = CreateDefaultSubobject<UStaticMeshComponent>("UpperRevoluteDisk_2");
	upper_revolute_disk_2->SetStaticMesh(upper_revolute_disk_asset.Object);
	upper_revolute_disk_2->SetupAttachment(upper_yoke_driver_2);
	upper_revolute_disk_2->SetSimulatePhysics(true);
	upper_revolute_disk_2->SetMobility(EComponentMobility::Movable);
	upper_revolute_disk_2->SetRelativeLocation({0, 0, 2.5f});
	upper_revolute_disk_2->SetMassOverrideInKg(NAME_None, 1.0f);

	upper_revolute_2_joint_2 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("UpperRevolute2Joint1_2"));
	upper_revolute_2_joint_2->SetupAttachment(upper_yoke_driver_2);
	upper_revolute_2_joint_2->ComponentName1.ComponentName = "UpperYokeDriver_2";
	upper_revolute_2_joint_2->ComponentName2.ComponentName = "UpperRevoluteDisk_2";
	upper_revolute_2_joint_2->SetDisableCollision(true);
	upper_revolute_2_joint_2->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);  // This one must be locked
	upper_revolute_2_joint_2->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);  // This one must be locked
	upper_revolute_2_joint_2->SetRelativeLocation({0,0,2.5f});
	// Leg 2 End // // // // // // // // // // // // // // // // // // // // // // // //

	// Leg 3 Start // // // // // // // // // // // // // // // // // // // // // // // //
	lower_yoke_driven_3 = CreateDefaultSubobject<UStaticMeshComponent>("LowerYokeDriven_3");
	lower_yoke_driven_3->SetStaticMesh(lower_yoke_driven_asset.Object);
	lower_yoke_driven_3->SetupAttachment(fixed_frame);
	lower_yoke_driven_3->SetSimulatePhysics(true);
	lower_yoke_driven_3->SetRelativeLocation({38.556625f,43.688023f,2.6f});
	lower_yoke_driven_3->SetRelativeRotation({0.0f,90.0f,0});
	lower_yoke_driven_3->SetMassOverrideInKg(NAME_None, 1.0f);

	lower_spider_3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LowerSpider_3"));
	lower_spider_3->SetStaticMesh(lower_spider_asset.Object);
	lower_spider_3->SetupAttachment(lower_yoke_driven_3);
	lower_spider_3->SetSimulatePhysics(false);
	lower_spider_3->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	lower_spider_3->SetRelativeRotation({0,0,16.5f}); // Same as lower yoke start roll

	lower_yoke_driver_3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LowerYokeDriver_3"));
	lower_yoke_driver_3->SetStaticMesh(lower_yoke_driver_asset.Object);
	lower_yoke_driver_3->SetupAttachment(lower_yoke_driven_3);
	lower_yoke_driver_3->SetSimulatePhysics(true);
	lower_yoke_driver_3->SetMobility(EComponentMobility::Movable);
	lower_yoke_driver_3->SetRelativeRotation({-1.95f,0, 16.5f});
	lower_yoke_driver_3->SetMassOverrideInKg(NAME_None, 1.0f);
	
	// Must be created in constructor
	lower_revolute_1_joint_3 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("RevoluteJoint_3"));
	lower_revolute_1_joint_3->SetupAttachment(lower_yoke_driven_3);
	lower_revolute_1_joint_3->ComponentName1.ComponentName = "LowerYokeDriven_3";
	lower_revolute_1_joint_3->ComponentName2.ComponentName = "LowerYokeDriver_3";
	lower_revolute_1_joint_3->SetDisableCollision(true);
	lower_revolute_1_joint_3->SetAngularSwing1Limit(_rotation_locked, 0);  // This one must be locked
	
	cylinder_3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cylinder_3"));
	cylinder_3->SetStaticMesh(cylinder_asset.Object);
	cylinder_3->SetupAttachment(lower_yoke_driver_3);
	cylinder_3->SetSimulatePhysics(true);
	cylinder_3->SetMobility(EComponentMobility::Movable);
	cylinder_3->SetRelativeLocation({0,0,40.35f});
	cylinder_3->SetMassOverrideInKg(NAME_None, 1.0f);
		
	yoke_driver_fixed_cylinder_3 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("FixedJointYokeCylinder_3"));
	yoke_driver_fixed_cylinder_3->SetupAttachment(lower_yoke_driver_3);
	yoke_driver_fixed_cylinder_3->ComponentName1.ComponentName = "LowerYokeDriver_3";
	yoke_driver_fixed_cylinder_3->ComponentName2.ComponentName = "Cylinder_3";
	yoke_driver_fixed_cylinder_3->SetDisableCollision(true);
	yoke_driver_fixed_cylinder_3->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0);
	yoke_driver_fixed_cylinder_3->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);
	yoke_driver_fixed_cylinder_3->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);
	yoke_driver_fixed_cylinder_3->SetRelativeLocation({0,0,2.5});

	piston_3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Piston_3"));
	piston_3->SetStaticMesh(piston_asset.Object);
	piston_3->SetupAttachment(cylinder_3);
	piston_3->SetSimulatePhysics(true);
	piston_3->SetMobility(EComponentMobility::Movable);
	piston_3->SetRelativeLocation({0,0,piston_start});  // {0, 0, 40.0f}
	piston_3->SetRelativeRotation({0,0.0f,0});
	piston_3->SetMassOverrideInKg(NAME_None, 1.0f);

	cylinder_prismatic_piston_3 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PrismaticCylinderPiston_3"));
	cylinder_prismatic_piston_3->SetupAttachment(cylinder_3);
	cylinder_prismatic_piston_3->ComponentName1.ComponentName = "Cylinder_3";
	cylinder_prismatic_piston_3->ComponentName2.ComponentName = "Piston_3";
	cylinder_prismatic_piston_3->SetDisableCollision(true);
	cylinder_prismatic_piston_3->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0);
	cylinder_prismatic_piston_3->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);
	cylinder_prismatic_piston_3->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);
	cylinder_prismatic_piston_3->SetLinearZLimit(ELinearConstraintMotion::LCM_Limited, 35.0f);

	upper_yoke_driven_3 = CreateDefaultSubobject<UStaticMeshComponent>("UpperYokeDriven_3");
	upper_yoke_driven_3->SetStaticMesh(upper_yoke_driven_asset.Object);
	upper_yoke_driven_3->SetupAttachment(piston_3);
	upper_yoke_driven_3->SetSimulatePhysics(true);
	upper_yoke_driven_3->SetMobility(EComponentMobility::Movable);
	upper_yoke_driven_3->SetRelativeLocation({0,0,42.6f});
	upper_yoke_driven_3->SetMassOverrideInKg(NAME_None, 1.0f);
	
	yoke_driven_fixed_piston_3 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("YokeDrivenFixedPiston1_3"));
	yoke_driven_fixed_piston_3->SetupAttachment(piston_3);
	yoke_driven_fixed_piston_3->ComponentName1.ComponentName = "Piston_3";
	yoke_driven_fixed_piston_3->ComponentName2.ComponentName = "UpperYokeDriven_3";
	yoke_driven_fixed_piston_3->SetDisableCollision(true);
	yoke_driven_fixed_piston_3->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0);
	yoke_driven_fixed_piston_3->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);
	yoke_driven_fixed_piston_3->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);
	yoke_driven_fixed_piston_3->SetRelativeLocation({0,0,42.6f});
	
	upper_yoke_driver_3 = CreateDefaultSubobject<UStaticMeshComponent>("UpperYokeDriver_3");
	upper_yoke_driver_3->SetStaticMesh(upper_yoke_driver_asset.Object);
	upper_yoke_driver_3->SetupAttachment(upper_yoke_driven_3);
	upper_yoke_driver_3->SetSimulatePhysics(true);
	upper_yoke_driver_3->SetMobility(EComponentMobility::Movable);
	upper_yoke_driver_3->SetRelativeRotation({1.95f,0, -16.5f});
	upper_yoke_driver_3->SetMassOverrideInKg(NAME_None, 1.0f);

	upper_revolute_1_joint_3 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("UpperRevolute1Joint1_3"));
	upper_revolute_1_joint_3->SetupAttachment(upper_yoke_driven_3);
	upper_revolute_1_joint_3->ComponentName1.ComponentName = "UpperYokeDriven_3";
	upper_revolute_1_joint_3->ComponentName2.ComponentName = "UpperYokeDriver_3";
	upper_revolute_1_joint_3->SetDisableCollision(true);
	upper_revolute_1_joint_3->SetAngularSwing1Limit(_rotation_locked, 0);  // This one must be locked

	upper_spider_3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UpperSpider_3"));
	upper_spider_3->SetStaticMesh(lower_spider_asset.Object);
	upper_spider_3->SetupAttachment(upper_yoke_driven_3);
	upper_spider_3->SetSimulatePhysics(false);
	upper_spider_3->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	upper_spider_3->SetMobility(EComponentMobility::Movable);
	upper_spider_3->SetRelativeRotation({0, 0, -16.5f});

	upper_revolute_disk_3 = CreateDefaultSubobject<UStaticMeshComponent>("UpperRevoluteDisk_3");
	upper_revolute_disk_3->SetStaticMesh(upper_revolute_disk_asset.Object);
	upper_revolute_disk_3->SetupAttachment(upper_yoke_driver_3);
	upper_revolute_disk_3->SetSimulatePhysics(true);
	upper_revolute_disk_3->SetMobility(EComponentMobility::Movable);
	upper_revolute_disk_3->SetRelativeLocation({0, 0, 2.5f});
	upper_revolute_disk_3->SetMassOverrideInKg(NAME_None, 1.0f);

	upper_revolute_2_joint_3 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("UpperRevolute2Joint1_3"));
	upper_revolute_2_joint_3->SetupAttachment(upper_yoke_driver_3);
	upper_revolute_2_joint_3->ComponentName1.ComponentName = "UpperYokeDriver_3";
	upper_revolute_2_joint_3->ComponentName2.ComponentName = "UpperRevoluteDisk_3";
	upper_revolute_2_joint_3->SetDisableCollision(true);
	upper_revolute_2_joint_3->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);  // This one must be locked
	upper_revolute_2_joint_3->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);  // This one must be locked
	upper_revolute_2_joint_3->SetRelativeLocation({0,0,2.5f});
	// Leg 3 End // // // // // // // // // // // // // // // // // // // // // // // //

	// Leg 4 Start // // // // // // // // // // // // // // // // // // // // // // // //
	lower_yoke_driven_4 = CreateDefaultSubobject<UStaticMeshComponent>("LowerYokeDriven_4");
	lower_yoke_driven_4->SetStaticMesh(lower_yoke_driven_asset.Object);
	lower_yoke_driven_4->SetupAttachment(fixed_frame);
	lower_yoke_driven_4->SetSimulatePhysics(true);
	lower_yoke_driven_4->SetRelativeLocation({-38.556625f,43.688023f,2.6f});
	lower_yoke_driven_4->SetRelativeRotation({0.0f,90.0f,0});
	lower_yoke_driven_4->SetMassOverrideInKg(NAME_None, 1.0f);

	lower_spider_4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LowerSpider_4"));
	lower_spider_4->SetStaticMesh(lower_spider_asset.Object);
	lower_spider_4->SetupAttachment(lower_yoke_driven_4);
	lower_spider_4->SetSimulatePhysics(false);
	lower_spider_4->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	lower_spider_4->SetRelativeRotation({0,0,-16.5f}); // Same as lower yoke start roll

	lower_yoke_driver_4= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LowerYokeDriver_4"));
	lower_yoke_driver_4->SetStaticMesh(lower_yoke_driver_asset.Object);
	lower_yoke_driver_4->SetupAttachment(lower_yoke_driven_4);
	lower_yoke_driver_4->SetSimulatePhysics(true);
	lower_yoke_driver_4->SetMobility(EComponentMobility::Movable);
	lower_yoke_driver_4->SetRelativeRotation({-1.95f,0, -16.5f});
	lower_yoke_driver_4->SetMassOverrideInKg(NAME_None, 1.0f);
	
	lower_revolute_1_joint_4 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("RevoluteJoint_4"));
	lower_revolute_1_joint_4->SetupAttachment(lower_yoke_driven_4);
	lower_revolute_1_joint_4->ComponentName1.ComponentName = "LowerYokeDriven_4";
	lower_revolute_1_joint_4->ComponentName2.ComponentName = "LowerYokeDriver_4";
	lower_revolute_1_joint_4->SetDisableCollision(true);
	lower_revolute_1_joint_4->SetAngularSwing1Limit(_rotation_locked, 0);  // This one must be locked
	
	cylinder_4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cylinder_4"));
	cylinder_4->SetStaticMesh(cylinder_asset.Object);
	cylinder_4->SetupAttachment(lower_yoke_driver_4);
	cylinder_4->SetSimulatePhysics(true);
	cylinder_4->SetMobility(EComponentMobility::Movable);
	cylinder_4->SetRelativeLocation({0,0,40.35f});
	cylinder_4->SetMassOverrideInKg(NAME_None, 1.0f);
		
	yoke_driver_fixed_cylinder_4 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("FixedJointYokeCylinder_4"));
	yoke_driver_fixed_cylinder_4->SetupAttachment(lower_yoke_driver_4);
	yoke_driver_fixed_cylinder_4->ComponentName1.ComponentName = "LowerYokeDriver_4";
	yoke_driver_fixed_cylinder_4->ComponentName2.ComponentName = "Cylinder_4";
	yoke_driver_fixed_cylinder_4->SetDisableCollision(true);
	yoke_driver_fixed_cylinder_4->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0);
	yoke_driver_fixed_cylinder_4->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);
	yoke_driver_fixed_cylinder_4->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);
	yoke_driver_fixed_cylinder_4->SetRelativeLocation({0,0,2.5});

	piston_4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Piston_4"));
	piston_4->SetStaticMesh(piston_asset.Object);
	piston_4->SetupAttachment(cylinder_4);
	piston_4->SetSimulatePhysics(true);
	piston_4->SetMobility(EComponentMobility::Movable);
	piston_4->SetRelativeLocation({0,0,piston_start});  // {0, 0, 40.0f}
	piston_4->SetRelativeRotation({0,0.0f,0});
	piston_4->SetMassOverrideInKg(NAME_None, 1.0f);

	cylinder_prismatic_piston_4 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PrismaticCylinderPiston_4"));
	cylinder_prismatic_piston_4->SetupAttachment(cylinder_4);
	cylinder_prismatic_piston_4->ComponentName1.ComponentName = "Cylinder_4";
	cylinder_prismatic_piston_4->ComponentName2.ComponentName = "Piston_4";
	cylinder_prismatic_piston_4->SetDisableCollision(true);
	cylinder_prismatic_piston_4->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0);
	cylinder_prismatic_piston_4->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);
	cylinder_prismatic_piston_4->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);
	cylinder_prismatic_piston_4->SetLinearZLimit(ELinearConstraintMotion::LCM_Limited, 35.0f);

	upper_yoke_driven_4 = CreateDefaultSubobject<UStaticMeshComponent>("UpperYokeDriven_4");
	upper_yoke_driven_4->SetStaticMesh(upper_yoke_driven_asset.Object);
	upper_yoke_driven_4->SetupAttachment(piston_4);
	upper_yoke_driven_4->SetSimulatePhysics(true);
	upper_yoke_driven_4->SetMobility(EComponentMobility::Movable);
	upper_yoke_driven_4->SetRelativeLocation({0,0,42.6f});
	upper_yoke_driven_4->SetMassOverrideInKg(NAME_None, 1.0f);
	
	yoke_driven_fixed_piston_4 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("YokeDrivenFixedPiston1_4"));
	yoke_driven_fixed_piston_4->SetupAttachment(piston_4);
	yoke_driven_fixed_piston_4->ComponentName1.ComponentName = "Piston_4";
	yoke_driven_fixed_piston_4->ComponentName2.ComponentName = "UpperYokeDriven_4";
	yoke_driven_fixed_piston_4->SetDisableCollision(true);
	yoke_driven_fixed_piston_4->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0);
	yoke_driven_fixed_piston_4->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);
	yoke_driven_fixed_piston_4->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);
	yoke_driven_fixed_piston_4->SetRelativeLocation({0,0,42.6f});
	
	upper_yoke_driver_4 = CreateDefaultSubobject<UStaticMeshComponent>("UpperYokeDriver_4");
	upper_yoke_driver_4->SetStaticMesh(upper_yoke_driver_asset.Object);
	upper_yoke_driver_4->SetupAttachment(upper_yoke_driven_4);
	upper_yoke_driver_4->SetSimulatePhysics(true);
	upper_yoke_driver_4->SetMobility(EComponentMobility::Movable);
	upper_yoke_driver_4->SetRelativeRotation({1.95f,0, 16.5f});
	upper_yoke_driver_4->SetMassOverrideInKg(NAME_None, 1.0f);

	upper_revolute_1_joint_4 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("UpperRevolute1Joint1_4"));
	upper_revolute_1_joint_4->SetupAttachment(upper_yoke_driven_4);
	upper_revolute_1_joint_4->ComponentName1.ComponentName = "UpperYokeDriven_4";
	upper_revolute_1_joint_4->ComponentName2.ComponentName = "UpperYokeDriver_4";
	upper_revolute_1_joint_4->SetDisableCollision(true);
	upper_revolute_1_joint_4->SetAngularSwing1Limit(_rotation_locked, 0);  // This one must be locked

	upper_spider_4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UpperSpider_4"));
	upper_spider_4->SetStaticMesh(lower_spider_asset.Object);
	upper_spider_4->SetupAttachment(upper_yoke_driven_4);
	upper_spider_4->SetSimulatePhysics(false);
	upper_spider_4->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	upper_spider_4->SetMobility(EComponentMobility::Movable);
	upper_spider_4->SetRelativeRotation({0, 0, 16.5f});

	upper_revolute_disk_4 = CreateDefaultSubobject<UStaticMeshComponent>("UpperRevoluteDisk_4");
	upper_revolute_disk_4->SetStaticMesh(upper_revolute_disk_asset.Object);
	upper_revolute_disk_4->SetupAttachment(upper_yoke_driver_4);
	upper_revolute_disk_4->SetSimulatePhysics(true);
	upper_revolute_disk_4->SetMobility(EComponentMobility::Movable);
	upper_revolute_disk_4->SetRelativeLocation({0, 0, 2.5f});
	upper_revolute_disk_4->SetMassOverrideInKg(NAME_None, 1.0f);

	upper_revolute_2_joint_4 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("UpperRevolute2Joint1_4"));
	upper_revolute_2_joint_4->SetupAttachment(upper_yoke_driver_4);
	upper_revolute_2_joint_4->ComponentName1.ComponentName = "UpperYokeDriver_4";
	upper_revolute_2_joint_4->ComponentName2.ComponentName = "UpperRevoluteDisk_4";
	upper_revolute_2_joint_4->SetDisableCollision(true);
	upper_revolute_2_joint_4->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);  // This one must be locked
	upper_revolute_2_joint_4->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);  // This one must be locked
	upper_revolute_2_joint_4->SetRelativeLocation({0,0,2.5f});
	// Leg 4 End // // // // // // // // // // // // // // // // // // // // // // // //

	// Leg 5 Start // // // // // // // // // // // // // // // // // // // // // // // //
	lower_yoke_driven_5 = CreateDefaultSubobject<UStaticMeshComponent>("LowerYokeDriven_5");
	lower_yoke_driven_5->SetStaticMesh(lower_yoke_driven_asset.Object);
	lower_yoke_driven_5->SetupAttachment(fixed_frame);
	lower_yoke_driven_5->SetSimulatePhysics(true);
	lower_yoke_driven_5->SetRelativeLocation({-57.113251f,11.547006f,2.6f});
	lower_yoke_driven_5->SetRelativeRotation({0.0f,30.0f,0});
	lower_yoke_driven_5->SetMassOverrideInKg(NAME_None, 1.0f);

	lower_spider_5 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LowerSpider_5"));
	lower_spider_5->SetStaticMesh(lower_spider_asset.Object);
	lower_spider_5->SetupAttachment(lower_yoke_driven_5);
	lower_spider_5->SetSimulatePhysics(false);
	lower_spider_5->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	lower_spider_5->SetRelativeRotation({0,0,-16.5f}); // Same as lower yoke start roll

	lower_yoke_driver_5= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LowerYokeDriver_5"));
	lower_yoke_driver_5->SetStaticMesh(lower_yoke_driver_asset.Object);
	lower_yoke_driver_5->SetupAttachment(lower_yoke_driven_5);
	lower_yoke_driver_5->SetSimulatePhysics(true);
	lower_yoke_driver_5->SetMobility(EComponentMobility::Movable);
	lower_yoke_driver_5->SetRelativeRotation({1.95f,0, -16.5f});
	lower_yoke_driver_5->SetMassOverrideInKg(NAME_None, 1.0f);
	
	lower_revolute_1_joint_5 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("RevoluteJoint_5"));
	lower_revolute_1_joint_5->SetupAttachment(lower_yoke_driven_5);
	lower_revolute_1_joint_5->ComponentName1.ComponentName = "LowerYokeDriven_5";
	lower_revolute_1_joint_5->ComponentName2.ComponentName = "LowerYokeDriver_5";
	lower_revolute_1_joint_5->SetDisableCollision(true);
	lower_revolute_1_joint_5->SetAngularSwing1Limit(_rotation_locked, 0);  // This one must be locked
	
	cylinder_5 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cylinder_5"));
	cylinder_5->SetStaticMesh(cylinder_asset.Object);
	cylinder_5->SetupAttachment(lower_yoke_driver_5);
	cylinder_5->SetSimulatePhysics(true);
	cylinder_5->SetMobility(EComponentMobility::Movable);
	cylinder_5->SetRelativeLocation({0,0,40.35f});
	cylinder_5->SetMassOverrideInKg(NAME_None, 1.0f);
		
	yoke_driver_fixed_cylinder_5 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("FixedJointYokeCylinder_5"));
	yoke_driver_fixed_cylinder_5->SetupAttachment(lower_yoke_driver_5);
	yoke_driver_fixed_cylinder_5->ComponentName1.ComponentName = "LowerYokeDriver_5";
	yoke_driver_fixed_cylinder_5->ComponentName2.ComponentName = "Cylinder_5";
	yoke_driver_fixed_cylinder_5->SetDisableCollision(true);
	yoke_driver_fixed_cylinder_5->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0);
	yoke_driver_fixed_cylinder_5->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);
	yoke_driver_fixed_cylinder_5->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);
	yoke_driver_fixed_cylinder_5->SetRelativeLocation({0,0,2.5});

	piston_5 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Piston_5"));
	piston_5->SetStaticMesh(piston_asset.Object);
	piston_5->SetupAttachment(cylinder_5);
	piston_5->SetSimulatePhysics(true);
	piston_5->SetMobility(EComponentMobility::Movable);
	piston_5->SetRelativeLocation({0,0,piston_start});  // {0, 0, 40.0f}
	piston_5->SetRelativeRotation({0,0.0f,0});
	piston_5->SetMassOverrideInKg(NAME_None, 1.0f);

	cylinder_prismatic_piston_5 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PrismaticCylinderPiston_5"));
	cylinder_prismatic_piston_5->SetupAttachment(cylinder_5);
	cylinder_prismatic_piston_5->ComponentName1.ComponentName = "Cylinder_5";
	cylinder_prismatic_piston_5->ComponentName2.ComponentName = "Piston_5";
	cylinder_prismatic_piston_5->SetDisableCollision(true);
	cylinder_prismatic_piston_5->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0);
	cylinder_prismatic_piston_5->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);
	cylinder_prismatic_piston_5->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);
	cylinder_prismatic_piston_5->SetLinearZLimit(ELinearConstraintMotion::LCM_Limited, 35.0f);

	upper_yoke_driven_5 = CreateDefaultSubobject<UStaticMeshComponent>("UpperYokeDriven_5");
	upper_yoke_driven_5->SetStaticMesh(upper_yoke_driven_asset.Object);
	upper_yoke_driven_5->SetupAttachment(piston_5);
	upper_yoke_driven_5->SetSimulatePhysics(true);
	upper_yoke_driven_5->SetMobility(EComponentMobility::Movable);
	upper_yoke_driven_5->SetRelativeLocation({0,0,42.6f});
	upper_yoke_driven_5->SetMassOverrideInKg(NAME_None, 1.0f);
	
	yoke_driven_fixed_piston_5 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("YokeDrivenFixedPiston1_5"));
	yoke_driven_fixed_piston_5->SetupAttachment(piston_5);
	yoke_driven_fixed_piston_5->ComponentName1.ComponentName = "Piston_5";
	yoke_driven_fixed_piston_5->ComponentName2.ComponentName = "UpperYokeDriven_5";
	yoke_driven_fixed_piston_5->SetDisableCollision(true);
	yoke_driven_fixed_piston_5->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0);
	yoke_driven_fixed_piston_5->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);
	yoke_driven_fixed_piston_5->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);
	yoke_driven_fixed_piston_5->SetRelativeLocation({0,0,42.6f});
	
	upper_yoke_driver_5 = CreateDefaultSubobject<UStaticMeshComponent>("UpperYokeDriver_5");
	upper_yoke_driver_5->SetStaticMesh(upper_yoke_driver_asset.Object);
	upper_yoke_driver_5->SetupAttachment(upper_yoke_driven_5);
	upper_yoke_driver_5->SetSimulatePhysics(true);
	upper_yoke_driver_5->SetMobility(EComponentMobility::Movable);
	upper_yoke_driver_5->SetRelativeRotation({-1.95f,0, 16.5f});
	upper_yoke_driver_5->SetMassOverrideInKg(NAME_None, 1.0f);

	upper_revolute_1_joint_5 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("UpperRevolute1Joint1_5"));
	upper_revolute_1_joint_5->SetupAttachment(upper_yoke_driven_5);
	upper_revolute_1_joint_5->ComponentName1.ComponentName = "UpperYokeDriven_5";
	upper_revolute_1_joint_5->ComponentName2.ComponentName = "UpperYokeDriver_5";
	upper_revolute_1_joint_5->SetDisableCollision(true);
	upper_revolute_1_joint_5->SetAngularSwing1Limit(_rotation_locked, 0);  // This one must be locked

	upper_spider_5 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UpperSpider_5"));
	upper_spider_5->SetStaticMesh(lower_spider_asset.Object);
	upper_spider_5->SetupAttachment(upper_yoke_driven_5);
	upper_spider_5->SetSimulatePhysics(false);
	upper_spider_5->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	upper_spider_5->SetMobility(EComponentMobility::Movable);
	upper_spider_5->SetRelativeRotation({0, 0, 16.5f});

	upper_revolute_disk_5 = CreateDefaultSubobject<UStaticMeshComponent>("UpperRevoluteDisk_5");
	upper_revolute_disk_5->SetStaticMesh(upper_revolute_disk_asset.Object);
	upper_revolute_disk_5->SetupAttachment(upper_yoke_driver_5);
	upper_revolute_disk_5->SetSimulatePhysics(true);
	upper_revolute_disk_5->SetMobility(EComponentMobility::Movable);
	upper_revolute_disk_5->SetRelativeLocation({0, 0, 2.5f});
	upper_revolute_disk_5->SetMassOverrideInKg(NAME_None, 1.0f);

	upper_revolute_2_joint_5 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("UpperRevolute2Joint1_5"));
	upper_revolute_2_joint_5->SetupAttachment(upper_yoke_driver_5);
	upper_revolute_2_joint_5->ComponentName1.ComponentName = "UpperYokeDriver_5";
	upper_revolute_2_joint_5->ComponentName2.ComponentName = "UpperRevoluteDisk_5";
	upper_revolute_2_joint_5->SetDisableCollision(true);
	upper_revolute_2_joint_5->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);  // This one must be locked
	upper_revolute_2_joint_5->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);  // This one must be locked
	upper_revolute_2_joint_5->SetRelativeLocation({0,0,2.5f});
	// Leg 5 End // // // // // // // // // // // // // // // // // // // // // // // //

	// Leg 6 Start // // // // // // // // // // // // // // // // // // // // // // // //
	lower_yoke_driven_6 = CreateDefaultSubobject<UStaticMeshComponent>("LowerYokeDriven_6");
	lower_yoke_driven_6->SetStaticMesh(lower_yoke_driven_asset.Object);
	lower_yoke_driven_6->SetupAttachment(fixed_frame);
	lower_yoke_driven_6->SetSimulatePhysics(true);
	lower_yoke_driven_6->SetRelativeLocation({-18.556623f,-55.235027f,2.6f});
	lower_yoke_driven_6->SetRelativeRotation({0.0f,30.0f,0});
	lower_yoke_driven_6->SetMassOverrideInKg(NAME_None, 1.0f);

	lower_spider_6 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LowerSpider_6"));
	lower_spider_6->SetStaticMesh(lower_spider_asset.Object);
	lower_spider_6->SetupAttachment(lower_yoke_driven_6);
	lower_spider_6->SetSimulatePhysics(false);
	lower_spider_6->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	lower_spider_6->SetRelativeRotation({0,0,16.5f}); // Same as lower yoke start roll

	lower_yoke_driver_6= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LowerYokeDriver_6"));
	lower_yoke_driver_6->SetStaticMesh(lower_yoke_driver_asset.Object);
	lower_yoke_driver_6->SetupAttachment(lower_yoke_driven_6);
	lower_yoke_driver_6->SetSimulatePhysics(true);
	lower_yoke_driver_6->SetMobility(EComponentMobility::Movable);
	lower_yoke_driver_6->SetRelativeRotation({1.95f,0, 16.5f});
	lower_yoke_driver_6->SetMassOverrideInKg(NAME_None, 1.0f);
	
	lower_revolute_1_joint_6 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("RevoluteJoint_6"));
	lower_revolute_1_joint_6->SetupAttachment(lower_yoke_driven_6);
	lower_revolute_1_joint_6->ComponentName1.ComponentName = "LowerYokeDriven_6";
	lower_revolute_1_joint_6->ComponentName2.ComponentName = "LowerYokeDriver_6";
	lower_revolute_1_joint_6->SetDisableCollision(true);
	lower_revolute_1_joint_6->SetAngularSwing1Limit(_rotation_locked, 0);  // This one must be locked
	
	cylinder_6 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cylinder_6"));
	cylinder_6->SetStaticMesh(cylinder_asset.Object);
	cylinder_6->SetupAttachment(lower_yoke_driver_6);
	cylinder_6->SetSimulatePhysics(true);
	cylinder_6->SetMobility(EComponentMobility::Movable);
	cylinder_6->SetRelativeLocation({0,0,40.35f});
	cylinder_6->SetMassOverrideInKg(NAME_None, 1.0f);
		
	yoke_driver_fixed_cylinder_6 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("FixedJointYokeCylinder_6"));
	yoke_driver_fixed_cylinder_6->SetupAttachment(lower_yoke_driver_6);
	yoke_driver_fixed_cylinder_6->ComponentName1.ComponentName = "LowerYokeDriver_6";
	yoke_driver_fixed_cylinder_6->ComponentName2.ComponentName = "Cylinder_6";
	yoke_driver_fixed_cylinder_6->SetDisableCollision(true);
	yoke_driver_fixed_cylinder_6->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0);
	yoke_driver_fixed_cylinder_6->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);
	yoke_driver_fixed_cylinder_6->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);
	yoke_driver_fixed_cylinder_6->SetRelativeLocation({0,0,2.5});

	piston_6 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Piston_6"));
	piston_6->SetStaticMesh(piston_asset.Object);
	piston_6->SetupAttachment(cylinder_6);
	piston_6->SetSimulatePhysics(true);
	piston_6->SetMobility(EComponentMobility::Movable);
	piston_6->SetRelativeLocation({0,0,piston_start});  // {0, 0, 40.0f}
	piston_6->SetRelativeRotation({0,0.0f,0});
	piston_6->SetMassOverrideInKg(NAME_None, 1.0f);

	cylinder_prismatic_piston_6 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PrismaticCylinderPiston_6"));
	cylinder_prismatic_piston_6->SetupAttachment(cylinder_6);
	cylinder_prismatic_piston_6->ComponentName1.ComponentName = "Cylinder_6";
	cylinder_prismatic_piston_6->ComponentName2.ComponentName = "Piston_6";
	cylinder_prismatic_piston_6->SetDisableCollision(true);
	cylinder_prismatic_piston_6->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0);
	cylinder_prismatic_piston_6->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);
	cylinder_prismatic_piston_6->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);
	cylinder_prismatic_piston_6->SetLinearZLimit(ELinearConstraintMotion::LCM_Limited, 35.0f);

	upper_yoke_driven_6 = CreateDefaultSubobject<UStaticMeshComponent>("UpperYokeDriven_6");
	upper_yoke_driven_6->SetStaticMesh(upper_yoke_driven_asset.Object);
	upper_yoke_driven_6->SetupAttachment(piston_6);
	upper_yoke_driven_6->SetSimulatePhysics(true);
	upper_yoke_driven_6->SetMobility(EComponentMobility::Movable);
	upper_yoke_driven_6->SetRelativeLocation({0,0,42.6f});
	upper_yoke_driven_6->SetMassOverrideInKg(NAME_None, 1.0f);
	
	yoke_driven_fixed_piston_6 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("YokeDrivenFixedPiston1_6"));
	yoke_driven_fixed_piston_6->SetupAttachment(piston_6);
	yoke_driven_fixed_piston_6->ComponentName1.ComponentName = "Piston_6";
	yoke_driven_fixed_piston_6->ComponentName2.ComponentName = "UpperYokeDriven_6";
	yoke_driven_fixed_piston_6->SetDisableCollision(true);
	yoke_driven_fixed_piston_6->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0);
	yoke_driven_fixed_piston_6->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);
	yoke_driven_fixed_piston_6->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);
	yoke_driven_fixed_piston_6->SetRelativeLocation({0,0,42.6f});
	
	upper_yoke_driver_6 = CreateDefaultSubobject<UStaticMeshComponent>("UpperYokeDriver_6");
	upper_yoke_driver_6->SetStaticMesh(upper_yoke_driver_asset.Object);
	upper_yoke_driver_6->SetupAttachment(upper_yoke_driven_6);
	upper_yoke_driver_6->SetSimulatePhysics(true);
	upper_yoke_driver_6->SetMobility(EComponentMobility::Movable);
	upper_yoke_driver_6->SetRelativeRotation({-1.95f,0, -16.5f});
	upper_yoke_driver_6->SetMassOverrideInKg(NAME_None, 1.0f);

	upper_revolute_1_joint_6 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("UpperRevolute1Joint1_6"));
	upper_revolute_1_joint_6->SetupAttachment(upper_yoke_driven_6);
	upper_revolute_1_joint_6->ComponentName1.ComponentName = "UpperYokeDriven_6";
	upper_revolute_1_joint_6->ComponentName2.ComponentName = "UpperYokeDriver_6";
	upper_revolute_1_joint_6->SetDisableCollision(true);
	upper_revolute_1_joint_6->SetAngularSwing1Limit(_rotation_locked, 0);  // This one must be locked

	upper_spider_6 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UpperSpider_6"));
	upper_spider_6->SetStaticMesh(lower_spider_asset.Object);
	upper_spider_6->SetupAttachment(upper_yoke_driven_6);
	upper_spider_6->SetSimulatePhysics(false);
	upper_spider_6->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	upper_spider_6->SetMobility(EComponentMobility::Movable);
	upper_spider_6->SetRelativeRotation({0, 0, -16.5f});

	upper_revolute_disk_6 = CreateDefaultSubobject<UStaticMeshComponent>("UpperRevoluteDisk_6");
	upper_revolute_disk_6->SetStaticMesh(upper_revolute_disk_asset.Object);
	upper_revolute_disk_6->SetupAttachment(upper_yoke_driver_6);
	upper_revolute_disk_6->SetSimulatePhysics(true);
	upper_revolute_disk_6->SetMobility(EComponentMobility::Movable);
	upper_revolute_disk_6->SetRelativeLocation({0, 0, 2.5f});
	upper_revolute_disk_6->SetMassOverrideInKg(NAME_None, 1.0f);

	upper_revolute_2_joint_6 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("UpperRevolute2Joint1_6"));
	upper_revolute_2_joint_6->SetupAttachment(upper_yoke_driver_6);
	upper_revolute_2_joint_6->ComponentName1.ComponentName = "UpperYokeDriver_6";
	upper_revolute_2_joint_6->ComponentName2.ComponentName = "UpperRevoluteDisk_6";
	upper_revolute_2_joint_6->SetDisableCollision(true);
	upper_revolute_2_joint_6->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);  // This one must be locked
	upper_revolute_2_joint_6->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);  // This one must be locked
	upper_revolute_2_joint_6->SetRelativeLocation({0,0,2.5f});
	// Leg 6 End // // // // // // // // // // // // // // // // // // // // // // // //
	
	// Linking Legs to platform 
	leg1_fixed_frame = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("FixFrameLeg1"));
	leg1_fixed_frame->SetupAttachment(RootComponent);  // Should this rather be linked to RootComponent?
	leg1_fixed_frame->ComponentName1.ComponentName = "FixedFrame";
	leg1_fixed_frame->ComponentName2.ComponentName = "LowerYokeDriven";
	leg1_fixed_frame->SetDisableCollision(true);
	leg1_fixed_frame->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0);
	leg1_fixed_frame->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);
	leg1_fixed_frame->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);

	leg1_dynamic_frame = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("DynamicFrameLeg1"));
	leg1_dynamic_frame->SetupAttachment(upper_revolute_disk_1);
	leg1_dynamic_frame->ComponentName1.ComponentName = "UpperRevoluteDisk";
	leg1_dynamic_frame->ComponentName2.ComponentName = "DynamicFrame";
	leg1_dynamic_frame->SetDisableCollision(true);
	leg1_dynamic_frame->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0);
	leg1_dynamic_frame->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);
	leg1_dynamic_frame->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);
	leg1_dynamic_frame->SetRelativeLocation({0, 0, 2.5f});

	leg2_fixed_frame = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("FixFrameLeg1_2"));
	leg2_fixed_frame->SetupAttachment(RootComponent);  // Should this rather be linked to RootComponent?
	leg2_fixed_frame->ComponentName1.ComponentName = "FixedFrame";
	leg2_fixed_frame->ComponentName2.ComponentName = "LowerYokeDriven_2";
	leg2_fixed_frame->SetDisableCollision(true);
	leg2_fixed_frame->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0);
	leg2_fixed_frame->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);
	leg2_fixed_frame->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);

	leg2_dynamic_frame = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("DynamicFrameLeg1_2"));
	leg2_dynamic_frame->SetupAttachment(upper_revolute_disk_2);
	leg2_dynamic_frame->ComponentName1.ComponentName = "UpperRevoluteDisk_2";
	leg2_dynamic_frame->ComponentName2.ComponentName = "DynamicFrame";
	leg2_dynamic_frame->SetDisableCollision(true);
	leg2_dynamic_frame->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0);
	leg2_dynamic_frame->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);
	leg2_dynamic_frame->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);
	leg2_dynamic_frame->SetRelativeLocation({0, 0, 2.5f});

	leg3_fixed_frame = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("FixFrameLeg1_3"));
	leg3_fixed_frame->SetupAttachment(RootComponent);  // Should this rather be linked to RootComponent?
	leg3_fixed_frame->ComponentName1.ComponentName = "FixedFrame";
	leg3_fixed_frame->ComponentName2.ComponentName = "LowerYokeDriven_3";
	leg3_fixed_frame->SetDisableCollision(true);
	leg3_fixed_frame->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0);
	leg3_fixed_frame->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);
	leg3_fixed_frame->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);

	leg3_dynamic_frame = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("DynamicFrameLeg1_3"));
	leg3_dynamic_frame->SetupAttachment(upper_revolute_disk_3);
	leg3_dynamic_frame->ComponentName1.ComponentName = "UpperRevoluteDisk_3";
	leg3_dynamic_frame->ComponentName2.ComponentName = "DynamicFrame";
	leg3_dynamic_frame->SetDisableCollision(true);
	leg3_dynamic_frame->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0);
	leg3_dynamic_frame->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);
	leg3_dynamic_frame->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);
	leg3_dynamic_frame->SetRelativeLocation({0, 0, 2.5f});

	leg4_fixed_frame = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("FixFrameLeg1_4"));
	leg4_fixed_frame->SetupAttachment(RootComponent);  // Should this rather be linked to RootComponent?
	leg4_fixed_frame->ComponentName1.ComponentName = "FixedFrame";
	leg4_fixed_frame->ComponentName2.ComponentName = "LowerYokeDriven_4";
	leg4_fixed_frame->SetDisableCollision(true);
	leg4_fixed_frame->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0);
	leg4_fixed_frame->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);
	leg4_fixed_frame->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);

	leg4_dynamic_frame = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("DynamicFrameLeg1_4"));
	leg4_dynamic_frame->SetupAttachment(upper_revolute_disk_4);
	leg4_dynamic_frame->ComponentName1.ComponentName = "UpperRevoluteDisk_4";
	leg4_dynamic_frame->ComponentName2.ComponentName = "DynamicFrame";
	leg4_dynamic_frame->SetDisableCollision(true);
	leg4_dynamic_frame->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0);
	leg4_dynamic_frame->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);
	leg4_dynamic_frame->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);
	leg4_dynamic_frame->SetRelativeLocation({0, 0, 2.5f});

	leg5_fixed_frame = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("FixFrameLeg1_5"));
	leg5_fixed_frame->SetupAttachment(RootComponent);  // Should this rather be linked to RootComponent?
	leg5_fixed_frame->ComponentName1.ComponentName = "FixedFrame";
	leg5_fixed_frame->ComponentName2.ComponentName = "LowerYokeDriven_5";
	leg5_fixed_frame->SetDisableCollision(true);
	leg5_fixed_frame->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0);
	leg5_fixed_frame->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);
	leg5_fixed_frame->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);

	leg5_dynamic_frame = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("DynamicFrameLeg1_5"));
	leg5_dynamic_frame->SetupAttachment(upper_revolute_disk_5);
	leg5_dynamic_frame->ComponentName1.ComponentName = "UpperRevoluteDisk_5";
	leg5_dynamic_frame->ComponentName2.ComponentName = "DynamicFrame";
	leg5_dynamic_frame->SetDisableCollision(true);
	leg5_dynamic_frame->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0);
	leg5_dynamic_frame->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);
	leg5_dynamic_frame->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);
	leg5_dynamic_frame->SetRelativeLocation({0, 0, 2.5f});

	leg6_fixed_frame = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("FixFrameLeg1_6"));
	leg6_fixed_frame->SetupAttachment(RootComponent);  // Should this rather be linked to RootComponent?
	leg6_fixed_frame->ComponentName1.ComponentName = "FixedFrame";
	leg6_fixed_frame->ComponentName2.ComponentName = "LowerYokeDriven_6";
	leg6_fixed_frame->SetDisableCollision(true);
	leg6_fixed_frame->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0);
	leg6_fixed_frame->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);
	leg6_fixed_frame->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);

	leg6_dynamic_frame = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("DynamicFrameLeg1_6"));
	leg6_dynamic_frame->SetupAttachment(upper_revolute_disk_6);
	leg6_dynamic_frame->ComponentName1.ComponentName = "UpperRevoluteDisk_6";
	leg6_dynamic_frame->ComponentName2.ComponentName = "DynamicFrame";
	leg6_dynamic_frame->SetDisableCollision(true);
	leg6_dynamic_frame->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0);
	leg6_dynamic_frame->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);
	leg6_dynamic_frame->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);
	leg6_dynamic_frame->SetRelativeLocation({0, 0, 2.5f});
	
}

// Called when the game starts or when spawned
void AStewartPlatform::BeginPlay()
{
	Super::BeginPlay();
	_wave_thread = new Fwave_generator(GetWorld());
	// TSharedPtr<Fwave_generator> someRunnable = MakeShareable<Fwave_generator>(GetWorld());  // TODO Figure out how to do this
	
}

// Called every frame
void AStewartPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// LEG 1
	FRotator lower_spider_orientation = {0, 0, lower_yoke_driver_1->GetRelativeRotation().Roll};
	lower_spider_1->SetRelativeRotation(lower_spider_orientation);
	float calc_upper_Spider_Rotation =  upper_yoke_driver_1->GetRelativeRotation().Roll - upper_yoke_driven_1->GetRelativeRotation().Roll;
	FRotator upper_spider_orientation = {0, 0, calc_upper_Spider_Rotation};
	upper_spider_1->SetRelativeRotation(upper_spider_orientation);

	// LEG 2
	FRotator lower_spider_orientation_2 = {0, 0, lower_yoke_driver_2->GetRelativeRotation().Roll};
	lower_spider_2->SetRelativeRotation(lower_spider_orientation_2);
	float calc_upper_Spider_Rotation_2 =  upper_yoke_driver_2->GetRelativeRotation().Roll - upper_yoke_driven_2->GetRelativeRotation().Roll;
	FRotator upper_spider_orientation_2 = {0, 0, calc_upper_Spider_Rotation_2};
	upper_spider_2->SetRelativeRotation(upper_spider_orientation_2);

	// LEG 3
	FRotator lower_spider_orientation_3 = {0, 0, lower_yoke_driver_3->GetRelativeRotation().Roll};
	lower_spider_3->SetRelativeRotation(lower_spider_orientation_3);
	float calc_upper_Spider_Rotation_3 =  upper_yoke_driver_3->GetRelativeRotation().Roll - upper_yoke_driven_3->GetRelativeRotation().Roll;
	FRotator upper_spider_orientation_3 = {0, 0, calc_upper_Spider_Rotation_3};
	upper_spider_3->SetRelativeRotation(upper_spider_orientation_3);

	// LEG 4
	FRotator lower_spider_orientation_4 = {0, 0, lower_yoke_driver_4->GetRelativeRotation().Roll};
	lower_spider_4->SetRelativeRotation(lower_spider_orientation_4);
	float calc_upper_Spider_Rotation_4 =  upper_yoke_driver_4->GetRelativeRotation().Roll - upper_yoke_driven_4->GetRelativeRotation().Roll;
	FRotator upper_spider_orientation_4 = {0, 0, calc_upper_Spider_Rotation_4};
	upper_spider_4->SetRelativeRotation(upper_spider_orientation_4);

	// LEG 5
	FRotator lower_spider_orientation_5 = {0, 0, lower_yoke_driver_5->GetRelativeRotation().Roll};
	lower_spider_5->SetRelativeRotation(lower_spider_orientation_5);
	float calc_upper_Spider_Rotation_5 =  upper_yoke_driver_5->GetRelativeRotation().Roll - upper_yoke_driven_5->GetRelativeRotation().Roll;
	FRotator upper_spider_orientation_5 = {0, 0, calc_upper_Spider_Rotation_5};
	upper_spider_5->SetRelativeRotation(upper_spider_orientation_5);

	// LEG 6
	FRotator lower_spider_orientation_6 = {0, 0, lower_yoke_driver_6->GetRelativeRotation().Roll};
	lower_spider_6->SetRelativeRotation(lower_spider_orientation_6);
	float calc_upper_Spider_Rotation_6 =  upper_yoke_driver_6->GetRelativeRotation().Roll - upper_yoke_driven_6->GetRelativeRotation().Roll;
	FRotator upper_spider_orientation_6 = {0, 0, calc_upper_Spider_Rotation_6};
	upper_spider_6->SetRelativeRotation(upper_spider_orientation_6);

	FVector position = {0,0,0};
	FRotator rotation = {0,0,0};
	_wave_thread->get_pose(position, rotation);
	position.Z = position.Z + 93.0f + 15.0f;
	rotation.Yaw = rotation.Yaw + 60.0f;
	dynamic_frame->SetRelativeRotation(rotation);
	dynamic_frame->SetRelativeLocation(position);
	
}

void AStewartPlatform::EndPlay(const EEndPlayReason::Type EndPlayReason){
	Super::EndPlay(EndPlayReason);

	if (_wave_thread && _wave_thread->is_running()) {
		_wave_thread->Stop();
		while (_wave_thread->is_running())
		{
			FPlatformProcess::Sleep(0.1);
		}
		delete _wave_thread;
		UE_LOG(LogTemp, Warning, TEXT("Stopped and deleted _wave_thread"));
	}
}