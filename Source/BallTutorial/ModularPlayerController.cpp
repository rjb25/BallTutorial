// Fill out your copyright notice in the Description page of Project Settings.

#include "ModularPlayerController.h"
#include "Kismet/GameplayStatics.h"

AModularPlayerController::AModularPlayerController()
{
    Right = 0.0f;
    Left = 0.0f;
    Forward = 0.0f;
    Back = 0.0f;
    Movement = nullptr;
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
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("ResponseChannel")));
    Movement = InPawn->FindComponentByClass<UMove>();
    if (Movement == nullptr) {
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("OOPS")));
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
	InputComponent->BindAction("Jump", IE_Pressed, this, &AModularPlayerController::jump);
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

void AModularPlayerController::jump() {
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
    if (Movement != nullptr){
        float Side = Right + Left;
        float Ahead = Forward + Back;
        if (!(Side == 0 && Ahead == 0)){
            FVector Direction = { Ahead, Side, 0 };
            Movement->Move(Direction,DeltaTime);
        }
        /*
        float Rotate = RotateRight + RotateLeft;
        if(Rotate != 0){
            FRotator RotationChange = FRotator(0.0f, 0.0f, 0.0f);
            float RotationScale = 3.0 * 60 * DeltaTime;
            rotationChange.Yaw = rotation * rotationScale;
            USceneComponent * springComponent = CastChecked<USceneComponent>(spring);
            springComponent->AddWorldRotation(rotationChange);
        }

        //Directional push
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
        */
    }
    else {

        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("LOST OOPSY")));
    }
}
