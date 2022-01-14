#include "MyUtil.h"
#include "Engine.h"

bool MyUtil::GroundCheck(AActor * Actor, float GripDepth, float GripWidth) {
    FVector Start = Actor->GetActorLocation();
    FVector End = Start - FVector(0.0f,0.0f,GripDepth+0.001f);
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
    UKismetSystemLibrary::SphereTraceMultiForObjects(Actor->GetWorld(), Start, End, GripWidth, ObjectTypes, bTraceComplex, ActorsToIgnore, EDrawDebugTrace::None, OutHits, ignoreSelf, TraceColor, TraceHitColor, DrawTime);
    for (FHitResult hit : OutHits) {
        AActor * otherActor = hit.GetActor();
        UPrimitiveComponent * Mesh = otherActor->FindComponentByClass<UPrimitiveComponent>();
        if (Mesh){
            ECollisionResponse response = Mesh->GetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody);
        //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("ResponseChannel: %d %s %s"), response, *otherActor->GetName(), *Mesh->GetName()));
            if(response == ECollisionResponse::ECR_Block || response == ECollisionResponse::ECR_MAX){
                return true;
            }else{
                return false;
            }
        }
    }
            return false;
}

