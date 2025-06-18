// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AnimComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class YUWIBO_API UAnimComponent : public UActorComponent
{
	GENERATED_BODY()
protected:
	UPROPERTY()
	class ABaseCharacter* Character; 

	UPROPERTY()
	class UBaseAnimInstance* Anim;

	UPROPERTY()
	class UCharacterMovementComponent* CharacterMovement;


	UPROPERTY()
	bool IsStop = false;

	UPROPERTY()
	bool Attacking = false;

	UPROPERTY(ReplicatedUsing = OnRep_UpdateAnimationValues)
	int32 SyncTrigger;
	UPROPERTY(Replicated)
	FRotator DefaultRotator;
	UPROPERTY(Replicated)
	bool IsAI;
	UPROPERTY(Replicated)
	bool RotationToMove;
	UPROPERTY(Replicated)
	bool IsAttack;
	UPROPERTY(Replicated)
	bool IsCrouch = false;
	UPROPERTY(Replicated)
	bool IsFalling;
	UPROPERTY(Replicated)
	FVector Velocity;
	UPROPERTY(Replicated)
	float GroundSpeed;
	UPROPERTY(Replicated)
	bool IsMoving;
	UPROPERTY(Replicated)
	float BlendAlpha;
	UPROPERTY(Replicated)
	FRotator AimRotation;
	UPROPERTY(Replicated)
	float YawOffset;
	UPROPERTY(Replicated)
	float RootYaw;
	UPROPERTY(Replicated)
	float AttackYaw;
	UPROPERTY(Replicated)
	FRotator MovingRotation;
	UPROPERTY(Replicated)
	FRotator FinalRotation; 
	UPROPERTY(Replicated)
	float ForwardValue;
	UPROPERTY(Replicated)
	float RightValue;
	UPROPERTY(Replicated)
	bool IsDead;
public:	
	// Sets default values for this component's properties
	UAnimComponent();

private:

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool GetIsAttack() const { return IsAttack; };

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetGroundSpeed() const { return GroundSpeed; };

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetBlendAlpha() const { return BlendAlpha; };


protected:

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void UpdateAnimation(float DeltaSeconds);

	virtual void SetRootYaw(float DeltaSeconds);

	virtual void SetIsAI();

public:

	UFUNCTION(Server, Reliable)
	void SetIsCrouch(bool IsTrue);
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	virtual void OnRep_UpdateAnimationValues();

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool GetIsCrouch() const { return IsCrouch; };

	UFUNCTION()
	FORCEINLINE void SetIsAttack(bool IsTrue) { IsAttack = IsTrue; }

	FORCEINLINE void SetAttackYaw(float value = 0.f) { AttackYaw = value; }

	FORCEINLINE void SetIsDead(bool IsTrue) { IsDead = IsTrue; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetRootYaw() const { return RootYaw; };


	virtual void Bind(class ABaseCharacter* NewCharacter);
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class YUWIBO_API UBodyChangerAnimComponent : public UAnimComponent
{

	GENERATED_BODY()
private:
	UPROPERTY()
	class UBodyChangerAnimInstance* BodyChangerAnim;

	FVector AttackHitResult;

	bool LookSky;

	float BodyChangerAttackYaw;

protected:

	void BeginPlay() override;

public:
	void OnRep_UpdateAnimationValues()override;

	void SetAttackRotation();

	void SetRootYaw(float DeltaSeconds)override;

	UFUNCTION(Server, Reliable)
	void SetServerAttackYaw(float Yaw);

	UFUNCTION(NetMulticast, Reliable)
	void SetNetAttackYaw(float Yaw);

}; 

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class YUWIBO_API UDroneAnimComponent : public UAnimComponent
{

	GENERATED_BODY()
private:
	UPROPERTY()
	class UDroneAnimInstance* DroneAnim;

	UPROPERTY(Replicated)
	bool LookSky;

	UPROPERTY(Replicated)
	FVector TargetLocation;

	UPROPERTY(Replicated)
	int8 UpDown;

	UPROPERTY(Replicated)
	float FlightHeight = 300.;
private:
	void BeginPlay() override;

	void SetRootYaw(float DeltaSeconds)override;

	void AIMove(float DeltaSeconds);

	void Fly(float DeltaSeconds);
protected:

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	void OnRep_UpdateAnimationValues()override;

	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(Server, Reliable)
	void SetUpDown(int8 Direction);
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class YUWIBO_API UGroundTurretAnimComponent : public UAnimComponent
{
	GENERATED_BODY()
private:
	UPROPERTY()
	class UGroundTurretAnimInstance* GroundTurretAnim;

	UPROPERTY(Replicated)
	FRotator GunRotation;
private:

	void UpdateAnimation(float DeltaSeconds) override;

protected:

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// Called when the game starts
	virtual void BeginPlay() override;

	void SetRootYaw(float DeltaSeconds)override;
public:
	void OnRep_UpdateAnimationValues()override;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class YUWIBO_API USoldierAnimComponent : public UAnimComponent
{
	GENERATED_BODY()
private:

	UPROPERTY()
	class USoldierAnimInstance* SoldierAnim;

	UPROPERTY(Replicated)
	FRotator HeadRotator;
	UPROPERTY(Replicated)
	FRotator HandRotator;
	UPROPERTY(Replicated)
	float CameraMovingYaw;
	UPROPERTY(Replicated)
	float CameraFinalYaw;
	UPROPERTY(Replicated)
	float BlendWeight;
	UPROPERTY(Replicated)
	bool IsLeftTurn = false;
	UPROPERTY(Replicated)
	bool IsRightTurn = false;

private:
	void UpdateAnimation(float DeltaSeconds) override;

	void SetRootYaw(float DeltaSeconds) override;

protected:

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	void OnRep_UpdateAnimationValues()override;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class YUWIBO_API UTankAnimComponent : public UAnimComponent
{
	GENERATED_BODY()
private:
	UPROPERTY()
	class ATank* Tank;

protected:

	UPROPERTY()
	class UTankAnimInstance* TankAnim;

	UPROPERTY()
	bool Result;
	UPROPERTY(Replicated)
	FVector ComponentLocation;
	UPROPERTY(Replicated)
	bool Mode;
	UPROPERTY()
	FHitResult HitResult;
	UPROPERTY()
	double LookYawAngle;
	UPROPERTY()
	double LookPitchAngle;
	UPROPERTY(Replicated)
	FVector AttackHitResult;
private:

	void SetRootYaw(float DeltaSeconds) override;

	UFUNCTION(NetMulticast, Reliable)
	void GunAttack(bool IsTrue, bool IsLookSky, FVector Vector);

	UFUNCTION(NetMulticast, Reliable)
	void SetNetAttackInfo(bool LookSky, FVector TargetLocation);
protected:
	virtual void BeginPlay() override;

	virtual void UpdateTankAnim(float DeltaSeconds);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void UpdateLookAngles(const FRotator& Rotator, float DeltaSeconds);

	UFUNCTION(Server, Reliable)
	void SetAttackInfo(bool LookSky, FVector TargetLocation, double Yaw, double Pitch);
public:
	void OnRep_UpdateAnimationValues()override; 
	
	void Bind(class ABaseCharacter* NewCharacter)override;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class YUWIBO_API UTankLv3AnimComponent : public UTankAnimComponent
{
	GENERATED_BODY()

private:
	UPROPERTY()
	class ATankLv3* TankLv3;

	UPROPERTY(Replicated)
	bool IsGLauncherMode = false;
private:
	void UpdateTankAnim(float DeltaSeconds) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	void OnRep_UpdateAnimationValues()override;

	void Bind(class ABaseCharacter* NewCharacter)override; 
	
	FORCEINLINE void ChangeGLauncherMode() { IsGLauncherMode = !IsGLauncherMode; }

	FORCEINLINE bool GetGLauncherMode() { return IsGLauncherMode; }
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class YUWIBO_API UTrainerAnimComponent : public UAnimComponent
{
	GENERATED_BODY()
protected:

	UPROPERTY()
	class ATrainer* Trainer;

	UPROPERTY()
	class UTrainerAnimInstance* TrainerAnim; 

	UPROPERTY(Replicated)
	FRotator BlendRotator;
	UPROPERTY(Replicated)
	FVector AttackHitResult;
	UPROPERTY(Replicated)
	bool LookSky;
	UPROPERTY(Replicated)
	class AMoose* RideMoose;
	UPROPERTY(Replicated)
	bool IsRide;
	UPROPERTY(Replicated)
	float MooseBlend;
	UPROPERTY(Replicated)
	float MooseSpeed;
private:
	void SetRootYaw(float DeltaSeconds) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	virtual void BeginPlay() override;

public:
	void OnRep_UpdateAnimationValues()override;

	void Bind(class ABaseCharacter* NewCharacter)override;

};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class YUWIBO_API UEnemyAnimComponent : public UAnimComponent
{
	GENERATED_BODY()
protected:

	UPROPERTY()
	class UBaseEnemyAnimInstance* BaseEnemyAnim;

	UPROPERTY(Replicated)
	bool IsFailMovoTo = false;
	UPROPERTY(Replicated)
	bool IsLook = false;
	UPROPERTY(Replicated)
	bool IsPlayer = true;
	UPROPERTY(Replicated)
	AActor* Target = nullptr;
	UPROPERTY(Replicated)
	FVector LookVector;
	UPROPERTY(Replicated)
	FRotator LookRotator;
private:

	UFUNCTION()
	void SetLook(bool IsTrue, FVector Vector) { IsLook = IsTrue;	LookVector = Vector; }


	UFUNCTION()
	FORCEINLINE void SetTarget(AActor* NewTarget) { Target = NewTarget; };

	void AILook(float DeltaSeconds);

	void AIMove();

protected:

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// Called when the game starts
	virtual void BeginPlay() override;

	void UpdateAnimation(float DeltaSeconds) override;
public:
	FRotator GetLookRotator() { return LookRotator; }

	void SetIsFailToMove(bool IsTrue) { IsFailMovoTo = IsTrue; }

	void OnRep_UpdateAnimationValues()override;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class YUWIBO_API UMooseAnimComponent : public UEnemyAnimComponent
{
	GENERATED_BODY()
private:

	void SetRootYaw(float DeltaSeconds) override;

	UFUNCTION(Server, Reliable)
	void AddRootYawServer(float value);
protected:

	void SetIsAI()override;
public:
	void AddRootYaw(float value);
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class YUWIBO_API URockMonsterAnimComponent : public UEnemyAnimComponent
{
	GENERATED_BODY()
protected:

	void UpdateAnimation(float DeltaSeconds) override;

};