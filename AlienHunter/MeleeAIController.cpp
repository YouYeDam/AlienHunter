// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MainCharacter.h"

void AMeleeAIController::BeginPlay()
{
    Super::BeginPlay();
    
    if (AIBehavior != nullptr) {
        RunBehaviorTree(AIBehavior);
        GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
    }
}

void AMeleeAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

bool AMeleeAIController::IsDead() const
{
    AMainCharacter* ControlledCharacter = Cast<AMainCharacter>(GetPawn());

    if (ControlledCharacter != nullptr) {
        return ControlledCharacter->IsDead();
    }

    return true;
}
