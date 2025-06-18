// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FootIkComponent.generated.h"


USTRUCT()
struct YUWIBO_API FTwoFootIkStruct
{
	GENERATED_USTRUCT_BODY()
public:

	float HipOffset;

	FRotator LeftFootRotatorValue;

	FRotator RightFootRotatorValue;

	float LeftFootOffset;

	float RightFootOffset;

};

USTRUCT()
struct YUWIBO_API FFourFootIkStruct
{
	GENERATED_USTRUCT_BODY()
public:

	float HipOffset;

	FRotator LeftFootRotatorValue;

	FRotator RightFootRotatorValue;

	FRotator BackLeftFootRotatorValue;

	FRotator BackRightFootRotatorValue;

	float LeftFootOffset;

	float RightFootOffset;

	float BackLeftFootOffset;

	float BackRightFootOffset;

};

UCLASS(Abstract, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class YUWIBO_API UFootIkComponent : public UActorComponent
{
	GENERATED_BODY()

protected:

	UPROPERTY(Category = FootIk, EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	class ACharacter* Character;
	UPROPERTY(Category = FootIk, EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	float HipOffset;
public:	
	// Sets default values for this component's properties
	UFootIkComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	float FootRotation(FVector SocketLocation, FRotator& FootRotator, float TraceDistance, float DeltaTime);

public:	
	// Called every frame
		
	void Bind(ACharacter* NewCharacter);

};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class YUWIBO_API UTwoFootIkComponent : public UFootIkComponent
{

	GENERATED_BODY()

private:
	UPROPERTY(Category = FootIk, EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	FName FirstSocket;
	UPROPERTY(Category = FootIk, EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	FName SecondSocket;
	UPROPERTY(Category = FootIk, EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	FRotator LeftFootRotatorValue;
	UPROPERTY(Category = FootIk, EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	FRotator RightFootRotatorValue;
	UPROPERTY(Category = FootIk, EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	float LeftFootOffset;
	UPROPERTY(Category = FootIk, EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	float RightFootOffset;
private:

	void FootIk(float DeltaTime);
public:
	// Sets default values for this component's properties

	DECLARE_DELEGATE_OneParam(FootIkSignature, FTwoFootIkStruct)
	FootIkSignature OnFootIk;
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void BindSocket(FName FirstSocketName, FName SecondSocketName);

	

};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class YUWIBO_API UFourFootIkComponent : public UFootIkComponent
{
	GENERATED_BODY()

private:	
	UPROPERTY(Category = FootIk, EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	FName FirstSocket;
	UPROPERTY(Category = FootIk, EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	FName SecondSocket;
	UPROPERTY(Category = FootIk, EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	FName ThirdSocket;
	UPROPERTY(Category = FootIk, EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	FName FourthSocket;
	UPROPERTY(Category = FootIk, EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	FRotator LeftFootRotatorValue;
	UPROPERTY(Category = FootIk, EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	FRotator RightFootRotatorValue;
	UPROPERTY(Category = FootIk, EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	FRotator BackLeftFootRotatorValue;
	UPROPERTY(Category = FootIk, EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	FRotator BackRightFootRotatorValue;
	UPROPERTY(Category = FootIk, EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	float LeftFootOffset;
	UPROPERTY(Category = FootIk, EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	float RightFootOffset;
	UPROPERTY(Category = FootIk, EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	float BackLeftFootOffset;
	UPROPERTY(Category = FootIk, EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	float BackRightFootOffset;
private:

	void FootIk(float DeltaTime);
public:
	// Sets default values for this component's properties

	DECLARE_DELEGATE_OneParam(FootIkSignature, FFourFootIkStruct)
	FootIkSignature OnFootIk;
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void BindSocket(FName FirstSocketName, FName SecondSocketName, FName ThirdSocketName, FName FourthSocketName);



};