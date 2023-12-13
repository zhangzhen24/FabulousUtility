#pragma once

#include "FuUIActionBindingArguments.h"
#include "Engine/CancellableAsyncAction.h"
#include "Input/UIActionBindingHandle.h"
#include "FuAsyncAction_UIActionListenerByInputAction.generated.h"

class UCommonUserWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFuUIActionListenerByInputActionDelegate, const UInputAction*, InputAction);

UCLASS(DisplayName = "Fu UI Action Listener By Input Action Async Action")
class FABULOUSUI_API UFuAsyncAction_UIActionListenerByInputAction : public UCancellableAsyncAction
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	TWeakObjectPtr<UCommonUserWidget> Widget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	FFuUIActionBindingArguments ActionArguments;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (DisplayThumbnail = false))
	TArray<TObjectPtr<UInputAction>> InputActions;

	TArray<FUIActionBindingHandle> ActionHandles;

public:
	UPROPERTY(BlueprintAssignable, Category = "Fabulous Utility|UI Action Listener By Input Action Async Action")
	FFuUIActionListenerByInputActionDelegate OnActionExecuted;

public:
	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|UI Async Actions",
		DisplayName = "Listen For UI Action By Input Action", BlueprintInternalUseOnly, Meta = (DefaultToSelf = "InWidget"))
	static UFuAsyncAction_UIActionListenerByInputAction* ListenForUIActionByInputAction(
		UPARAM(DisplayName = "Widget") UCommonUserWidget* InWidget,
		UPARAM(DisplayName = "Input Action") UInputAction* InInputAction,
		UPARAM(DisplayName = "Action Arguments") FFuUIActionBindingArguments InActionArguments);

	UFUNCTION(BlueprintCallable, Category = "Fabulous Utility|UI Async Actions",
		DisplayName = "Listen For UI Actions By Input Actions", BlueprintInternalUseOnly, Meta = (DefaultToSelf = "InWidget"))
	static UFuAsyncAction_UIActionListenerByInputAction* ListenForUIActionsByInputActions(
		UPARAM(DisplayName = "Widget") UCommonUserWidget* InWidget,
		UPARAM(DisplayName = "Input Actions") TArray<UInputAction*> InInputActions,
		UPARAM(DisplayName = "Action Arguments") FFuUIActionBindingArguments InActionArguments);

public:
	virtual void Activate() override;

	virtual void SetReadyToDestroy() override;

	virtual bool ShouldBroadcastDelegates() const override;

private:
	void Widget_OnActionExecuted(TWeakObjectPtr<UInputAction> InputAction) const;
};