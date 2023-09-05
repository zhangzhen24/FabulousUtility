#include "Utility/FuEffectUtility.h"

#include "AbilitySystemComponent.h"
#include "FuMacros.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FuEffectUtility)

const FGameplayTagContainer& UFuEffectUtility::GetOwnedTags(const TSubclassOf<UGameplayEffect> EffectClass)
{
	if (FU_ENSURE(IsValid(EffectClass)))
	{
		return EffectClass.GetDefaultObject()->InheritableOwnedTagsContainer.CombinedTags;
	}

	static const FGameplayTagContainer None;
	check(None.IsEmpty())

	return None;
}

int32 UFuEffectUtility::GetEffectStackCountByClass(const UAbilitySystemComponent* AbilitySystem,
                                                   const TSubclassOf<UGameplayEffect> EffectClass)
{
	if (!FU_ENSURE(IsValid(AbilitySystem)) || !FU_ENSURE(IsValid(EffectClass)) ||
	    !FU_ENSURE(EffectClass.GetDefaultObject()->StackingType != EGameplayEffectStackingType::None))
	{
		return 0;
	}

	auto MaxCount{0};

	for (const auto& ActiveEffect : &AbilitySystem->GetActiveGameplayEffects())
	{
		// The backing gameplay effect class must be exactly the same. This is required for
		// compatibility with the UAbilitySystemComponent::GetGameplayEffectCount() function.

		if (ActiveEffect.Spec.Def->GetClass() == EffectClass)
		{
			MaxCount = FMath::Max(MaxCount, ActiveEffect.Spec.StackCount);
		}
	}

	return MaxCount;
}

bool UFuEffectUtility::HasActiveEffectsByQuery(const UAbilitySystemComponent* AbilitySystem, const FGameplayEffectQuery& EffectQuery)
{
	if (!FU_ENSURE(IsValid(AbilitySystem)) || !FU_ENSURE(!EffectQuery.IsEmpty()))
	{
		return false;
	}

	for (const auto& ActiveEffect : &AbilitySystem->GetActiveGameplayEffects())
	{
		if (EffectQuery.Matches(ActiveEffect))
		{
			return true;
		}
	}

	return false;
}

void UFuEffectUtility::FindActiveEffectsByQuery(const UAbilitySystemComponent* AbilitySystem,
                                                const FGameplayEffectQuery& EffectQuery, TArray<FActiveGameplayEffect>& ActiveEffects)
{
	ActiveEffects.Reset();

	if (!FU_ENSURE(IsValid(AbilitySystem)) || !FU_ENSURE(!EffectQuery.IsEmpty()))
	{
		return;
	}

	for (const auto& ActiveEffect : &AbilitySystem->GetActiveGameplayEffects())
	{
		if (EffectQuery.Matches(ActiveEffect))
		{
			ActiveEffects.Emplace(ActiveEffect);
		}
	}
}

bool UFuEffectUtility::HasActiveEffectsWithTag(const UAbilitySystemComponent* AbilitySystem,
                                               const FGameplayTag& Tag, const bool bIgnoreInhibitedEffects)
{
	if (!FU_ENSURE(IsValid(AbilitySystem)) || !FU_ENSURE(Tag.IsValid()))
	{
		return false;
	}

	for (const auto& ActiveEffect : &AbilitySystem->GetActiveGameplayEffects())
	{
		if (ActiveEffect.bIsInhibited && !bIgnoreInhibitedEffects)
		{
			continue;
		}

		if (ActiveEffect.Spec.Def->InheritableOwnedTagsContainer.CombinedTags.HasTag(Tag) ||
		    ActiveEffect.Spec.DynamicGrantedTags.HasTag(Tag))
		{
			return true;
		}
	}

	return false;
}

bool UFuEffectUtility::HasActiveEffectsWithAnyTags(const UAbilitySystemComponent* AbilitySystem,
                                                   const FGameplayTagContainer& Tags, const bool bIgnoreInhibitedEffects)
{
	if (!FU_ENSURE(IsValid(AbilitySystem)) || !FU_ENSURE(!Tags.IsEmpty()))
	{
		return false;
	}

	for (const auto& ActiveEffect : &AbilitySystem->GetActiveGameplayEffects())
	{
		if (ActiveEffect.bIsInhibited && !bIgnoreInhibitedEffects)
		{
			continue;
		}

		if (ActiveEffect.Spec.Def->InheritableOwnedTagsContainer.CombinedTags.HasAny(Tags) ||
		    ActiveEffect.Spec.DynamicGrantedTags.HasAny(Tags))
		{
			return true;
		}
	}

	return false;
}

