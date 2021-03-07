#include "RollingBall.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "ConstructorHelpers.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Physics/PhysicsInterfaceCore.h"
#include "Components/PrimitiveComponent.h"
#include "Math/Vector.h"
#include "Kismet/KismetMathLibrary.h"
#include "Materials/Material.h"
#include "Logging/MessageLog.h"

ARollingBall::ARollingBall()
{
	//Basic setup of components
	//The actual material and mesh of our ball are set in the blueprint child class to avoid hard coded paths.
	base = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball"));
	RootComponent = base;
	//Adjust the position of our rolling ball so it isn't halway in the ground when placed into level.
	base->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	base->SetSimulatePhysics(true);

	spring = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring"));
	//Prevents the spring from rolling with our ball
	spring->SetAbsolute(false, true, false);
	spring->SetRelativeRotation(FRotator(-30.0f, 0.0f, 0.0f));
	spring->TargetArmLength = 500.0f;
	spring->SetupAttachment(base);

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(spring);
	camera->SetRelativeRotation(FRotator(15.0f, 0.0f, 0.0f));
}

// Called when the game starts or when spawned
void ARollingBall::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARollingBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Directional push
    FRotator springRotation = spring->GetComponentRotation();
	float side = m_right + m_left;
	float ahead = m_forward + m_back;
	FVector direction = { ahead, side, 0 };
	FVector nowhere = { 0,0,0 };
	FVector verticalAxis = { 0,0,1 };
	FVector unitDirection = UKismetMathLibrary::GetDirectionUnitVector(nowhere, direction);
	float pushScale = 3000.0;
	FVector rotatedUnitDirection = UKismetMathLibrary::RotateAngleAxis(unitDirection, springRotation.Yaw, verticalAxis);
	base->AddForce(DeltaTime * rotatedUnitDirection * pushScale * base->GetMass());

	//Rotational torque for movement
	float swap = rotatedUnitDirection.Y;
	rotatedUnitDirection.Y = rotatedUnitDirection.X;
	rotatedUnitDirection.X = swap * (-1);
	float torqueScale = 100000000.0;
	base->AddTorqueInRadians(DeltaTime * rotatedUnitDirection * torqueScale);

	//Rotation setting for camera angle
	float rotation = m_rotateRight + m_rotateLeft;
	FRotator rotationChange = FRotator(0.0f, 0.0f, 0.0f);
	float rotationScale = 3.0*60*DeltaTime;
	rotationChange.Yaw = rotation * rotationScale;
	USceneComponent * springComponent = CastChecked<USceneComponent>(spring);
	springComponent->AddWorldRotation(rotationChange);
}

//These are called by our player controller
void ARollingBall::moveRight(float AxisValue) {
	m_right = AxisValue;
}
void ARollingBall::moveLeft(float AxisValue) {
	m_left = AxisValue;
}
void ARollingBall::moveForward(float AxisValue) {
	m_forward = AxisValue;
}
void ARollingBall::moveBack(float AxisValue) {
	m_back = AxisValue;
}
void ARollingBall::rotateRight(float AxisValue) {
	m_rotateRight = AxisValue;
}
void ARollingBall::rotateLeft(float AxisValue) {
	m_rotateLeft = AxisValue;
}
