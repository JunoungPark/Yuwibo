// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ProjectilePool.generated.h"

/**
 * 
 */
UCLASS()
class YUWIBO_API UProjectilePool : public UObject
{
	GENERATED_BODY()

private:
    UPROPERTY()
    TSet<class AProjectile*> ProjectilePool; // �Ѿ� ����Ʈ
    
    double Size = 1;

    AActor* Owner = nullptr;
public:

    // �ʱ�ȭ
    template<typename T>
    void InitializePool(AActor* NewOwner, double NewSize = 0)
    {
        if (!NewOwner->HasAuthority()) return;

        Owner = NewOwner;
        if (NewSize) Size = NewSize;

        FActorSpawnParameters spawnParameters;
        spawnParameters.Owner = Owner;
        spawnParameters.Instigator = Owner->GetInstigator();

        for (uint8 i = 0; i < 20; ++i)
        {
            if (auto NewProjectile = GetWorld()->SpawnActor<T>(FVector(0, 0, 1000), FRotator(), spawnParameters))
            {
                NewProjectile->SetProjectilePool(this); // �ʱ�ȭ �� ��Ȱ��ȭ
                if (Size != 1) NewProjectile->SetActorScale3D(FVector(Size));
                ProjectilePool.Emplace(NewProjectile);
            }
        }
    }

    template<typename T>
    T* GetProjectile(FVector Location, FRotator Rotation)
    {
        if (!Owner) return nullptr;

        T* Projectile = nullptr;

        if (ProjectilePool.Num() > 0)
        {
            // TSet�� ù ��° ��� ��������
            Projectile = static_cast<T*>(*ProjectilePool.begin());
            if(Projectile) // ��� ����
            {
                ProjectilePool.Remove(Projectile);

                Projectile->SetActorLocationAndRotation(Location, Rotation);
             
            }
        }
        else
        {
            FActorSpawnParameters spawnParameters;
            spawnParameters.Owner = Owner;
            spawnParameters.Instigator = Owner->GetInstigator();

            Projectile = GetWorld()->SpawnActor<T>(Location, Rotation, spawnParameters);
            if (Projectile) 
            {
                Projectile->SetProjectilePool(this);
                if (Size != 1) Projectile->SetActorScale3D(FVector(Size));
            }
        }

        if (Projectile) Projectile->Activate();
        return Projectile;
    }
    

    // �Ѿ� ��ȯ
    void ReturnProjectile(AProjectile* Projectile);

};
