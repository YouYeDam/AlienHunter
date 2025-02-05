// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableActor.generated.h"

UCLASS()
class ALIENHUNTER_API AInteractableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AInteractableActor();

	virtual void Tick(float DeltaTime) override;
	
	// 상호작용 메서드
	UFUNCTION(BlueprintCallable)
	virtual void Interact(AActor* InteractingActor);

	void Collect(AActor* Collector);

	// 콜리전 이벤트 처리
	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh; // 액터의 메시

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* InteractionSphere;	// 상호작용 범위 컴포넌트

	bool bCanInteract = false; // 현재 상호작용 가능한 상태

	float InteractableRadius = 200; // 상호작용 범위 한계
};
