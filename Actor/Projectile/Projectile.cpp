// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "ProjectilePool.h"
#include "GameFramework/ProjectileMovementComponent.h"
// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));

	ProjectileMovementComponent->bRotationFollowsVelocity = true;

	bReplicates = true;

	SetReplicateMovement(true);

	Deactivate();
}

//void AProjectile::Disappear()
//{
//	GetWorldTimerManager().ClearAllTimersForObject(this);
//	Destroy();
//}

void AProjectile::Disappear()
{
	ProjectilePool->ReturnProjectile(this);
}


void AProjectile::Activate()
{
	ProjectileMovementComponent->Activate(); 
	OnActivate();
	ProjectileMovementComponent->Velocity = GetActorRotation().Vector() * ProjectileMovementComponent->InitialSpeed;
	SetActorEnableCollision(true);
}

void AProjectile::OnActivate_Implementation()
{
	SetActorHiddenInGame(false);
}

void AProjectile::Deactivate()
{
	ProjectileMovementComponent->Deactivate(); 
	OnDeactivate();
	SetActorEnableCollision(false);
}

void AProjectile::OnDeactivate_Implementation()
{
	SetActorHiddenInGame(true);
}
