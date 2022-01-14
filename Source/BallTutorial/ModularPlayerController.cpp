// Fill out your copyright notice in the Description page of Project Settings.

#include "ModularPlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "HealthComponent.h"
#include "MyUtil.h"

AModularPlayerController::AModularPlayerController()
{
    Right = 0.0f;
    Left = 0.0f;
    Forward = 0.0f;
    Back = 0.0f;
    MovementComp = nullptr;
    JumpComp = nullptr;
    SpringComp = nullptr;
    CameraTurnSpeed = 1.0f;
}

void AModularPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
    Possessed(InPawn);

}

void AModularPlayerController::AcknowledgePossession(APawn* InPawn)
{
    Super::AcknowledgePossession(InPawn);
    Possessed(InPawn);

}

void AModularPlayerController::Possessed(APawn* InPawn) {
    MovementComp = InPawn->FindComponentByClass<UMove>();
    JumpComp = InPawn->FindComponentByClass<UJump>();
    SpringComp = InPawn->FindComponentByClass<USpringArmComponent>();
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
    /*
    UAdventureGameInstance * game = Cast<UAdventureGameInstance>(GetGameInstance());
    UAdventureSaveGame * save = game->AdventureSave;
    save->newGame = false;
	AModularBall * rollingBall = Cast<AModularBall>(GetPawn());
    if(rollingBall != nullptr){
        save->PlayerLocation = rollingBall->GetActorLocation();
        save->PlayerCheckpoint =  rollingBall->m_checkpoint;
    }
    UGameplayStatics::SaveGameToSlot(save, game->AdventureSlot, 0);
    */
}

void AModularPlayerController::slow(float AxisValue) {
}

void AModularPlayerController::attack(float AxisValue) {
}
void AModularPlayerController::act(float AxisValue) {
}

void AModularPlayerController::boost(float AxisValue) {
}


void AModularPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (SpringComp != nullptr && MovementComp != nullptr && DeltaTime < 0.3f){
        float Side = Right + Left;
        float Ahead = Forward + Back;
        if (!(Side == 0 && Ahead == 0)){
            FVector Direction = { Ahead, Side, 0 };
            FVector Nowhere = { 0,0,0 };
            FVector UnitDirection = UKismetMathLibrary::GetDirectionUnitVector(Nowhere, Direction);
            FVector VerticalAxis = { 0,0,1 };
            FRotator SpringRotation = SpringComp->GetComponentRotation();
            FVector RotatedUnitDirection = UKismetMathLibrary::RotateAngleAxis(UnitDirection, SpringRotation.Yaw, VerticalAxis);
            MovementComp->Move(RotatedUnitDirection,DeltaTime);
        }
    }
    else {

        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("MOVEMENT NULL")));
    }
        if (JumpComp != nullptr && Jump){
            JumpComp->Jump();
        }
        float Rotate = RotateRight + RotateLeft;
        if(SpringComp != nullptr && Rotate != 0){
            FRotator RotationChange = FRotator(0.0f, 0.0f, 0.0f);
            RotationChange.Yaw = Rotate * CameraTurnSpeed;
            SpringComp->AddWorldRotation(RotationChange);
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
