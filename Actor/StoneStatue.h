// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StoneStatue.generated.h"

UCLASS()
class YUWIBO_API AStoneStatue : public AActor
{
	GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	AStoneStatue();

private:
	UStaticMeshComponent* StaticMesh;
	

	UPROPERTY(ReplicatedUsing = OnRep_DissolveAmount)
	float DissolveAmount = 0.f;

private:
	UFUNCTION()
	void OverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(NetMulticast, Reliable)
	void OnVisible();

	UFUNCTION()
	void OnRep_DissolveAmount();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetVisible();
};
