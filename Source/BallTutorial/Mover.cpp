// Fill out your copyright notice in the Description page of Project Settings.

#include "Mover.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UMover::UMover()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = true;
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
    FToState StartLocation;
    StartLocation.Destination = GetOwner()->GetActorLocation();
    StartLocation.TimeTo = TimeToStartLocation;
    StartLocation.PauseAfter = PauseAfterStartLocation;
    LocationChanges.Insert(StartLocation,0);

    LocationSet.States = &LocationChanges;
    LocationSet.PriorStateIndex = 0;
    LocationSet.StateIndex = 1;
    StateSets.Add(&LocationSet);

    FToState StartRotation;
    StartRotation.Destination = GetOwner()->GetActorRotation().Vector();
    StartRotation.TimeTo = TimeToStartRotation;
    StartRotation.PauseAfter = PauseAfterStartRotation;
    RotationChanges.Insert(StartRotation,0);

    RotationSet.States = &RotationChanges;
    RotationSet.StateIndex = 1;
    RotationSet.PriorStateIndex = 0;
    StateSets.Add(&RotationSet);

    FToState StartSize;
    StartSize.Destination = FVector(1.0f,1.0f,1.0f);
    StartSize.TimeTo = TimeToStartSize;
    StartSize.PauseAfter = PauseAfterStartSize;
    SizeChanges.Insert(StartSize,0);

    SizeSet.States = &SizeChanges;
    SizeSet.StateIndex = 1;
    SizeSet.PriorStateIndex = 0;
    StateSets.Add(&SizeSet);

    for(int StateSetI = 0; StateSetI< StateSets.Num(); StateSetI++){
        FStateSet * StateSet = StateSets[StateSetI];
        if(StateSet->States->Num() > 1){
            FToState * State = &(*StateSet->States)[StateSet->StateIndex];
            StateSet->TimeOfNextIndex = State->TimeTo + State->PauseAfter;
        }
    }
    ActorScale = GetOwner()->GetActorScale3D();
    Reverses.Add(&ReverseNotLoopLocation);
    Reverses.Add(&ReverseNotLoopRotation);
    Reverses.Add(&ReverseNotLoopSize);


    for (int StateSetI = 0; StateSetI < StateSets.Num(); StateSetI++){
        FStateSet * StateSet = StateSets[StateSetI];
        for (int StateI = 1; StateI < StateSet->States->Num(); StateI++) {

            FToState * StatePrior = &(*StateSet->States)[StateI-1];
            FToState * State = &(*StateSet->States)[StateI];
            if(State->ActorAsDestination != nullptr){
                //BAD FORM SHOULD BE AN ENUM
                if(StateSetI == 0){
                    State->Destination = State->ActorAsDestination->GetActorLocation();
                } else if (StateSetI == 1){
                    State->Destination = State->ActorAsDestination->GetActorRotation().Vector();
                } else if (StateSetI == 2){
                    State->Destination = UKismetMathLibrary::Divide_VectorVector(State->ActorAsDestination->GetActorScale3D(),ActorScale);
                }

            } else
            if(State->Relative){
                State->SaveRelative = State->Destination;
                State->Destination = StatePrior->Destination + State->SaveRelative;
            }
        }
    }
}


