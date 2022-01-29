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
	dynamic_frame->SetRelativeLocation({0, 0, 150.0f});

	// // // Leg 1 start // // //
	lower_yoke_driven_1 = CreateDefaultSubobject<UStaticMeshComponent>("LowerYokeDriven");
	lower_yoke_driven_1->SetStaticMesh(lower_yoke_driven_asset.Object);
	lower_yoke_driven_1->SetupAttachment(fixed_frame);
	lower_yoke_driven_1->SetSimulatePhysics(false);
	lower_yoke_driven_1->SetRelativeLocation({18.5566f,-55.235f,2.5f});

	lower_spider_1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LowerSpider"));
	lower_spider_1->SetStaticMesh(lower_spider_asset.Object);
	lower_spider_1->SetupAttachment(lower_yoke_driven_1);
	lower_spider_1->SetSimulatePhysics(false);
	lower_spider_1->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	lower_spider_1->SetMobility(EComponentMobility::Movable);

	lower_yoke_driver_1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LowerYokeDriver"));
	lower_yoke_driver_1->SetStaticMesh(lower_yoke_driver_asset.Object);
	lower_yoke_driver_1->SetupAttachment(lower_yoke_driven_1);
	lower_yoke_driver_1->SetSimulatePhysics(true);
	lower_yoke_driver_1->SetMobility(EComponentMobility::Movable);
	// lower_yoke_driver->SetRelativeLocation({0, 0, 200});

	// Constraint settings
	constexpr   EAngularConstraintMotion _rotation_locked = ACM_Locked;
	
	// Must be created in constructor
	lower_revolute_1_joint_1 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("RevoluteJoint"));
	lower_revolute_1_joint_1->SetupAttachment(lower_yoke_driven_1);
	lower_revolute_1_joint_1->ComponentName1.ComponentName = "LowerYokeDriven";
	lower_revolute_1_joint_1->ComponentName2.ComponentName = "LowerYokeDriver";
	lower_revolute_1_joint_1->SetDisableCollision(true);
	lower_revolute_1_joint_1->SetAngularSwing1Limit(_rotation_locked, 0);  // This one must be locked
	// lower_revolute_1_joint->SetAngularSwing2Limit(_rotation_locked, 0);  // Red
	// lower_revolute_1_joint->SetAngularTwistLimit(_rotation_locked, 0);  // Green

	
	cylinder_1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cylinder"));
	cylinder_1->SetStaticMesh(cylinder_asset.Object);
	cylinder_1->SetupAttachment(lower_yoke_driver_1);
	cylinder_1->SetSimulatePhysics(true);
	cylinder_1->SetMobility(EComponentMobility::Movable);
	cylinder_1->SetRelativeLocation({0,0,40});
		
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
	piston_1->SetRelativeLocation({0,0,40.0f});

	cylinder_prismatic_piston_1 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PrismaticCylinderPiston"));
	cylinder_prismatic_piston_1->SetupAttachment(cylinder_1);
	cylinder_prismatic_piston_1->ComponentName1.ComponentName = "Cylinder";
	cylinder_prismatic_piston_1->ComponentName2.ComponentName = "Piston";
	cylinder_prismatic_piston_1->SetDisableCollision(true);
	cylinder_prismatic_piston_1->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0);
	cylinder_prismatic_piston_1->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);
	cylinder_prismatic_piston_1->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);
	cylinder_prismatic_piston_1->SetLinearZLimit(ELinearConstraintMotion::LCM_Limited, 35.0f);
	// cylinder_prismatic_piston->SetLinearZLimit(ELinearConstraintMotion::LCM_Free, 0.0f);

	upper_yoke_driven_1 = CreateDefaultSubobject<UStaticMeshComponent>("UpperYokeDriven");
	upper_yoke_driven_1->SetStaticMesh(upper_yoke_driven_asset.Object);
	upper_yoke_driven_1->SetupAttachment(piston_1);
	upper_yoke_driven_1->SetSimulatePhysics(true);
	upper_yoke_driven_1->SetMobility(EComponentMobility::Movable);
	upper_yoke_driven_1->SetRelativeLocation({0,0,42.5f});
	
	yoke_driven_fixed_piston_1 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("YokeDrivenFixedPiston1"));
	yoke_driven_fixed_piston_1->SetupAttachment(piston_1);
	yoke_driven_fixed_piston_1->ComponentName1.ComponentName = "Piston";
	yoke_driven_fixed_piston_1->ComponentName2.ComponentName = "UpperYokeDriven";
	yoke_driven_fixed_piston_1->SetDisableCollision(true);
	yoke_driven_fixed_piston_1->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0);
	yoke_driven_fixed_piston_1->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);
	yoke_driven_fixed_piston_1->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);
	yoke_driven_fixed_piston_1->SetRelativeLocation({0,0,42.5f});
	
	upper_yoke_driver_1 = CreateDefaultSubobject<UStaticMeshComponent>("UpperYokeDriver");
	upper_yoke_driver_1->SetStaticMesh(upper_yoke_driver_asset.Object);
	upper_yoke_driver_1->SetupAttachment(upper_yoke_driven_1);
	upper_yoke_driver_1->SetSimulatePhysics(true);
	upper_yoke_driver_1->SetMobility(EComponentMobility::Movable);

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

	upper_revolute_disk_1 = CreateDefaultSubobject<UStaticMeshComponent>("UpperRevoluteDisk");
	upper_revolute_disk_1->SetStaticMesh(upper_revolute_disk_asset.Object);
	upper_revolute_disk_1->SetupAttachment(upper_yoke_driven_1);
	upper_revolute_disk_1->SetSimulatePhysics(true);
	upper_revolute_disk_1->SetMobility(EComponentMobility::Movable);
	upper_revolute_disk_1->SetRelativeLocation({0, 0, 2.0f});

	upper_revolute_2_joint_1 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("UpperRevolute2Joint1"));
	upper_revolute_2_joint_1->SetupAttachment(upper_yoke_driver_1);
	upper_revolute_2_joint_1->ComponentName1.ComponentName = "UpperYokeDriver";
	upper_revolute_2_joint_1->ComponentName2.ComponentName = "UpperRevoluteDisk";
	upper_revolute_2_joint_1->SetDisableCollision(true);
	upper_revolute_2_joint_1->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);  // This one must be locked
	upper_revolute_2_joint_1->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);  // This one must be locked
	upper_revolute_2_joint_1->SetRelativeLocation({0,0,2.0f});
	// Leg 1 End //
	
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
	leg1_dynamic_frame->SetRelativeLocation({0, 0, 3.5f});
	
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

	FRotator lower_spider_orientation = {0.0f, 0.0f, lower_yoke_driver_1->GetRelativeRotation().Roll};
	lower_spider_1->SetRelativeRotation(lower_spider_orientation);

	FRotator upper_spider_orientation = {0.0f, 0.0f, upper_yoke_driver_1->GetRelativeRotation().Roll};
	upper_spider_1->SetRelativeRotation(upper_spider_orientation);
	
	float realtimeSeconds = GetWorld()->GetRealTimeSeconds(); // https://answers.unrealengine.com/questions/167413/elapsed-milliseconds-since-start-of-level.html
	float movement = 15.0f*sin(realtimeSeconds);
	dynamic_frame->SetRelativeLocation({movement/1.0f,movement/1.0f,movement + 140.0f});

}

