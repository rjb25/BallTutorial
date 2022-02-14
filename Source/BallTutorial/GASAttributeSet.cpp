// Fill out your copyright notice in the Description page of Project Settings.


#include "Net/UnrealNetwork.h"
#include "GASAttributeSet.h"

UGASAttributeSet::UGASAttributeSet()
{
}

void UGASAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, Health, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, AttackPower, COND_None, REPNOTIFY_Always);
}

void UGASAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) 
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, Health, OldHealth);
}

void UGASAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina) 
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, Stamina, OldStamina);
}

void UGASAttributeSet::OnRep_AttackPower(const FGameplayAttributeData& OldAttackPower) 
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, AttackPower, OldAttackPower);
}