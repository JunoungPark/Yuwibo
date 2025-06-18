// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Yuwibo/Component/ItemComponent.h"
#include "ItemBox.generated.h"

UCLASS()
class YUWIBO_API AItemBox : public AActor, public ItemsHolder
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBox();

private:

	UPROPERTY(VisibleAnywhere)
	TSet<UItemComponent*> ConnectPlayer;
	UPROPERTY(VisibleAnywhere)
	FTimerHandle TimerHandle;
	//TArray<UItem*> Item;
	UPROPERTY(VisibleAnywhere)
		USkeletalMeshComponent* SkeletalMesh;
	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* BoxComponent;
	//UPROPERTY(VisibleAnywhere)
	//	int Item[8];
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_OpenCount)
	uint8 OpenCount = 0;
private:
	UFUNCTION()
	void OnRep_OpenCount();

	void SpawnMonster();
protected:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
		void OverlapPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OverlapEndPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
		
public:	

	void Open(UItemComponent* ItemComponent);
	
	void Close(UItemComponent* ItemComponent);

	TSet<UItemComponent*> GetConnectPlayer() { return ConnectPlayer; };

	AActor* GetActor() { return this; };

};
