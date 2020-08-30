// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CAD3D_ChairComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class CAD3D_API UCAD3D_ChairComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:

    UCAD3D_ChairComponent();

    void Enable();
    void Disable();

public:

    static const FString ChairMeshPath;
    static const FString TableAndChairMaterialPath;

    static FVector GetChairMeshOrigin();
    static FVector GetChairMeshExtent();
};