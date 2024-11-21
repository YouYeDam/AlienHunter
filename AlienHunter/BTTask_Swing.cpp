// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Swing.h"
#include "AIController.h"
#include "MainCharacter.h"

UBTTask_Swing::UBTTask_Swing()
{
    NodeName = TEXT("Swing");
}

EBTNodeResult::Type UBTTask_Swing::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    if (OwnerComp.GetAIOwner() == nullptr) {
        return EBTNodeResult::Failed;
    }
    AMainCharacter* Character = Cast<AMainCharacter>(OwnerComp.GetAIOwner()->GetPawn());
    if (Character == nullptr) {
        return EBTNodeResult::Failed;
    }
    Character->Swing();
    
    return EBTNodeResult::Succeeded;
}
