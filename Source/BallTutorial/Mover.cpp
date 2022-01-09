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
        if (StateSet->States->Num() > StateSet->StateIndex){
            TArray<FToState> StatesVal= *(StateSet->States);
            FToState State = StatesVal[StateSet->StateIndex];
            StateSet->TimeOfNextIndex = State.TimeTo + State.PauseAfter;
        }else {
            if(StateSet->States->Num() > 1){
                GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Mover index out of bounds")));
            }
        }
    }
    ActorScale = GetOwner()->GetActorScale3D();
    Reverses.Add(&ReverseNotLoopLocation);
    Reverses.Add(&ReverseNotLoopRotation);
    Reverses.Add(&ReverseNotLoopSize);


    for (int StateSetI = 0; StateSetI < StateSets.Num(); StateSetI++){
        FStateSet * StateSet = StateSets[StateSetI];
        for (int StateI = 1; StateI < StateSet->States->Num(); StateI++) {
            TArray<FToState> StatesVal= *(StateSet->States);
            FToState * StatePrior = &StatesVal[StateI-1];
            FToState * State = &StatesVal[StateI];
            if(State->ActorAsDestination != nullptr){
GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Not Null")));
                //BAD FORM SHOULD BE AN ENUM
                if(StateSetI == 0){
                    State->Destination = State->ActorAsDestination->GetActorLocation();
                } else if (StateSetI == 1){
                    State->Destination = State->ActorAsDestination->GetActorRotation().Vector();
                } else if (StateSetI == 2){
                    State->Destination = UKismetMathLibrary::Divide_VectorVector(State->ActorAsDestination->GetActorScale3D(),ActorScale);
                }

                if(State->Relative){
                    State->SaveRelative = State->Destination;
                    State->Destination = StatePrior->Destination + State->SaveRelative;
                }
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
        TArray<FToState> DynamicStatesVal = *(StateSet->States);
        if(DynamicStatesVal.Num() > 1){

            if(UpdateActorDestinations){
                for (int StateI = 1; StateI < StateSet->States->Num(); StateI++) {

                    FToState * StatePrior = &DynamicStatesVal[StateSet->StateIndex];
                    FToState * State = &DynamicStatesVal[StateSet->PriorStateIndex];
                    if(State->ActorAsDestination != nullptr){
                        //BAD FORM SHOULD BE AN ENUM
                        if(StateSetI == 0){
                            State->Destination = State->ActorAsDestination->GetActorLocation();
                        } else if (StateSetI == 1){
                            State->Destination = State->ActorAsDestination->GetActorRotation().Vector();
                        } else if (StateSetI == 2){
                            State->Destination = UKismetMathLibrary::Divide_VectorVector(State->ActorAsDestination->GetActorScale3D(),ActorScale);
                        }else if(State->Relative){
                            State->SaveRelative = State->Destination;
                            State->Destination = StatePrior->Destination + State->SaveRelative;
                        }
                    }
                }
            }
            float NextTime = StateSet->TimeOfNextIndex;

            TArray<FToState> StatesVal= *(StateSet->States);

            FToState * State = &StatesVal[StateSet->StateIndex];
            if (NextTime < TimeTotal){
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
                State = &StatesVal[StateSet->StateIndex];

                FToState * TimeToState;
                if (StateSet->IndexDirection != -1){
                    TimeToState = State;
                } else {
                    TimeToState = &StatesVal[StateSet->StateIndex+1];
                }
                StateSet->TimeOfNextIndex += TimeToState->TimeTo + State->PauseAfter;
            }
            FToState * TimeToState;
            if (StateSet->IndexDirection != -1){
                TimeToState = State;
            } else {
                TimeToState = &StatesVal[StateSet->StateIndex+1];
            }
            float TimeRemaining = StateSet->TimeOfNextIndex-TimeTotal-State->PauseAfter;
            float Progress;
            if (TimeToState->TimeTo < 0.000001){
                Progress = 1.0;
            } else {
                Progress = (TimeToState->TimeTo-TimeRemaining)/TimeToState->TimeTo;
            }
            FVector Start = StatesVal[StateSet->PriorStateIndex].Destination;
            FVector End = StatesVal[StateSet->StateIndex].Destination;

            StateSet->Current = FMath::Lerp(Start,End,FMath::Clamp(Progress,0.0f,1.0f));
        }
    }
    FStateSet * LocationStateSet = StateSets[0];
    if (LocationStateSet->States->Num() > LocationStateSet->StateIndex){
        GetOwner()->SetActorLocation(LocationStateSet->Current);
    }
    FStateSet * RotationStateSet = StateSets[1];
    if (RotationStateSet->States->Num() > RotationStateSet->StateIndex){
        GetOwner()->SetActorRotation(RotationStateSet->Current.Rotation());
    }
    FStateSet * SizeStateSet = StateSets[2];
    if (SizeStateSet->States->Num() > SizeStateSet->StateIndex){
        FVector AdjustedScale = UKismetMathLibrary::Multiply_VectorVector(SizeStateSet->Current, ActorScale);
        GetOwner()->SetActorScale3D(AdjustedScale);
    }
}

//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("RESET StateIndex %d IndexDirection %d States %d  bool1 %d bool2 %d"), CurrentSet->StateIndex,CurrentSet->IndexDirection,CurrentSet->States.Num(),CurrentSet->StateIndex+CurrentSet->IndexDirection < CurrentSet->States.Num(), CurrentSet->StateIndex+CurrentSet->IndexDirection > -1));
//GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Green, FString::Printf(TEXT("Progress: %f clamped %f nextindex%f Total%f remain %f  timeto %f pause %f Current %s statecount %d"), Progress, FMath::Clamp(Progress,0.0f,1.0f),CurrentSet->TimeOfNextIndex,TimeTotal,TimeRemaining, TimeToState.TimeTo,TimeToState.PauseAfter,*(CurrentSet->Current).ToString() , CurrentSet->States.Num()));
