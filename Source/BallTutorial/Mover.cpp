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
    ReverseNotLoopLocation = false;
    ReverseNotLoopRotation = false;
    ReverseNotLoopSize = false;

    TimeToStartLocation = 5.0f;
    PauseAfterStartLocation = 0.0f;
    TimeToStartRotation = 5.0f;
    PauseAfterStartRotation = 0.0f;
    TimeToStartSize = 5.0f;
    PauseAfterStartSize = 0.0f;
    UpdateActorDestinations = false;

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
    StateSet LocationSet;
    LocationSet.From = GetOwner()->GetActorLocation();
    FToState StartLocation;
    StartLocation.Destination = LocationSet.From;
    StartLocation.TimeTo = TimeToStartLocation;
    StartLocation.PauseAfter = PauseAfterStartLocation;
    LocationChanges.Insert(StartLocation,0);

    LocationSet.States = LocationChanges;
    LocationSet.StateIndex = 1;
    StateSets.Add(LocationSet);

    StateSet RotationSet;
    RotationSet.From = GetOwner()->GetActorRotation().Vector();
    FToState StartRotation;
    StartRotation.Destination = RotationSet.From;
    StartRotation.TimeTo = TimeToStartRotation;
    StartRotation.PauseAfter = PauseAfterStartRotation;
    RotationChanges.Insert(StartRotation,0);

    RotationSet.States = RotationChanges;
    RotationSet.StateIndex = 1;
    StateSets.Add(RotationSet);

    StateSet SizeSet;
    SizeSet.From = FVector(1.0f,1.0f,1.0f);
    FToState StartSize;
    StartSize.Destination = SizeSet.From;
    StartSize.TimeTo = TimeToStartSize;
    StartSize.PauseAfter = PauseAfterStartSize;
    SizeChanges.Insert(StartSize,0);

    SizeSet.States = SizeChanges;
    SizeSet.StateIndex = 1;
    StateSets.Add(SizeSet);

    for(int i = 0; i< StateSets.Num(); i++){
        StateSet * CurrentSet = &StateSets[i];
        if (CurrentSet->States.Num() > CurrentSet->StateIndex){
            FToState CurrentToState = CurrentSet->States[CurrentSet->StateIndex];
            CurrentSet->TimeOfNextIndex = CurrentToState.TimeTo + CurrentToState.PauseAfter;
        }else {
            if(CurrentSet->States.Num() > 1){
                GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Mover index out of bounds")));
            }
        }
    }
    ActorScale = GetOwner()->GetActorScale3D();
    Reverses.Add(&ReverseNotLoopLocation);
    Reverses.Add(&ReverseNotLoopRotation);
    Reverses.Add(&ReverseNotLoopSize);

    for (int i = 1; i < LocationChanges.Num(); i++) {
        FToState * LocationPrior = &(StateSets[0].States[i-1]);
        FToState * LocationChange = &(StateSets[0].States[i]);
        if(LocationChange->ActorAsDestination != nullptr){
            LocationChange->Destination = LocationChange->ActorAsDestination->GetActorLocation();
        }else if(LocationChange->Relative){
            LocationChange->SaveRelative = LocationChange->Destination;
            LocationChange->Destination = LocationPrior->Destination + LocationChange->SaveRelative;
        }
    }

    // ...

}


