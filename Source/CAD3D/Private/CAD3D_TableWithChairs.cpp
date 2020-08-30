// Fill out your copyright notice in the Description page of Project Settings.

#include "CAD3D_TableWithChairs.h"
#include "CAD3D_ChairPoolComponent.h"
#include "CAD3D_ChairComponent.h"

ACAD3D_TableWithChairs::ACAD3D_TableWithChairs()
{
	PrimaryActorTick.bCanEverTick = false;

    ChairPool = CreateDefaultSubobject<UCAD3D_ChairPoolComponent>(TEXT("ChairPool"));
    
    ChairMeshOrigin = UCAD3D_ChairComponent::GetChairMeshOrigin();
    ChairMeshExtent = UCAD3D_ChairComponent::GetChairMeshExtent();
    LengthChairWithMargins = (ChairMeshExtent.Y * 2) + (MarginChair * 2);

}

void ACAD3D_TableWithChairs::BeginPlay()
{
    Super::BeginPlay();
    UpdateChairs();
}

void ACAD3D_TableWithChairs::Update()
{
    Super::Update();

    UpdateChairs();
}

//#pragma optimize("", off)
void ACAD3D_TableWithChairs::UpdateChairs()
{
    if(!ChairPool)
    {
        return;
    }

    FVector TableTopBoundsMax = GetLocalBoundsMax();
    FVector TableTopBoundsMin = GetLocalBoundsMin();

    float TableLengthX = TableTopBoundsMax.X - TableTopBoundsMin.X;
    float TableLengthY = TableTopBoundsMax.Y - TableTopBoundsMin.Y;
    
    float AvailableTableLengthX = TableLengthX - ((TableLegMeshExtent.X * 2) * 2);
    float AvailableTableLengthY = TableLengthY - ((TableLegMeshExtent.Y * 2) * 2);
    
    int32 NumChairsAlongX = AvailableTableLengthX / LengthChairWithMargins;
    int32 NumChairsAlongY = AvailableTableLengthY / LengthChairWithMargins;

    float OffsetX = TableLegMeshExtent.X * 2;
    float OffsetY = TableLegMeshExtent.Y * 2;

    float ChairStrideX = AvailableTableLengthX / NumChairsAlongX;
    float ChairStrideY = AvailableTableLengthY / NumChairsAlongY;

    int32 IndexChair = 0;
    for(int32 i = 0; i < NumChairsAlongX; i++)
    {
        float CoordX = TableTopBoundsMin.X + OffsetX - (ChairStrideX / 2) + ((i + 1) * ChairStrideX);

        {
            UCAD3D_ChairComponent* Chair = GetChairAt(IndexChair);
            PlaceChair(Chair, FVector(CoordX, TableTopBoundsMin.Y, 0.0f), FRotator(0.0f, 90.0f, 0.0f));
            IndexChair++;
        }
        
        {
            UCAD3D_ChairComponent* Chair = GetChairAt(IndexChair);
            PlaceChair(Chair, FVector(CoordX, TableTopBoundsMax.Y, 0.0f), FRotator(0.0f, -90.0f, 0.0f));
            IndexChair++;
        }
    }

    for(int32 i=0; i < NumChairsAlongY; i++)
    {
        float CoordY = TableTopBoundsMin.Y + OffsetY - (ChairStrideY / 2) + ((i + 1) * ChairStrideY);

        {
            UCAD3D_ChairComponent* Chair = GetChairAt(IndexChair);
            PlaceChair(Chair, FVector(TableTopBoundsMin.X, CoordY, 0.0f), FRotator(0.0f, 0.0f, 0.0f));
            IndexChair++;
        }
        
        {
            UCAD3D_ChairComponent* Chair = GetChairAt(IndexChair);
            PlaceChair(Chair, FVector(TableTopBoundsMax.X, CoordY, 0.0f), FRotator(0.0f, 180.0f, 0.0f));
            IndexChair++;
        }

    }

    for(int32 i=ChairsInUse.Num()-1; i >= IndexChair; i--)
    {
        ChairPool->AddReusable(ChairsInUse[i]); 
        ChairsInUse.RemoveAt(i, 1, false);
    }
}
//#pragma optimize("", on)

UCAD3D_ChairComponent * ACAD3D_TableWithChairs::GetChairAt(int32 IndexChair)
{
    UCAD3D_ChairComponent* Chair = nullptr;

    if(ChairsInUse.Num() > IndexChair) 
    {
        Chair = ChairsInUse[IndexChair];
    }
    else
    {
        Chair = ChairPool->GetReusable();
        Chair->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
        ChairsInUse.Emplace(Chair);
    }

    return Chair;
}

void ACAD3D_TableWithChairs::PlaceChair(UCAD3D_ChairComponent* Chair, FVector NewLocation, FRotator NewRotation)
{
    if(Chair)
    {   
        Chair->SetRelativeLocation(NewLocation);
        Chair->SetRelativeRotation(NewRotation);
    }
}
