// Groundhog & Drake All Rights Reserved.

#include "BTTask_RotateToFaceTarget.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_RotateToFaceTarget::UBTTask_RotateToFaceTarget()
{
	NodeName = "Native Rotate to Face Target Actor";
	AnglePrecision = 10.f;
	RotationInterpSpeed = 5.f;

	bNotifyTick = true;
	bNotifyTaskFinished = true;
	bCreateNodeInstance = false;

	INIT_TASK_NODE_NOTIFY_FLAGS();

	InTargetToFaceKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, InTargetToFaceKey), AActor::StaticClass());
}

void UBTTask_RotateToFaceTarget::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (auto* BBAsset = GetBlackboardAsset()) {
		InTargetToFaceKey.ResolveSelectedKey(*BBAsset);
	}
}

uint16 UBTTask_RotateToFaceTarget::GetInstanceMemorySize() const
{
	return sizeof(FRotateToFaceTargetTaskMemory);
}

FString UBTTask_RotateToFaceTarget::GetStaticDescription() const
{
	const FString KeyDescription = InTargetToFaceKey.SelectedKeyName.ToString();
	return FString::Printf(TEXT("Smoothly rotates to face %s Key until the angle precision %s is reached"), *KeyDescription,
						   *FString::SanitizeFloat(AnglePrecision));
}

EBTNodeResult::Type UBTTask_RotateToFaceTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UObject* ActorObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(InTargetToFaceKey.SelectedKeyName);
	auto* TargetActor = Cast<AActor>(ActorObject);

	APawn* OwningPawn = OwnerComp.GetAIOwner()->GetPawn();

	auto* Memory = CastInstanceNodeMemory<FRotateToFaceTargetTaskMemory>(NodeMemory);
	check(Memory);

	Memory->OwningPawn = OwningPawn;
	Memory->TargetActor = TargetActor;

	if (!Memory->IsValid()) {
		return EBTNodeResult::Type::Failed;
	}

	if (HasReachedAnglePrecision(OwningPawn, TargetActor)) {
		Memory->Reset();
		return EBTNodeResult::Type::Succeeded;
	}
	return EBTNodeResult::Type::InProgress;
}

void UBTTask_RotateToFaceTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	auto* Memory = CastInstanceNodeMemory<FRotateToFaceTargetTaskMemory>(NodeMemory);
	if (!Memory->IsValid()) {
		FinishLatentTask(OwnerComp, EBTNodeResult::Type::Failed);
	}

	if (HasReachedAnglePrecision(Memory->OwningPawn.Get(), Memory->TargetActor.Get())) {
		Memory->Reset();
		FinishLatentTask(OwnerComp, EBTNodeResult::Type::Succeeded);
	} else {
		const auto LookAtRot = UKismetMathLibrary::FindLookAtRotation(Memory->OwningPawn->GetActorLocation(), Memory->TargetActor->GetActorLocation());
		const auto TargetRot = FMath::RInterpTo(Memory->OwningPawn->GetActorRotation(), LookAtRot, DeltaSeconds, RotationInterpSpeed);
		Memory->OwningPawn->SetActorRotation(TargetRot);
	}
}

bool UBTTask_RotateToFaceTarget::HasReachedAnglePrecision(APawn* QueryPawn, AActor* TargetActor) const
{
	const auto OwnerForward = QueryPawn->GetActorForwardVector();
	const auto OwnerToTargetNormalized = (TargetActor->GetActorLocation() - QueryPawn->GetActorLocation()).GetSafeNormal();

	const float DotResult = FVector::DotProduct(OwnerForward, OwnerToTargetNormalized);
	const float AngleDiff = UKismetMathLibrary::DegAcos(DotResult);

	return AngleDiff <= AnglePrecision;
}