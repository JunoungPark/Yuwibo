// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Components/ActorComponent.h"
#include "InformationComponent.generated.h"

/**
 * 
 */


UCLASS()
class YUWIBO_API UInformationComponent : public UActorComponent
{
	GENERATED_BODY()

	UInformationComponent();
private:
	UPROPERTY()
	class ABaseCharacter* Character;

	UPROPERTY()
	UInformationWidget* InformationWidget;
public:
	void BeginPlay() override;

	void AddViewportWidget();

	void DestroyWidget();

	void SetData();

	void OnOffInformation();

	FORCEINLINE void Bind(ABaseCharacter* NewCharacter) { Character = NewCharacter; }
};

UCLASS()
class YUWIBO_API UInformationEntryWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

	friend void UInformationComponent::SetData();
private:
	UPROPERTY()
	FName CharacterName;

	UPROPERTY(meta = (BindWidget))
	class UImage* Image;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Key;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Information;
private:

	void SetImage(int32 Index);
protected:

	void NativeOnListItemObjectSet(UObject* ListItemObject) override;
public:

};

UCLASS()
class YUWIBO_API UInformationWidget : public UUserWidget
{
	GENERATED_BODY()

	friend void UInformationComponent::SetData();
private:
	UPROPERTY(meta = (BindWidget))
	class UListView* Informations;

};
