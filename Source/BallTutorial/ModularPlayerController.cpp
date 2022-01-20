// Fill out your copyright notice in the Description page of Project Settings.

#include "ModularPlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "HealthComponent.h"
#include "AdventureGameInstance.h"
#include "MyUtil.h"
#include "Move.h"
#include "Jump.h"
#include "SpawnBall.h"
#include "Soul.h"

AModularPlayerController::AModularPlayerController()
{
}

void AModularPlayerController::BeginPlay()
{
    Super::BeginPlay();
    if(ActorToSpawn != nullptr){
        if(IsLocalPlayerController()){
            FVector SpawnPoint = FVector(0.0f,3150.0f,272.0f);
            FRotator SpawnPointRotation = FRotator(0.0f,0.0f,0.0f);

            if(SpawnMarkerClass != nullptr){
                TArray<AActor*> OutActors;
                UGameplayStatics::GetAllActorsOfClass(GetWorld(), SpawnMarkerClass, OutActors);
                if(OutActors.Num() > 0){
                    AActor * SpawnMarker = OutActors[0];
                    SpawnPoint = SpawnMarker->GetActorLocation();
                    SpawnPointRotation = SpawnMarker->GetActorRotation();
                }
            }

            ASoul* InSoul = GetWorld()->SpawnActor<ASoul>(ActorToSpawn, SpawnPoint, SpawnPointRotation);
            Possessed(InSoul);
        }
    } else {
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Actor class to spawn not set")));
    }
}

void AModularPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
    Marker = InPawn;
    MarkerBody = Marker->FindComponentByClass<UStaticMeshComponent>();
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Green, FString::Printf(TEXT("I spawned: %s %s"), *Marker->GetName(), Marker->HasAuthority() ? TEXT("SERVER") : TEXT("CLIENT")));

}

void AModularPlayerController::AcknowledgePossession(APawn* InPawn)
{
    Super::AcknowledgePossession(InPawn);
    Marker = InPawn;
    MarkerBody = Marker->FindComponentByClass<UStaticMeshComponent>();
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Green, FString::Printf(TEXT("I spawned: %s %s"), *Marker->GetName(), Marker->HasAuthority() ? TEXT("SERVER") : TEXT("CLIENT")));

}

void AModularPlayerController::Possessed(ASoul* InSoul) {
    if (InSoul != nullptr){
        MovementComp = InSoul->FindComponentByClass<UMove>();
        JumpComp = InSoul->FindComponentByClass<UJump>();
        SpringComp = InSoul->FindComponentByClass<USpringArmComponent>();
        SpawnBallComp = InSoul->FindComponentByClass<USpawnBall>();
        Body = InSoul->FindComponentByClass<UStaticMeshComponent>();
        if(Soul != nullptr){
            Soul->PlayerController = nullptr;
        }
        Soul = InSoul;
        Soul->PlayerController = this;
        Actor = Cast<AActor>(InSoul);
        Primitive = Cast<UPrimitiveComponent>(Body);
        SetViewTarget(Actor);
        if (FirstPossession){
            UAdventureGameInstance * Game = Cast<UAdventureGameInstance>(GetGameInstance());
            if (Game != nullptr){
                UAdventureSaveGame * Save = Game->AdventureSave;
                if (Save != nullptr){
                    if(!Save->newGame){
                        Actor->SetActorLocation(Save->PlayerLocation);
                        Checkpoint = Save->PlayerCheckpoint;
                    }
                }
            }
        }
        FirstPossession = false;
    }
}

// Called to bind functionality to input
void AModularPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Respond every frame to the values of our two movement axes, "MoveX" and "MoveY".
	InputComponent->BindAxis("Right", this, &AModularPlayerController::moveRight);
	InputComponent->BindAxis("Left", this, &AModularPlayerController::moveLeft);
	InputComponent->BindAxis("Forward", this, &AModularPlayerController::moveForward);
	InputComponent->BindAxis("Back", this, &AModularPlayerController::moveBack);
	InputComponent->BindAxis("RotateRight", this, &AModularPlayerController::rotateRight);
	InputComponent->BindAxis("RotateLeft", this, &AModularPlayerController::rotateLeft);
	InputComponent->BindAxis("Slow", this, &AModularPlayerController::slow);
	InputComponent->BindAxis("Boost", this, &AModularPlayerController::boost);
	InputComponent->BindAxis("Attack", this, &AModularPlayerController::attack);
	InputComponent->BindAxis("Act", this, &AModularPlayerController::act);
	InputComponent->BindAction("Jump", IE_Pressed, this, &AModularPlayerController::JumpPressed);
	InputComponent->BindAction("Jump", IE_Released, this, &AModularPlayerController::JumpReleased);
	InputComponent->BindAction("Menu", IE_Pressed, this, &AModularPlayerController::menu);
	InputComponent->BindAction("Possess", IE_Pressed, this, &AModularPlayerController::possess);
	InputComponent->BindAction("GetYouOne", IE_Pressed, this, &AModularPlayerController::GetYouOne);
}

void AModularPlayerController::moveRight(float AxisValue) {
    Right = AxisValue;
}

void AModularPlayerController::moveLeft(float AxisValue) {
    Left = AxisValue;
}

void AModularPlayerController::moveForward(float AxisValue) {
    Forward = AxisValue;
}

void AModularPlayerController::moveBack(float AxisValue) {
    Back = AxisValue;
}

