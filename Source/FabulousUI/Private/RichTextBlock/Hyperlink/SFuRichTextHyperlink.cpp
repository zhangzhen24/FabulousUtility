// Fill out your copyright notice in the Description page of Project Settings.


#include "RichTextBlock/Hyperlink/SFuRichTextHyperlink.h"


void SFuRichTextHyperlink::Construct(const FArguments& InArgs, const TSharedRef<FSlateHyperlinkRun::FWidgetViewModel>& InViewModel)
{
	this->ViewModel = InViewModel;
	this->Href = InArgs._Href;
	this->TypeId = InArgs._TypeId;
	this->OnNavigate = InArgs._OnNavigate;

	check(InArgs._HyperlinkStyle);
	const FButtonStyle* UnderlineStyle = InArgs._UnderlineStyle != nullptr ? InArgs._UnderlineStyle : &InArgs._HyperlinkStyle->UnderlineStyle;
	const FTextBlockStyle* TextStyle = InArgs._TextStyle != nullptr ? InArgs._TextStyle : &InArgs._HyperlinkStyle->TextStyle;
	TAttribute<FMargin> Padding = InArgs._Padding.IsSet() ? InArgs._Padding : InArgs._HyperlinkStyle->Padding;


	SButton::Construct(
		SButton::FArguments()
		.Text(InArgs._Text)
		.ContentPadding(Padding)
		.ButtonStyle(UnderlineStyle)
		.TextStyle(TextStyle)
		.ForegroundColor(FSlateColor::UseForeground())
		.TextShapingMethod(InArgs._TextShapingMethod)
		.TextFlowDirection(InArgs._TextFlowDirection)
		.OnClicked(this, &SFuRichTextHyperlink::Hyperlink_OnClicked)
	);
}
