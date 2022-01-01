#include "RollingBall.h"
#include "AdvancedFriendsGameInstance.h"
#include "AdventureGameInstance.h"
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
#include "Kismet/KismetSystemLibrary.h"
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

    //Multiplayer settings
    bReplicates = false;
    bReplicateMovement = false;
    bAlwaysRelevant = true;
    m_timeout = -1.0f;
}

// Called when the game starts or when spawned
void ARollingBall::BeginPlay()
{
    Super::BeginPlay();
    if (!IsLocallyControlled()) {
        base->SetSimulatePhysics(false);
        base->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
    } else {
        UAdventureGameInstance * game = Cast<UAdventureGameInstance>(GetGameInstance());
        if (game != nullptr){
            UAdventureSaveGame * save = game->AdventureSave;
            if (save != nullptr){
                if(!save->newGame){
                    this->SetActorLocation(save->PlayerLocation);
                    m_checkpoint = save->PlayerCheckpoint;
                    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("INSTANCEVALIDFULL")));
                }
            }
        }
    }
}

void ARollingBall::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    //Send our position to server which will then send our position to other clients
    if (HasAuthority()) {
        ClientSetPosition(GetActorTransform());
    }
    else if (!HasAuthority() && IsLocallyControlled()) {
        ServerSetPosition(GetActorTransform());
    }

    if (IsLocallyControlled()) {
        //Directional push
        if (m_timeout < 0.0f){
            FRotator springRotation = spring->GetComponentRotation();
            float mod = 1;
            bool boosting = m_boost > 0.0f;
            bool slowing = m_slow > 0.0f;
            bool grounded = (boosting || slowing) && jumpCheck();
            if (grounded && boosting){
                mod += m_boost+0.5;
            }
            float side = m_right + m_left;
            float ahead = m_forward + m_back;
            FVector direction = { ahead, side, 0 };
            FVector nowhere = { 0,0,0 };
            FVector verticalAxis = { 0,0,1 };
            FVector unitDirection = UKismetMathLibrary::GetDirectionUnitVector(nowhere, direction);
            float pushScale = 3000.0;
            FVector rotatedUnitDirection = UKismetMathLibrary::RotateAngleAxis(unitDirection, springRotation.Yaw, verticalAxis);
            base->AddForce(DeltaTime * rotatedUnitDirection * mod * pushScale * base->GetMass());

            //Rotational torque for movement
            float swap = rotatedUnitDirection.Y;
            rotatedUnitDirection.Y = rotatedUnitDirection.X;
            rotatedUnitDirection.X = swap * (-1);
            float torqueScale = 1000000.0;
            base->AddTorqueInRadians(DeltaTime * rotatedUnitDirection * mod * torqueScale * base->GetMass());
            if(grounded && slowing){
                FVector slowed = -1 * this->GetVelocity() * m_slow * pushScale * 0.01;
                base->AddForce(DeltaTime * slowed * base->GetMass());
                FVector slowedAngular = -1 * base->GetPhysicsAngularVelocity() * m_slow * torqueScale * 0.01;
                base->AddTorqueInRadians(DeltaTime * slowedAngular * base->GetMass());
            }
        } else {
            m_timeout -= DeltaTime;
        }

        //Rotation setting for camera angle
        float rotation = m_rotateRight + m_rotateLeft;
        FRotator rotationChange = FRotator(0.0f, 0.0f, 0.0f);
        float rotationScale = 3.0 * 60 * DeltaTime;
        rotationChange.Yaw = rotation * rotationScale;
        USceneComponent * springComponent = CastChecked<USceneComponent>(spring);
        springComponent->AddWorldRotation(rotationChange);
    }
}

void ARollingBall::timeout(float duration) {
    m_timeout = duration;
}

void ARollingBall::checkpoint() {
    Cast<UPrimitiveComponent>(base)->SetPhysicsLinearVelocity(FVector(0,0,0));
    Cast<UPrimitiveComponent>(base)->SetPhysicsAngularVelocityInDegrees(FVector(0,0,0));
    this->SetActorLocation(m_checkpoint);
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

void ARollingBall::slow(float AxisValue) {
    m_slow = AxisValue;
}

void ARollingBall::boost(float AxisValue) {
    m_boost = AxisValue;
}

void ARollingBall::jump() {
    bool check = jumpCheck();
    if(check){
        float jumpForce = 30000.0;
        base->AddForce(FVector(0.0f,0.0f,1.0f) * jumpForce * base->GetMass());
    }
}
bool ARollingBall::ServerSetPosition_Validate(FTransform position) {
    return true;
}
void ARollingBall::ServerSetPosition_Implementation(FTransform position) {
    SetActorTransform(position);
}
bool ARollingBall::ClientSetPosition_Validate(FTransform position) {
    return true;
}
void ARollingBall::ClientSetPosition_Implementation(FTransform position) {
    if (!IsLocallyControlled()) {
        SetActorTransform(position);
    }
}
bool ARollingBall::jumpCheck_Implementation() {
    FVector Start = GetActorLocation();
    FVector End = Start - FVector(0.0f,0.0f,m_gripDepth+0.001f);
    FHitResult OutHit;
    bool ignoreSelf = true;
    float DrawTime = 5.0f;
    FLinearColor TraceColor = FLinearColor::Red;
    FLinearColor TraceHitColor = FLinearColor::Green;
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(this);
    bool bTraceComplex = true;
    TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes;
    ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);
    ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery2);
    bool hit = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), Start, End, m_gripWidth, ObjectTypes, bTraceComplex, ActorsToIgnore, EDrawDebugTrace::None, OutHit, ignoreSelf, TraceColor, TraceHitColor, DrawTime);

    return hit;
}