// Called every frame
void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    TimeTotal += DeltaTime;

    for (int StateSetI = 0; StateSetI < StateSets.Num(); StateSetI++){
        FStateSet * StateSet = StateSets[StateSetI];
        if(StateSet->States->Num() > 1){

            FToState * State;
            FToState * StatePrior;

            //Update Indexing if finished
            if (StateSet->TimeOfNextIndex < TimeTotal){
                StateSet->PriorStateIndex = StateSet->StateIndex;
                if( StateSet->StateIndex+StateSet->IndexDirection < StateSet->States->Num() && StateSet->StateIndex+ StateSet->IndexDirection > -1){
                    StateSet->StateIndex += StateSet->IndexDirection;
                } else if(*Reverses[StateSetI]){
                    StateSet->IndexDirection *= -1;
                    StateSet->StateIndex += StateSet->IndexDirection;
                } else {
                    StateSet->IndexDirection = 1;
                    StateSet->StateIndex = 0;
                }

                //We need the updated state
                StatePrior = &(*StateSet->States)[StateSet->PriorStateIndex];
                State = &(*StateSet->States)[StateSet->StateIndex];

                FToState * TimeToState;
                if (StateSet->IndexDirection != -1){
                    TimeToState = State;
                } else {
                    TimeToState = StatePrior;
                }
                StateSet->TimeOfNextIndex += TimeToState->TimeTo + State->PauseAfter;
            } else {
                StatePrior = &(*StateSet->States)[StateSet->PriorStateIndex];
                State = &(*StateSet->States)[StateSet->StateIndex];
            }

            //Update Destinations if we are referencing moving actors
            if(UpdateActorDestinations){
                if(State->ActorAsDestination != nullptr){
                    //USE ENUM
                    if(StateSetI == 0){
                        State->Destination = State->ActorAsDestination->GetActorLocation();
                    } else if (StateSetI == 1){
                        State->Destination = State->ActorAsDestination->GetActorRotation().Vector();
                    } else if (StateSetI == 2){
                        State->Destination = UKismetMathLibrary::Divide_VectorVector(State->ActorAsDestination->GetActorScale3D(),ActorScale);
                    }
                } else if(State->Relative){
                    State->Destination = StatePrior->Destination + State->SaveRelative;
                }
            }

            //Progress based on time code
            FToState * TimeToState;
            if (StateSet->IndexDirection != -1){
                TimeToState = State;
            } else {
                TimeToState = StatePrior;
            }

            float TimeRemaining = StateSet->TimeOfNextIndex-TimeTotal-State->PauseAfter;
            float Progress;
            if (TimeToState->TimeTo < 0.000001){
                Progress = 1.0;
            } else {
                Progress = (TimeToState->TimeTo-TimeRemaining)/TimeToState->TimeTo;
            }
            FVector Start = StatePrior->Destination;
            FVector End = State->Destination;

            FVector Current = FMath::Lerp(Start,End,FMath::Clamp(Progress,0.0f,1.0f));
            /*
            GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Green, FString::Printf(
                        TEXT("Progress: %f clamped %f nextindex%f Total%f remain %f timeto %f pause %f Current %s statecount %d"), 
                        Progress, 
                        FMath::Clamp(Progress,0.0f,1.0f),
                        StateSet->TimeOfNextIndex,
                        TimeTotal, 
                        TimeRemaining,
                        TimeToState->TimeTo, 
                        State->PauseAfter,
                        *State->Destination.ToString(),
                        StateSet->States->Num()));
                        */

            //Actually set the state 
            //USE ENUM
            switch(StateSetI){
                case 0:
                    GetOwner()->SetActorLocation(Current);
                    break;
                case 1:
                    GetOwner()->SetActorRotation(Current.Rotation());
                    break;
                case 2:
                    FVector AdjustedScale = UKismetMathLibrary::Multiply_VectorVector(Current, ActorScale);
                    GetOwner()->SetActorScale3D(AdjustedScale);
            }

            //Do wrap up stuff for next iteration


        }
    }
}

//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("RESET StateIndex %d IndexDirection %d States %d  bool1 %d bool2 %d"), CurrentSet->StateIndex,CurrentSet->IndexDirection,CurrentSet->States.Num(),CurrentSet->StateIndex+CurrentSet->IndexDirection < CurrentSet->States.Num(), CurrentSet->StateIndex+CurrentSet->IndexDirection > -1));
//GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Green, FString::Printf(TEXT("Progress: %f clamped %f nextindex%f Total%f remain %f  timeto %f pause %f Current %s statecount %d"), Progress, FMath::Clamp(Progress,0.0f,1.0f),CurrentSet->TimeOfNextIndex,TimeTotal,TimeRemaining, TimeToState.TimeTo,TimeToState.PauseAfter,*(CurrentSet->Current).ToString() , CurrentSet->States.Num()));
//GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Green, FString::Printf(
//TEXT("Current %s"), 
//*(StatesVal[StateSet->StateIndex].Destination.ToString()) ));
