// Fill out your copyright notice in the Description page of Project Settings.


#include "ObserverPlayerHUD.h"

void AObserverPlayerHUD::DrawHUD()
{
	Super::DrawHUD();

	if (CrosshairTexture)
	{
		//找出画布的中心
		FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	
		//缩放纹理大小
		float TextureWidth = CrosshairTexture->GetSurfaceWidth()*0.02f;
		float TextureHeight = CrosshairTexture->GetSurfaceHeight()*0.02f;
		FVector2D TextureSize = FVector2D(TextureWidth, TextureHeight);
		//偏移纹理大小的一半，以便纹理中心与画布中心对齐
		FVector2D CrossHairDrawPosition(Center.X - (TextureWidth * 0.5), Center.Y - (TextureHeight * 0.5));

		//在中心点绘制十字准星

		FCanvasTileItem TileItem(CrossHairDrawPosition, CrosshairTexture->Resource, TextureSize, FLinearColor::White);
		TileItem.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(TileItem);
	
		
	}
}
