// Fill out your copyright notice in the Description page of Project Settings.

#include "ModularPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "HealthComponent.h"
#include "AdventureGameInstance.h"
#include "MyUtil.h"
#include "Move.h"
#include "Jump.h"
#include "Joint.h"
#include "Ability.h"
#include "HealthComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"

AModularPlayerController::AModularPlayerController()
{
}

void AModularPlayerController::BeginPlay()
{
    Super::BeginPlay();
}

void AModularPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
    Possessed(InPawn);
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Green, FString::Printf(TEXT("I spawned: %s %s"), *InPawn->GetName(), InPawn->HasAuthority() ? TEXT("SERVER") : TEXT("CLIENT")));

}

void AModularPlayerController::AcknowledgePossession(APawn* InPawn)
{
    Super::AcknowledgePossession(InPawn);
    Possessed(InPawn);
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Green, FString::Printf(TEXT("I spawned: %s %s"), *InPawn->GetName(), InPawn->HasAuthority() ? TEXT("SERVER") : TEXT("CLIENT")));

}

void AModularPlayerController::Possessed(APawn * InPawn) {
    if (InPawn != nullptr){
        Pawn = InPawn;
        Actor = Cast<AActor>(InPawn);
        Character = Cast<ACharacter>(Pawn);
        MovementComp = InPawn->FindComponentByClass<UMove>();
        JumpComp = InPawn->FindComponentByClass<UJump>();
        HealthComp = InPawn->FindComponentByClass<UHealthComponent>();
        JointComp = InPawn->FindComponentByClass<UJoint>();
        CameraComp = InPawn->FindComponentByClass<UCameraComponent>();
        InPawn->GetComponents<UAbility>(AbilityComps);
        Body = InPawn->FindComponentByClass<UStaticMeshComponent>();
        if(HealthComp != nullptr){
            HealthComp->Teams.Add(0);
        }
        Primitive = Cast<UPrimitiveComponent>(Body);
        SetViewTarget(Actor);
        if (FirstPossession && HasAuthority()){
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
	InputComponent->BindAxis("Right", this, &AModularPlayerController::RightAxis);
	InputComponent->BindAxis("Forward", this, &AModularPlayerController::ForwardAxis);
	InputComponent->BindAxis("Rotate", this, &AModularPlayerController::RotateAxis);
	InputComponent->BindAxis("Nod", this, &AModularPlayerController::NodAxis);
	InputComponent->BindAction("Ability", IE_Pressed, this, &AModularPlayerController::AbilityPressed);
	InputComponent->BindAction("Ability", IE_Released, this, &AModularPlayerController::AbilityReleased);
	InputComponent->BindAction("Slow", IE_Pressed, this, &AModularPlayerController::SlowPressed);
	InputComponent->BindAction("Slow", IE_Released, this, &AModularPlayerController::SlowReleased);
	InputComponent->BindAction("Boost", IE_Pressed, this, &AModularPlayerController::BoostPressed);
	InputComponent->BindAction("Boost", IE_Released, this, &AModularPlayerController::BoostReleased);
	InputComponent->BindAction("Jump", IE_Pressed, this, &AModularPlayerController::JumpPressed);
	InputComponent->BindAction("Jump", IE_Released, this, &AModularPlayerController::JumpReleased);
	InputComponent->BindAction("Menu", IE_Pressed, this, &AModularPlayerController::menu);
	//InputComponent->BindAction("Possess", IE_Pressed, this, &AModularPlayerController::possess);
}

void AModularPlayerController::RightAxis(float AxisValue) {
    Right = FMath::Clamp(AxisValue,-1.0f,1.0f);
}

void AModularPlayerController::ForwardAxis(float AxisValue) {
    Forward = FMath::Clamp(AxisValue,-1.0f,1.0f);
}

void AModularPlayerController::NodAxis(float AxisValue) {
    float Nod = FMath::Clamp(AxisValue,-1.0f,1.0f);
    AddPitchInput(Nod);
}

void AModularPlayerController::RotateAxis(float AxisValue) {
    float Rotate = FMath::Clamp(AxisValue,-1.0f,1.0f);
    AddYawInput(Rotate);
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
        Save->PlayerCheckpoint = Checkpoint;
    }
    UGameplayStatics::SaveGameToSlot(Save, Game->AdventureSlot, 0);
}

/*
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
*/

/*
void AModularPlayerController::TryPossess(ASoul * PossessMe) {
    Possessed(PossessMe);
}
*/

void AModularPlayerController::AbilityPressed() {
    Ability = true;

}
void AModularPlayerController::AbilityReleased() {
    Ability = false;
	SetInputMode(FInputModeGameOnly());
}

void AModularPlayerController::BoostPressed() {
        Boost = true;
}
void AModularPlayerController::BoostReleased() {
        Boost = false;
}

void AModularPlayerController::SlowPressed() {
        Slow = true;
}
void AModularPlayerController::SlowReleased() {
        Slow = false;
}

void AModularPlayerController::ToCheckpoint() {
    if(Primitive){
        Primitive->SetPhysicsLinearVelocity(FVector(0,0,0));
        Primitive->SetPhysicsAngularVelocityInDegrees(FVector(0,0,0));
    }
    Actor->SetActorLocation(Checkpoint);
}

void AModularPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FRotator Rotation = FRotator(0.0f,0.0f,0.0f);
    Rotation.Yaw = GetControlRotation().Yaw;
    if(Pawn != nullptr){
        Pawn->AddMovementInput(UKismetMathLibrary::GetForwardVector(Rotation), Forward);
        Pawn->AddMovementInput(UKismetMathLibrary::GetRightVector(Rotation), Right);
    }

    if (AbilityComps.Num() > 0 && Ability){
        FVector RotatedUnitForwardDirection = 
            UKismetMathLibrary::RotateAngleAxis(FVector(1.0f,0.0f,0.0f), 
                    Actor->GetActorRotation().Yaw,
                    FVector(0.0f,0.0f,1.0f));
        for (UAbility * AbilityComp : AbilityComps){
            AbilityComp->Use(RotatedUnitForwardDirection);
        }
    }


    if (Jump){
       Character->Jump(); 
    }
}

bool AModularPlayerController::ServerHurt_Validate(AActor* toHurt, float pain, TSubclassOf<UDamageType> DamageType) {
    return true;
}

void AModularPlayerController::ServerHurt_Implementation(AActor* toHurt, float pain, TSubclassOf<UDamageType> DamageType) {
    UHealthComponent * health = toHurt->FindComponentByClass<UHealthComponent>();
    if (health != nullptr){
        UGameplayStatics::ApplyDamage(toHurt, pain, this, Actor, DamageType);
    }
}

