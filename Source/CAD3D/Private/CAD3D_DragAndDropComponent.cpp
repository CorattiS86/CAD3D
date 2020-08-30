// Fill out your copyright notice in the Description page of Project Settings.

#include "CAD3D_DragAndDropComponent.h"
#include "ConstructorHelpers.h"
#include "Engine.h"
#include "CAD3D_DragAndDropObserver.h"

const FString UCAD3D_DragAndDropComponent::DraggerMeshPath = FString("/Game/Meshes/DraggerCube.DraggerCube");
const FString UCAD3D_DragAndDropComponent::DraggerMaterialPath = FString("/Game/Materials/CAD3D_MatDragger.CAD3D_MatDragger");

UCAD3D_DragAndDropComponent::UCAD3D_DragAndDropComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    
    bSelected = false;

    DeltaMouseWorldSpaceThreshold = 0.15f;
    DeltaMeshLocationMovement = 20.0f;
    
    ConstructorHelpers::FObjectFinder<UStaticMesh> DraggerMesh(*DraggerMeshPath);
    
    ConstructorHelpers::FObjectFinder<UMaterial> DraggerMaterial(*DraggerMaterialPath);

    if( DraggerMesh.Succeeded() && DraggerMaterial.Succeeded())
    {
        SetStaticMesh(DraggerMesh.Object);
		SetMaterial(0, DraggerMaterial.Object);
        OnClicked.AddDynamic(this, &UCAD3D_DragAndDropComponent::OnClick);
        OnReleased.AddDynamic(this, &UCAD3D_DragAndDropComponent::OnRelease);      
    }

}

void UCAD3D_DragAndDropComponent::BeginPlay()
{
    Super::BeginPlay();

    if(GetWorld())
    {
        MainPlayer = GetWorld()->GetFirstPlayerController();
    }
}


void UCAD3D_DragAndDropComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if(bSelected)
    {
        Update();
    }
}

void UCAD3D_DragAndDropComponent::SetMousePositionOverMeshLocation()
{ 
    if(MainPlayer)
    {
        FVector MouseWorldSpaceLocation;
        FVector MouseWorldSpaceDirection;
        MainPlayer->DeprojectMousePositionToWorld(MouseWorldSpaceLocation, MouseWorldSpaceDirection);

        FVector MeshLocation = GetComponentLocation();
        FVector2D MeshScreenSpaceLocation;
        MainPlayer->ProjectWorldLocationToScreen(MeshLocation, MeshScreenSpaceLocation);
        MainPlayer->SetMouseLocation((int)MeshScreenSpaceLocation.X, (int)MeshScreenSpaceLocation.Y);

        MainPlayer->DeprojectMousePositionToWorld(MouseWorldSpaceLocation, MouseWorldSpaceDirection);
        LastMouseWorldSpaceLocationX = MouseWorldSpaceLocation.X;
        LastMouseWorldSpaceLocationY = MouseWorldSpaceLocation.Y;
    }
}

void UCAD3D_DragAndDropComponent::OnClick(UPrimitiveComponent * pComponent, FKey ButtonPressed)
{
    bSelected = true;
   
    if(MainPlayer)
    {
        FVector MeshLocation = GetComponentLocation();
        FVector2D MeshScreenSpaceLocation;
        MainPlayer->ProjectWorldLocationToScreen(MeshLocation, MeshScreenSpaceLocation);
        MainPlayer->SetMouseLocation((int)MeshScreenSpaceLocation.X, (int)MeshScreenSpaceLocation.Y);

        FVector MouseWorldSpaceLocation;
        FVector MouseWorldSpaceDirection;
        MainPlayer->DeprojectMousePositionToWorld(MouseWorldSpaceLocation, MouseWorldSpaceDirection);

        LastMouseWorldSpaceLocationX = MouseWorldSpaceLocation.X;
        LastMouseWorldSpaceLocationY = MouseWorldSpaceLocation.Y;
    }
}

void UCAD3D_DragAndDropComponent::OnRelease(UPrimitiveComponent * pComponent, FKey ButtonPressed)
{
    bSelected = false;
}

#pragma optimize("", off) 
void UCAD3D_DragAndDropComponent::Update()
{
    if (MainPlayer)
    {
        FVector MouseWorldSpaceLocation;
        FVector MouseWorldSpaceDirection;
        MainPlayer->DeprojectMousePositionToWorld(MouseWorldSpaceLocation, MouseWorldSpaceDirection);

        float DeltaWorldSpaceMouseX = MouseWorldSpaceLocation.X - LastMouseWorldSpaceLocationX;
        float DeltaWorldSpaceMouseY = MouseWorldSpaceLocation.Y - LastMouseWorldSpaceLocationY;
        
        if (FMath::Abs(DeltaWorldSpaceMouseX) > DeltaMouseWorldSpaceThreshold)
        {
            float DeltaX = FMath::Sign(DeltaWorldSpaceMouseX) * DeltaMeshLocationMovement;
            AddRelativeLocation(FVector(DeltaX, 0.0f, 0.0f));  
            NotifyChangeAlongX(DeltaX);
            SetMousePositionOverMeshLocation();
        }
        else if (FMath::Abs(DeltaWorldSpaceMouseY) > DeltaMouseWorldSpaceThreshold)
        {
            float DeltaY = FMath::Sign(DeltaWorldSpaceMouseY) * DeltaMeshLocationMovement;
            AddRelativeLocation(FVector(0.0f, DeltaY, 0.0f));
            NotifyChangeAlongY(DeltaY);
            SetMousePositionOverMeshLocation();
        }
    }

    
}
#pragma optimize("", on)
void UCAD3D_DragAndDropComponent::SetMeshByReference(UStaticMesh * NewMesh)
{
    if(!!NewMesh)
    {
        SetStaticMesh(NewMesh);
    }
}

void UCAD3D_DragAndDropComponent::SetMeshByPath(const FString & NewMeshPath)
{
     ConstructorHelpers::FObjectFinder<UStaticMesh> NewMesh(*NewMeshPath);
     if(NewMesh.Succeeded())
     {
         SetStaticMesh(NewMesh.Object);
     }
}

void UCAD3D_DragAndDropComponent::SetMaterialByReference(int32 Index, UMaterial * NewMaterial)
{
    if(!!NewMaterial)
    {
        SetMaterial(Index, NewMaterial);
    }
}

void UCAD3D_DragAndDropComponent::SetMaterialByPath(int32 Index, const FString & NewMaterialPath)
{
    ConstructorHelpers::FObjectFinder<UMaterial> NewMaterial(*NewMaterialPath);
    if(NewMaterial.Succeeded())
    {
        SetMaterial(Index, NewMaterial.Object);
    }
}

void UCAD3D_DragAndDropComponent::NotifyChangeAlongX(float DeltaX)
{
    for (auto It = Observers.CreateConstIterator(); It; ++It)
    {
        (*It)->OnChangeAlongX(this, DeltaX);
    }
}

void UCAD3D_DragAndDropComponent::NotifyChangeAlongY(float DeltaY)
{
    for (auto It = Observers.CreateConstIterator(); It; ++It)
    {
        (*It)->OnChangeAlongY(this, DeltaY);
    }
}