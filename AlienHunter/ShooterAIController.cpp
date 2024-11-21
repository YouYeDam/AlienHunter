// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MainCharacter.h"

void AShooterAIController::BeginPlay()
{
    Super::BeginPlay();

    if (AIBehavior != nullptr) {
        RunBehaviorTree(AIBehavior);
        GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
    }
}

void AShooterAIController::Tick(float DeltaTime)
{

    Super::Tick(DeltaTime);
}

bool AShooterAIController::IsDead() const
{
    AMainCharacter* ControlledCharacter = Cast<AMainCharacter>(GetPawn());

    if (ControlledCharacter != nullptr) {
        return ControlledCharacter->IsDead();
    }

    return true;
}
