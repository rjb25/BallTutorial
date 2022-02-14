// Fill out your copyright notice in the Description page of Project Settings.

#include "TransformMover.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UTransformMover::UTransformMover()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = true;
    SetIsReplicatedByDefault(true);

    // ...
}


// Called when the game starts
void UTransformMover::BeginPlay()
{
    Super::BeginPlay();

    Owner = GetOwner();
    Owner->SetReplicates(true);
    Body = Owner->FindComponentByClass<UStaticMeshComponent>();
    if (Body != nullptr){
        Body->SetMobility(EComponentMobility::Type::Movable);
    } else {
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("No static mesh for mover")));
    }
    Reference = Owner->GetActorTransform();
    for(int ToTransformI = 0; ToTransformI < ToTransforms.Num(); ToTransformI++){
        if(ToTransforms[ToTransformI].ActorAsDestination == nullptr){
            ToTransforms[ToTransformI].GlobalDestination = Reference * ToTransforms[ToTransformI].RelativeDestination;
        }
    }
}


// Called every frame
void UTransformMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if(ToTransforms.Num() > 1){
        TimeTotal += DeltaTime;
        if (TimeOfChange < TimeTotal && Owner->HasAuthority()){
            FromIndex = ToIndex;
            if( ToIndex + IndexDirection < ToTransforms.Num() && ToIndex + IndexDirection > -1){
                ToIndex += IndexDirection;
            } else if(ReverseNotLoop){
                IndexDirection *= -1;
                ToIndex += IndexDirection;
            } else {
                ToIndex = 0;
            }
            AllUpdate(FromIndex,ToIndex,IndexDirection);
        }

        float TimeRemaining = TimeOfChange-TimeTotal-PauseAfterDestination;
        float Progress;
        if (TimeToDestination < 0.000001){
            Progress = 1.0;
        } else {
            Progress = (TimeToDestination-TimeRemaining)/TimeToDestination;
        }
        FTransform Start = ToTransforms[FromIndex].GlobalDestination;
        FTransform End = ToTransforms[ToIndex].GlobalDestination;

        if(ToTransforms[FromIndex].ActorAsDestination != nullptr){
            FTransform ActorTransform = ToTransforms[FromIndex].ActorAsDestination->GetActorTransform();
            if(ActorLocation){
                Start.SetLocation(ActorTransform.GetLocation());
            }
            if(ActorRotation){
                Start.SetRotation(ActorTransform.GetRotation());
            }
            if(ActorScale){
                Start.SetScale3D(ActorTransform.GetScale3D());
            }
        }

        if(ToTransforms[ToIndex].ActorAsDestination != nullptr){
            FTransform ActorTransform = ToTransforms[ToIndex].ActorAsDestination->GetActorTransform();
            if(ActorLocation){
                End.SetLocation(ActorTransform.GetLocation());
            }
            if(ActorRotation){
                End.SetRotation(ActorTransform.GetRotation());
            }
            if(ActorScale){
                End.SetScale3D(ActorTransform.GetScale3D());
            }
        }

        FTransform Current = UKismetMathLibrary::TLerp(Start,End,FMath::Clamp(Progress,0.0f,1.0f));
        Owner->SetActorTransform(Current);
    }
}

bool UTransformMover::AllUpdate_Validate(int ServerFromIndex, int ServerToIndex, int ServerIndexDirection) {
    return true;
}

void UTransformMover::AllUpdate_Implementation(int ServerFromIndex, int ServerToIndex, int ServerIndexDirection) {
    FromIndex = ServerFromIndex;
    ToIndex = ServerToIndex;
    IndexDirection = ServerIndexDirection;
    if (IndexDirection != -1){
        TimeToDestination = ToTransforms[ToIndex].TimeTo;
    } else {
        TimeToDestination = ToTransforms[FromIndex].TimeTo;
    }
    PauseAfterDestination = ToTransforms[ToIndex].PauseAfter;
    TimeOfChange = TimeToDestination + PauseAfterDestination;
    TimeTotal = 0.0f;
}
