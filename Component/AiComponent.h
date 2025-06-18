// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "AiComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class YUWIBO_API UAiComponent : public UActorComponent
{
	GENERATED_BODY()

private:

	class ACharacter* Target = nullptr;

	class USphereComponent* SphereComponent;

	class ACharacter* Character;

	class UAIControllComponent* OwnerComponent;

	UPROPERTY(EditAnywhere)
	TArray<class ACharacter*> Characters;

	UPROPERTY(EditAnywhere)
	TArray<ACharacter*> LowpriorityCharacters;
private:

	UFUNCTION()
	virtual void OverlapCharacter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OverlapEndCharacter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void UpdateTarget();
protected:
	void BeginPlay()override;

public:
	UAiComponent();

	ACharacter* GetTarget();

	bool GetCheckOwnerLockOnAndTarget();

	void AddLowpriorityCharacters(ACharacter* NewCharacter);

	FORCEINLINE void SetSphereRadius(float Radius) { SphereComponent->SetSphereRadius(Radius); }

	void RemoveCharacter(ACharacter* NewCharacter);

	float GetSphereRadius() { return SphereComponent->GetScaledSphereRadius(); }

	void Bind(class ACharacter* NewCharacter);
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class YUWIBO_API UEnemyComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyComponent();

private:

	class ACharacter* Character;
	UPROPERTY(EditAnywhere)
	int TrainerCount;
	UPROPERTY(EditAnywhere)
	TArray<class ACharacter*> AttackCharacters;

public:

	FORCEINLINE void AddAttackCharacter(ACharacter* NewCharacter) { AttackCharacters.Emplace(NewCharacter); };

	FORCEINLINE void RemoveAttackCharacters(ACharacter* RemoveCharacter) { AttackCharacters.Remove(RemoveCharacter); };

	FORCEINLINE void EmptyAttackCharacters() { AttackCharacters.Empty(); };

	FORCEINLINE bool FindCharacter(class ACharacter* FindCharacter) const { return AttackCharacters.Find(FindCharacter) != INDEX_NONE; };

	FORCEINLINE void AddTrainerCount() { TrainerCount++; };

	UFUNCTION(BlueprintPure)
	FORCEINLINE int GetTrainerCount() const { return TrainerCount; };

	void Bind(class ACharacter* NewCharacter);
};

UCLASS()
class YUWIBO_API UAIControllComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UAIControllComponent();
private:
	class ACharacter* Character;

	class USphereComponent* SphereComponent;
	UPROPERTY()
	class ACharacter* Target;
	UPROPERTY()
	bool LockOn;
	UPROPERTY(EditAnywhere)
	TArray<class ACharacter*> Characters;

protected:
	UFUNCTION()
	virtual void OverlapCharacter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OverlapEndCharacter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
public:

	void SetTarget(ACharacter* NewCharacter);
	FORCEINLINE void SetLockOn(bool IsTrue) { LockOn = IsTrue; };
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool GetLockOn() const { return LockOn; };
	UFUNCTION(BlueprintPure)
	FORCEINLINE USphereComponent* GetSphereComponent() const { return SphereComponent; };
	UFUNCTION(BlueprintPure)
	FORCEINLINE class ACharacter* GetTarget() const { return Target; };
	UFUNCTION(BlueprintPure)
	FORCEINLINE TArray<class ACharacter*> GetCharacters() const { return Characters; };

	void Bind(class ACharacter* NewCharacter);

	FORCEINLINE void ResetTarget() { Target = nullptr; LockOn = false; }
};
