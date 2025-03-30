#include "Components/ExedreFastRenderTarget.h"


UExedreWidgetRenderTarget::UExedreWidgetRenderTarget(
	const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	FString Path = "Texture2D'/Game/UI/txt_LogoUE4.txt_LogoUE4'";
	static ConstructorHelpers::FObjectFinder<UTexture2D> Texture(*Path);
	DefaultTexture = Texture.Object;

	ImageBrush = FSlateBrush();
	ImageBrush.SetResourceObject(DefaultTexture);

	RenderingMaterial = nullptr;
}


void UExedreWidgetRenderTarget::SetRenderMaterial( UMaterialInterface* Material )
{
	if( Material != nullptr && RenderingMaterial != Material )
	{
		// Store new reference
		RenderingMaterial = Material;

		// Updating internal rendering brush
		ImageBrush.SetResourceObject(Material);
	}
}


TSharedRef<SWidget> UExedreWidgetRenderTarget::RebuildWidget()
{
	if( !WidgetParent.IsValid() )
	{
		WidgetParent = SNew(SImage).Image( &ImageBrush );
	}

	return WidgetParent.ToSharedRef();
}


void UExedreWidgetRenderTarget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	WidgetParent.Reset();
}