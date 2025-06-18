// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

UCLASS()
class YUWIBO_API UHpWidget : public UUserWidget
{
	GENERATED_BODY()
private:

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* PB_HpBar;

public:

	FORCEINLINE class UProgressBar* GetPB_HpBar() { return PB_HpBar; };
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class YUWIBO_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	// Sets default values for this component's properties
	UHealthComponent();

	DECLARE_EVENT(UHealthComponent,FDissolveEndEVENT)
	FDissolveEndEVENT DissolveEnd;
private:
	UPROPERTY()
	TSet<UMeshComponent*> Meshs;

	UPROPERTY(ReplicatedUsing = OnRep_DissolveAmount)
	float DissolveAmount = 0.f;

	bool DissolveOff = false;

	class ABaseCharacter* Character;

	TSet<UObject*> RelatedComponents;
	/** �÷��̾��� �ִ� ü��. ü���� �ִ��Դϴ�. �� ���� ���� �� ���۵Ǵ� ĳ������ ü�� ���Դϴ�.*/
	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float MaxHealth;

	/** �÷��̾��� ���� ü��. 0�� �Ǹ� ���� ������ ���ֵ˴ϴ�.*/
	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth)
	float CurrentHealth;

	UPROPERTY(VisibleAnywhere)
	UHpWidget* HpWidget;
	/** ���� ü�¿� ������ ���濡 ���� RepNotify*/
	
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_DissolveAmount();

	UFUNCTION()
	void OnRep_CurrentHealth();
	
	void OnHealthUpdate();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetCurrentHealth(float healthValue);

	FORCEINLINE void SetMaxHealth(float healthValue) { MaxHealth = healthValue; CurrentHealth = MaxHealth; };

	void DestroyWidget();

	void AddViewportWidget();

	void Bind(class ABaseCharacter* NewCharacter);

	FORCEINLINE void AddRelatedComponents(UObject* NewObject) { RelatedComponents.Emplace(NewObject); }
	FORCEINLINE void RemoveRelatedComponents(UObject* NewObject) { RelatedComponents.Remove(NewObject); }

	UFUNCTION(BlueprintPure, Category = "Health")
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

	UFUNCTION(BlueprintPure, Category = "Health")
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }

	FORCEINLINE float GetReducedHealth() { return MaxHealth - CurrentHealth; }
	
	FORCEINLINE void Dissolving() { DissolveAmount = 1.5; }

	FORCEINLINE bool IsDissolving() { return DissolveAmount != 0.f; }

	FORCEINLINE void StartDissolve() { SetComponentTickEnabled(true); }

	UFUNCTION()
	FORCEINLINE void StartDissolveOff() 
	{
		if (DissolveAmount == 1.5f)
		{
			DissolveOff = true; SetComponentTickEnabled(true);
		}
	}
	
	FORCEINLINE void AddMesh(UMeshComponent* Mesh) { Meshs.Emplace(Mesh); }

	FORCEINLINE TSet<UMeshComponent*> GetMesh() { return Meshs; }

};
