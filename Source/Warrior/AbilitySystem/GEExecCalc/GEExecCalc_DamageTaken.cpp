// Groundhog & Drake All Rights Reserved.

#include "GEExecCalc_DamageTaken.h"
#include "Warrior/WarriorGameplayTags.h"
#include "Warrior/AbilitySystem/WarriorAttributeSet.h"
#include "Warrior/WarriorDebugHelper.h"

struct FWarriorDamageCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefencePower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageTaken);

	FWarriorDamageCapture()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWarriorAttributeSet, AttackPower, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWarriorAttributeSet, DefencePower, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWarriorAttributeSet, DamageTaken, Target, false);
	}
};

static const FWarriorDamageCapture& GetWarriorDamageCapture()
{
	static FWarriorDamageCapture WarriorDamageCapture;
	return WarriorDamageCapture;
}

UGEExecCalc_DamageTaken::UGEExecCalc_DamageTaken()
{
	// Slow way
	/*FProperty* AttackPowerProperty =
		FindFieldChecked<FProperty>(UWarriorAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UWarriorAttributeSet, AttackPower));

	FGameplayEffectAttributeCaptureDefinition AttackPowerCaptureDefinition(AttackPowerProperty,							  //
																		   EGameplayEffectAttributeCaptureSource::Source, //
																		   false);

	RelevantAttributesToCapture.Add(AttackPowerCaptureDefinition);*/

	RelevantAttributesToCapture.Add(GetWarriorDamageCapture().AttackPowerDef);
	RelevantAttributesToCapture.Add(GetWarriorDamageCapture().DefencePowerDef);
	RelevantAttributesToCapture.Add(GetWarriorDamageCapture().DamageTakenDef);
}

void UGEExecCalc_DamageTaken::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
													 FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();

	float SourceAttackPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetWarriorDamageCapture().AttackPowerDef, EvaluateParameters,
															   SourceAttackPower);

	Debug::Print("SourceAttackPower", SourceAttackPower);

	float TargetDefencePower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetWarriorDamageCapture().DefencePowerDef, EvaluateParameters,
															   TargetDefencePower);

	Debug::Print("TargetDefencePower", TargetDefencePower);

	float BaseDamage = 0.f;
	int32 UsedLightAttackComboCount = 0;
	int32 UsedHeavyAttackComboCount = 0;

	for (const auto& TagMagnitude : EffectSpec.SetByCallerTagMagnitudes) {
		if (TagMagnitude.Key.MatchesTagExact(WarriorGameplayTags::Shared_SetByCaller_BaseDamage)) {
			BaseDamage = TagMagnitude.Value;
			Debug::Print("BaseDamage", BaseDamage);
		}

		if (TagMagnitude.Key.MatchesTagExact(WarriorGameplayTags::Player_SetByCaller_AttackType_Light)) {
			UsedLightAttackComboCount = TagMagnitude.Value;
			Debug::Print("UsedLightAttackComboCount", UsedLightAttackComboCount);
		}

		if (TagMagnitude.Key.MatchesTagExact(WarriorGameplayTags::Player_SetByCaller_AttackType_Heavy)) {
			UsedHeavyAttackComboCount = TagMagnitude.Value;
			Debug::Print("UsedHeavyAttackComboCount", UsedHeavyAttackComboCount);
		}
	}

	if (UsedLightAttackComboCount) {
		const float DamageIncreasePercent = (UsedLightAttackComboCount - 1) * 0.05f + 1.f;
		BaseDamage *= DamageIncreasePercent;
	}

	if (UsedHeavyAttackComboCount) {
		const float DamageIncreasePercent = UsedHeavyAttackComboCount * 0.15f + 1.f;
		BaseDamage *= DamageIncreasePercent;
	}

	Debug::Print("ScaledBaseDamage", BaseDamage);

	const float FinalDamageDone = BaseDamage * SourceAttackPower / TargetDefencePower;
	if (FinalDamageDone > 0.f) {
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(GetWarriorDamageCapture().DamageTakenProperty, EGameplayModOp::Override, FinalDamageDone));
	}

	Debug::Print("FinalDamageDone", FinalDamageDone);
}