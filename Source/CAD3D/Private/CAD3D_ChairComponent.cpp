// Fill out your copyright notice in the Description page of Project Settings.

#include "CAD3D_ChairComponent.h"
#include "ConstructorHelpers.h"
#include "CAD3D_ChairComponent.h"

const FString UCAD3D_ChairComponent::ChairMeshPath = FString("/Game/Meshes/Chair.Chair");
const FString UCAD3D_ChairComponent::TableAndChairMaterialPath = FString(TEXT("/Game/Materials/CAD3D_MatTableAndChair.CAD3D_MatTableAndChair"));


UCAD3D_ChairComponent::UCAD3D_ChairComponent()
{
    ConstructorHelpers::FObjectFinder<UStaticMesh> ChairMesh(*ChairMeshPath);

    ConstructorHelpers::FObjectFinder<UMaterial> TableAndChairMaterial(*TableAndChairMaterialPath);

    if (ChairMesh.Succeeded() && TableAndChairMaterial.Succeeded())
    {
        SetStaticMesh(ChairMesh.Object);
        SetMaterial(0, TableAndChairMaterial.Object);

    }
    
}

void UCAD3D_ChairComponent::Enable()
{
    SetVisibility(true);
}

void UCAD3D_ChairComponent::Disable()
{
    SetVisibility(false);
}

FVector UCAD3D_ChairComponent::GetChairMeshOrigin()
{
    ConstructorHelpers::FObjectFinder<UStaticMesh> ChairMesh(*ChairMeshPath);

    if (ChairMesh.Succeeded())
    {
        return ChairMesh.Object->GetBounds().Origin;
    }

    return FVector();
}

FVector UCAD3D_ChairComponent::GetChairMeshExtent()
{
    ConstructorHelpers::FObjectFinder<UStaticMesh> ChairMesh(*ChairMeshPath);

    if (ChairMesh.Succeeded())
    {
        return ChairMesh.Object->GetBounds().BoxExtent;
    }

    return FVector();
}
