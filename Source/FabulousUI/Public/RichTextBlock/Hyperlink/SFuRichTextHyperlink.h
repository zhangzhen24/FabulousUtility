// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Text/SlateHyperlinkRun.h"
#include "Widgets/Input/SRichTextHyperlink.h"

class FWidgetViewModel;

class SFuRichTextHyperlink : public SHyperlink
{
public:
	SLATE_BEGIN_ARGS(SFuRichTextHyperlink)
			: _Text()
			  , _Href()
			  , _TypeId()
			  , _HyperlinkStyle(&FCoreStyle::Get().GetWidgetStyle<FHyperlinkStyle>("Hyperlink"))
			  , _TextStyle(nullptr)
			  , _UnderlineStyle(nullptr)
			  , _Padding()
			  , _OnNavigate()
			  , _OnHoverd()
			  , _OnUnhoverd()
			  , _TextShapingMethod()
			  , _TextFlowDirection()
		{
		}

		SLATE_ATTRIBUTE(FText, Text)
		SLATE_ARGUMENT(FString, Href)
		SLATE_ARGUMENT(FString, TypeId)
		SLATE_STYLE_ARGUMENT(FHyperlinkStyle, HyperlinkStyle)
		SLATE_STYLE_ARGUMENT(FTextBlockStyle, TextStyle)
		SLATE_STYLE_ARGUMENT(FButtonStyle, UnderlineStyle)
		SLATE_ATTRIBUTE(FMargin, Padding)
		SLATE_EVENT(FSimpleDelegate, OnNavigate)
		SLATE_EVENT(FSimpleDelegate, OnHoverd)
		SLATE_EVENT(FSimpleDelegate, OnUnhoverd)
		SLATE_ARGUMENT(TOptional<ETextShapingMethod>, TextShapingMethod)
		SLATE_ARGUMENT(TOptional<ETextFlowDirection>, TextFlowDirection)
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs, const TSharedRef<FSlateHyperlinkRun::FWidgetViewModel>& InViewModel);

	virtual void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override
	{
		OnHoverd.ExecuteIfBound();
		SHyperlink::OnMouseEnter(MyGeometry, MouseEvent);
		ViewModel->SetIsHovered(true);
	}

	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override
	{
		OnUnhovered.ExecuteIfBound();
		SHyperlink::OnMouseLeave(MouseEvent);
		ViewModel->SetIsHovered(false);
	}

	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override
	{
		FReply Reply = SHyperlink::OnMouseButtonDown(MyGeometry, MouseEvent);
		ViewModel->SetIsPressed(IsPressed());
		return Reply;
	}

	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override
	{
		FReply Reply = SHyperlink::OnMouseButtonUp(MyGeometry, MouseEvent);
		ViewModel->SetIsPressed(IsPressed());
		return Reply;
	}

	virtual bool IsHovered() const
	{
		return ViewModel->IsHovered();
	}

	virtual bool IsPressed() const
	{
		return ViewModel->IsPressed();
	}

	FReply Hyperlink_OnClicked() const
	{
		OnNavigate.ExecuteIfBound();
		return FReply::Handled();
	}

private:
	FString Href;
	FString TypeId;
	TSharedPtr<FSlateHyperlinkRun::FWidgetViewModel> ViewModel;
	FSimpleDelegate OnHoverd;
	FSimpleDelegate OnUnhovered;
};
