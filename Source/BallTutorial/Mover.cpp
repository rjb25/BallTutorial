// Fill out your copyright notice in the Description page of Project Settings.

#include "Mover.h"
#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UMover::UMover()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;
    TimeTotal = 0.0f;

    // ...
}


// Called when the game starts
void UMover::BeginPlay()
{
    Super::BeginPlay();
    Owner = GetOwner();
    Body = Owner->FindComponentByClass<UStaticMeshComponent>();
    if (Body != nullptr){
        Body->SetMobility(EComponentMobility::Type::Movable);
    } else {
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("No static mesh for mover")));
    }
    StateSet *LocationSet;
    LocationSet->States = &LocationChanges;
    LocationSet.StateIndex = LocationChangesStartIndex;
    LocationSet.From = GetOwner()->GetActorLocation();
    StateSets.Add(LocationSet);

    StateSet RotationSet;
    RotationSet.States = RotationChanges;
    RotationSet.StateIndex = RotationChangesStartIndex;
    RotationSet.From = GetOwner()->GetActorRotation().Vector();
    StateSets.Add(RotationSet);

    StateSet SizeSet;
    SizeSet.States = SizeChanges;
    SizeSet.StateIndex = SizeChangesStartIndex;
    SizeSet.From = GetOwner()->GetActorLocation();
    StateSets.Add(SizeSet);

    for(int i = 0; i< StateSets.Num(); i++){
        StateSet * CurrentSet = &StateSets[i];
        if (CurrentSet->States.Num() > CurrentSet->StateIndex){
            FToState CurrentToState = CurrentSet->States[CurrentSet->StateIndex];
            CurrentSet->TimeOfNextIndex = CurrentToState.TimeTo + CurrentToState.PauseAfter;
        }else {
            if(CurrentSet->States.Num() > 0){
                GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Mover index out of bounds")));
            }
        }
    }
    ActorScale = GetOwner()->GetActorScale3D();

    // ...

}


// Called every frame
void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    TimeTotal += DeltaTime;
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    for (int i = 0; i < StateSets.Num(); i++){
        StateSet * CurrentSet = &StateSets[i];
        if (CurrentSet->States.Num() > CurrentSet->StateIndex){
            float NextTime = CurrentSet->TimeOfNextIndex;

            FToState * CurrentToState = &CurrentSet->States[CurrentSet->StateIndex];
            if (NextTime < TimeTotal){
                GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Next State")));
                CurrentSet->From = CurrentToState->Destination;
                if( CurrentSet->States.Num() > CurrentSet->StateIndex+1){
                    CurrentSet->StateIndex++;
                } else {
                    CurrentSet->StateIndex = 0;
                }
                CurrentToState = &CurrentSet->States[CurrentSet->StateIndex];
                CurrentSet->TimeOfNextIndex += CurrentToState->TimeTo + CurrentToState->PauseAfter;
            }
            float TimeRemaining = CurrentSet->TimeOfNextIndex-TimeTotal-CurrentToState->PauseAfter;
            float Progress = (CurrentToState->TimeTo-TimeRemaining)/CurrentToState->TimeTo;
            FVector Start = CurrentSet->From;
            FVector End = CurrentSet->States[CurrentSet->StateIndex].Destination;

            CurrentSet->Current = FMath::Lerp(Start,End,FMath::Clamp(Progress,0.0f,1.0f));
            //GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Green, FString::Printf(TEXT("Progress: %f clamped %f nextindex%f Total%f remain %f  timeto %f pause %f Current %s"), Progress, FMath::Clamp(Progress,0.0f,1.0f),CurrentSet->TimeOfNextIndex,TimeTotal,TimeRemaining, CurrentToState->TimeTo,CurrentToState->PauseAfter,*(CurrentSet->Current-End).ToString() ));
        }else {
            if(CurrentSet->States.Num() > 0){
                GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Mover index out of bounds")));
            }
        }
    }
    StateSet * LocationStateSet = &StateSets[0];
    if (LocationStateSet->States.Num() > LocationStateSet->StateIndex){
        GetOwner()->SetActorLocation(LocationStateSet->Current);
    }
    StateSet * RotationStateSet = &StateSets[1];
    if (RotationStateSet->States.Num() > RotationStateSet->StateIndex){
        GetOwner()->SetActorRotation(RotationStateSet->Current.Rotation());
    }
    StateSet * SizeStateSet = &StateSets[2];
    if (SizeStateSet->States.Num() > SizeStateSet->StateIndex){
        float x = ActorScale.X * SizeStateSet->Current.X;
        float y = ActorScale.Y * SizeStateSet->Current.Y;
        float z = ActorScale.Z * SizeStateSet->Current.Z;
        GetOwner()->SetActorScale3D(FVector(x,y,z));
    }


    // ...
}