void AModularPlayerController::rotateRight(float AxisValue) {
    RotateRight = AxisValue;
}

void AModularPlayerController::rotateLeft(float AxisValue) {
    RotateLeft = AxisValue;
}

void AModularPlayerController::JumpPressed() {
    Jump = true;
}
void AModularPlayerController::JumpReleased() {
    Jump = false;
}

void AModularPlayerController::menu() {
    UAdventureGameInstance * Game = Cast<UAdventureGameInstance>(GetGameInstance());
    UAdventureSaveGame * Save = Game->AdventureSave;
    Save->newGame = false;
    if(Actor){
        Save->PlayerLocation = Actor->GetActorLocation();
        Save->PlayerCheckpoint =  Checkpoint;
    }
    UGameplayStatics::SaveGameToSlot(Save, Game->AdventureSlot, 0);
}
void AModularPlayerController::possess() {
    FVector Start = Actor->GetActorLocation() - FVector(0.0f,0.0f,50.0f);
    FVector End = Actor->GetActorLocation() + FVector(0.0f,0.0f,50.0f);
    TArray<FHitResult> OutHits;
    bool ignoreSelf = true;
    float DrawTime = 5.0f;
    FLinearColor TraceColor = FLinearColor::Red;
    FLinearColor TraceHitColor = FLinearColor::Green;
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(Actor);
    bool bTraceComplex = true;
    TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes;
    ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);
    ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery2);
    ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery3);
    ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery4);
    bool hit = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), Start, End, 100.0f, ObjectTypes, bTraceComplex, ActorsToIgnore, EDrawDebugTrace::None, OutHits, ignoreSelf, TraceColor, TraceHitColor, DrawTime);
    for (FHitResult OutHit : OutHits) {
        AActor * OtherActor = OutHit.GetActor();
        ASoul * PossessMe = Cast<ASoul>(OtherActor);
        if (PossessMe != nullptr){
            TryPossess(PossessMe);
        }
    }
}
void AModularPlayerController::TryPossess(ASoul * PossessMe) {
    Possessed(PossessMe);
}
void AModularPlayerController::GetYouOne() {
    if(ActorToSpawn != nullptr){
            FVector SpawnLocation = Actor->GetActorLocation() + FVector(200.0f, 0.0f, 10.0f);
            FRotator SpawnRotation = Actor->GetActorRotation();
            AActor* One = Actor->GetWorld()->SpawnActor<AActor>(ActorToSpawn, SpawnLocation, SpawnRotation);
    } else {
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Actor class to spawn not set")));

    }
}

void AModularPlayerController::attack(float AxisValue) {
}
void AModularPlayerController::act(float AxisValue) {

    Act = AxisValue;
}

void AModularPlayerController::boost(float AxisValue) {
    if (AxisValue > 0.1){
        Boost = true;
    }else {
        Boost = false;
    }
}

void AModularPlayerController::slow(float AxisValue) {
    if (AxisValue > 0.1){
        Slow = true;
    }else {
        Slow = false;
    }
}

void AModularPlayerController::ToCheckpoint() {
    if(Primitive){
        Primitive->SetPhysicsLinearVelocity(FVector(0,0,0));
        Primitive->SetPhysicsAngularVelocityInDegrees(FVector(0,0,0));
    }
    Soul->SetActorLocation(Checkpoint);
}

void AModularPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (SpringComp != nullptr){
        FRotator SpringRotation = SpringComp->GetComponentRotation();
        FVector VerticalAxis = { 0,0,1 };
        if (MovementComp != nullptr){
            float Side = Right + Left;
            float Ahead = Forward + Back;
            FVector RotatedUnitDirection;
            if (!(Side == 0 && Ahead == 0) && DeltaTime < 0.3f){
                FVector Direction = FVector(Ahead, Side, 0);
                FVector Nowhere = { 0,0,0 };
                FVector RotatedDirection = 
                    UKismetMathLibrary::RotateAngleAxis(Direction, 
                            SpringRotation.Yaw, 
                            VerticalAxis);
                RotatedUnitDirection = 
                    UKismetMathLibrary::GetDirectionUnitVector(Nowhere, 
                            RotatedDirection);
            } else {
                RotatedUnitDirection = FVector(0,0,0);
            }
            MovementComp->Move(RotatedUnitDirection,DeltaTime,Boost,Slow);
        }
        if (SpawnBallComp != nullptr && Act != 0){
            FVector RotatedUnitForwardDirection = 
                UKismetMathLibrary::RotateAngleAxis(FVector(1.0f,0.0f,0.0f), 
                        SpringRotation.Yaw, 
                        VerticalAxis);
            SpawnBallComp->Spawn(RotatedUnitForwardDirection);
        }
        float Rotate = RotateRight + RotateLeft;
        if( Rotate != 0){
            FRotator RotationChange = FRotator(0.0f, 0.0f, 0.0f);
            RotationChange.Yaw = Rotate * CameraTurnSpeed;
            SpringComp->AddWorldRotation(RotationChange);
        }
    }
    if (JumpComp != nullptr && Jump){
        JumpComp->Jump();
    }
}

bool AModularPlayerController::ServerHurt_Validate(AActor* toHurt, float pain) {
    return true;
}

void AModularPlayerController::ServerHurt_Implementation(AActor* toHurt, float pain) {
    UHealthComponent * health = toHurt->FindComponentByClass<UHealthComponent>();
    if (health != nullptr){
        health->Suffer(pain);
    }
}

