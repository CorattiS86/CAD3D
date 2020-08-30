// Fill out your copyright notice in the Description page of Project Settings.

#include "CAD3D_Table.h"

ACAD3D_Table::ACAD3D_Table()
{
    PrimaryActorTick.bCanEverTick = false;

    Draggers.Reset(NumDraggers);
    for(int32 i=0; i<NumDraggers; i++)
    {
        FString DraggerName = "Dragger_" + FString::FromInt(i);
        UCAD3D_DragAndDropComponent* NewDragAndDropComponent = CreateDefaultSubobject<UCAD3D_DragAndDropComponent>(FName(*DraggerName));
        NewDragAndDropComponent->SetupAttachment(RootComponent);

        DraggersMap.Emplace(NewDragAndDropComponent, (BOUNDS_XY)i);
        Draggers.Emplace(NewDragAndDropComponent);
    }
    UpdateDraggersLocation();
} 

void ACAD3D_Table::Update()
{
    Super::Update();

    UpdateDraggersLocation();
}

void ACAD3D_Table::UpdateDraggersLocation()
{
    FVector TableBoundsMax = GetLocalBoundsMax();
    FVector TableBoundsMin = GetLocalBoundsMin();

    float ZCoord = TableHeight;

    FVector DraggerLocations[] = {  FVector(TableBoundsMax.X, 
                                            TableBoundsMax.Y, 
                                            ZCoord),
                                    FVector(TableBoundsMax.X, 
                                            TableBoundsMin.Y, 
                                            ZCoord),
                                    FVector(TableBoundsMin.X, 
                                            TableBoundsMax.Y, 
                                            ZCoord),
                                    FVector(TableBoundsMin.X, 
                                            TableBoundsMin.Y, 
                                            ZCoord)};


    if(Draggers.Num() == NumDraggers)
    {
        for (int i = 0; i < NumDraggers; i++)
        {
            Draggers[i]->SetRelativeLocation(DraggerLocations[i]);
            Draggers[i]->AddObserver(this);
        }
    }
}

void ACAD3D_Table::OnChangeAlongX(UCAD3D_DragAndDropComponent * Dragger, float DeltaX)
{
    BOUNDS_XY DraggerBounds_XY = DraggersMap[Dragger];
    switch (DraggerBounds_XY)
    {
    case ACAD3D_Table::MAX_X_MAX_Y:
    case ACAD3D_Table::MAX_X_MIN_Y:
    {
        FVector NewLocalBoundsMax = this->GetLocalBoundsMax();
        NewLocalBoundsMax.X += DeltaX;
        UpdateLocalBoundsMax(NewLocalBoundsMax);
        break;
    }
    case ACAD3D_Table::MIN_X_MAX_Y:
    case ACAD3D_Table::MIN_X_MIN_Y:
    {
        FVector NewLocalBoundsMin = this->GetLocalBoundsMin();
        NewLocalBoundsMin.X += DeltaX;
        UpdateLocalBoundsMin(NewLocalBoundsMin);
        break;
    }
    default:
        break;
    }

}

void ACAD3D_Table::OnChangeAlongY(UCAD3D_DragAndDropComponent * Dragger, float DeltaY)
{
    BOUNDS_XY DraggerBounds_XY = DraggersMap[Dragger];
    switch (DraggerBounds_XY)
    {
    case ACAD3D_Table::MAX_X_MAX_Y:
    case ACAD3D_Table::MIN_X_MAX_Y:
    {
        FVector NewLocalBoundsMax = this->GetLocalBoundsMax();
        NewLocalBoundsMax.Y += DeltaY;
        UpdateLocalBoundsMax(NewLocalBoundsMax);
        break;
    }
    case ACAD3D_Table::MAX_X_MIN_Y:
    case ACAD3D_Table::MIN_X_MIN_Y:
    {
        FVector NewLocalBoundsMin = this->GetLocalBoundsMin();
        NewLocalBoundsMin.Y += DeltaY;
        UpdateLocalBoundsMin(NewLocalBoundsMin);
        break;
    }
    default:
        break;
    }
}


