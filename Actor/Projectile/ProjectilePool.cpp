// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectilePool.h"
#include "Projectile.h"

void UProjectilePool::ReturnProjectile(AProjectile* Projectile)
{
    Projectile->Deactivate();// ��Ȱ��ȭ

    ProjectilePool.Emplace(Projectile);
}