// Called every frame
void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    if(UpdateActorDestinations){
        for (int i = 1; i < LocationChanges.Num(); i++) {
            FToState * LocationPrior = &(StateSets[0].States[i-1]);
            FToState * LocationChange = &(StateSets[0].States[i]);
            if(LocationChange->ActorAsDestination != nullptr){
                LocationChange->Destination = LocationChange->ActorAsDestination->GetActorLocation();
            }else if(LocationChange->Relative){
                LocationChange->Destination = LocationPrior->Destination + LocationChange->SaveRelative;
            }
        }
    }
    TimeTotal += DeltaTime;
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    for (int i = 0; i < StateSets.Num(); i++){
        StateSet * CurrentSet = &StateSets[i];
        if(CurrentSet->States.Num() > 1){
            float NextTime = CurrentSet->TimeOfNextIndex;

            FToState CurrentToState = CurrentSet->States[CurrentSet->StateIndex];
            if (NextTime < TimeTotal){
                CurrentSet->From = CurrentToState.Destination;
                if( CurrentSet->StateIndex+CurrentSet->IndexDirection < CurrentSet->States.Num() && CurrentSet->StateIndex+ CurrentSet->IndexDirection > -1){
                    if(i ==0){
                        //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("CONTINUE StateIndex %d IndexDirection %d States %d  bool1 %d bool2 %d"), CurrentSet->StateIndex,CurrentSet->IndexDirection,CurrentSet->States.Num(),CurrentSet->StateIndex+CurrentSet->IndexDirection < CurrentSet->States.Num(), CurrentSet->StateIndex+CurrentSet->IndexDirection > -1));
                    }
                    CurrentSet->StateIndex += CurrentSet->IndexDirection;
                } else if(*Reverses[i]){
                    if(i ==0){
                        //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("REVERSE StateIndex %d IndexDirection %d States %d  bool1 %d bool2 %d"), CurrentSet->StateIndex,CurrentSet->IndexDirection,CurrentSet->States.Num(),CurrentSet->StateIndex+CurrentSet->IndexDirection < CurrentSet->States.Num(), CurrentSet->StateIndex+CurrentSet->IndexDirection > -1));
                    }
                    CurrentSet->IndexDirection *= -1;
                    CurrentSet->StateIndex += CurrentSet->IndexDirection;
                } else {
                    if(i ==0){
                        //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("RESET StateIndex %d IndexDirection %d States %d  bool1 %d bool2 %d"), CurrentSet->StateIndex,CurrentSet->IndexDirection,CurrentSet->States.Num(),CurrentSet->StateIndex+CurrentSet->IndexDirection < CurrentSet->States.Num(), CurrentSet->StateIndex+CurrentSet->IndexDirection > -1));
                    }
                    CurrentSet->IndexDirection = 1;
                    CurrentSet->StateIndex = 0;
                }
                CurrentToState = CurrentSet->States[CurrentSet->StateIndex];

                FToState TimeToState;
                if (CurrentSet->IndexDirection != -1){
                    TimeToState = CurrentToState;
                } else {
                    TimeToState = CurrentSet->States[CurrentSet->StateIndex+1];
                }
                CurrentSet->TimeOfNextIndex += TimeToState.TimeTo + CurrentToState.PauseAfter;
            }
            FToState TimeToState;
            if (CurrentSet->IndexDirection != -1){
                TimeToState = CurrentToState;
            } else {
                TimeToState = CurrentSet->States[CurrentSet->StateIndex+1];
            }
            float TimeRemaining = CurrentSet->TimeOfNextIndex-TimeTotal-CurrentToState.PauseAfter;
            float Progress;
            if (TimeToState.TimeTo < 0.000001){
                Progress = 1.0;
            } else {
                Progress = (TimeToState.TimeTo-TimeRemaining)/TimeToState.TimeTo;
            }
            FVector Start = CurrentSet->From;
            FVector End = CurrentSet->States[CurrentSet->StateIndex].Destination;

            CurrentSet->Current = FMath::Lerp(Start,End,FMath::Clamp(Progress,0.0f,1.0f));
            //GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Green, FString::Printf(TEXT("Progress: %f clamped %f nextindex%f Total%f remain %f  timeto %f pause %f Current %s statecount %d"), Progress, FMath::Clamp(Progress,0.0f,1.0f),CurrentSet->TimeOfNextIndex,TimeTotal,TimeRemaining, TimeToState.TimeTo,TimeToState.PauseAfter,*(CurrentSet->Current).ToString() , CurrentSet->States.Num()));
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

