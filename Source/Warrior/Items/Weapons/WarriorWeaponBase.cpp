// Groundhog & Drake All Rights Reserved.

#include "WarriorWeaponBase.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Pawn.h"
#include "Warrior/WarriorDebugHelper.h"

AWarriorWeaponBase::AWarriorWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponMesh);

	WeaponCollisionBox = CreateDefaultSubobject<UBoxComponent>("WeaponCollisionBox");
	WeaponCollisionBox->SetupAttachment(GetRootComponent());
	WeaponCollisionBox->SetBoxExtent(FVector(20.f));
	WeaponCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	WeaponCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnCollisionBoxBeginOverlap);
	WeaponCollisionBox->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnCollisionBoxEndOverlap);
}

void AWarriorWeaponBase::OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
													UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
													const FHitResult& SweepResult)
{
	auto WeaponOwningPawn = GetInstigator<APawn>();
	check(WeaponOwningPawn);

	if (auto* HitPawn = Cast<APawn>(OtherActor)) {
		if (WeaponOwningPawn != HitPawn) {
			Debud::Print(GetName() + TEXT("BeginOverlap") + HitPawn->GetName(), FColor::Green);
		}
	}
}

void AWarriorWeaponBase::OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
												  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	auto WeaponOwningPawn = GetInstigator<APawn>();
	check(WeaponOwningPawn);

	if (auto* HitPawn = Cast<APawn>(OtherActor)) {
		if (WeaponOwningPawn != HitPawn) {
			Debud::Print(GetName() + TEXT("EndOverlap") + HitPawn->GetName(), FColor::Green);
		}
	}
}