int32 UFuEffectUtility::GetEffectsCountByTag(const UAbilitySystemComponent* AbilitySystem,
                                             const FGameplayTag& Tag, const bool bIgnoreInhibitedEffects)
{
	if (!FU_ENSURE(IsValid(AbilitySystem)) || !FU_ENSURE(Tag.IsValid()))
	{
		return false;
	}

	auto Count{0};

	for (const auto& ActiveEffect : &AbilitySystem->GetActiveGameplayEffects())
	{
		if (ActiveEffect.bIsInhibited && !bIgnoreInhibitedEffects)
		{
			continue;
		}

		if (ActiveEffect.Spec.Def->InheritableOwnedTagsContainer.CombinedTags.HasTag(Tag) ||
		    ActiveEffect.Spec.DynamicGrantedTags.HasTag(Tag))
		{
			Count += ActiveEffect.Spec.StackCount;
		}
	}

	return Count;
}

int32 UFuEffectUtility::GetEffectsCountWithAnyTags(const UAbilitySystemComponent* AbilitySystem,
                                                   const FGameplayTagContainer& Tags, const bool bIgnoreInhibitedEffects)
{
	if (!FU_ENSURE(IsValid(AbilitySystem)) || !FU_ENSURE(!Tags.IsEmpty()))
	{
		return 0;
	}

	auto Count{0};

	for (const auto& ActiveEffect : &AbilitySystem->GetActiveGameplayEffects())
	{
		if (ActiveEffect.bIsInhibited && !bIgnoreInhibitedEffects)
		{
			continue;
		}

		if (ActiveEffect.Spec.Def->InheritableOwnedTagsContainer.CombinedTags.HasAny(Tags) ||
		    ActiveEffect.Spec.DynamicGrantedTags.HasAny(Tags))
		{
			Count += ActiveEffect.Spec.StackCount;
		}
	}

	return Count;
}

const FActiveGameplayEffect* UFuEffectUtility::GetActiveEffectTimeRemainingAndDurationByTag(const UAbilitySystemComponent* AbilitySystem,
                                                                                            const FGameplayTag& Tag,
                                                                                            float& TimeRemaining, float& Duration)
{
	TimeRemaining = -1.0f;
	Duration = -1.0f;

	if (!FU_ENSURE(IsValid(AbilitySystem)))
	{
		return nullptr;
	}

	const auto& ActiveEffects{AbilitySystem->GetActiveGameplayEffects()};

	const FActiveGameplayEffect* Result{nullptr};
	const auto Time{ActiveEffects.GetWorldTime()};

	for (auto& ActiveEffect : &ActiveEffects)
	{
		if (!ActiveEffect.Spec.Def->InheritableOwnedTagsContainer.CombinedTags.HasTag(Tag) &&
		    !ActiveEffect.Spec.DynamicGrantedTags.HasTag(Tag))
		{
			continue;
		}

		const auto OtherDuration{ActiveEffect.GetDuration()};

		if (OtherDuration < 0.0f)
		{
			// Special case for infinite effects.

			Result = &ActiveEffect;

			TimeRemaining = FGameplayEffectConstants::INFINITE_DURATION;
			Duration = FGameplayEffectConstants::INFINITE_DURATION;
			break;
		}

		const auto OtherTimeRemaining{OtherDuration + ActiveEffect.StartWorldTime - Time};

		if (OtherTimeRemaining > TimeRemaining)
		{
			Result = &ActiveEffect;

			TimeRemaining = OtherTimeRemaining;
			Duration = OtherDuration;
		}
	}

	return Result;
}

bool UFuEffectUtility::TryGetEffectTimeRemainingAndDurationByTag(const UAbilitySystemComponent* AbilitySystem,
                                                                 const FGameplayTag& Tag, float& TimeRemaining, float& Duration)
{
	if (!IsValid(AbilitySystem))
	{
		TimeRemaining = -1.0f;
		Duration = -1.0f;
		return false;
	}

	return GetActiveEffectTimeRemainingAndDurationByTag(AbilitySystem, Tag, TimeRemaining, Duration) != nullptr;
}