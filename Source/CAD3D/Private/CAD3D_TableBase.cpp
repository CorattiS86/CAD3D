// Fill out your copyright notice in the Description page of Project Settings.

#include "CAD3D_TableBase.h"
#include "ConstructorHelpers.h"
#include "ProceduralMeshComponent.h"

const FString ACAD3D_TableBase::LegMeshPath = FString("/Game/Meshes/TableLeg.TableLeg");
const FString ACAD3D_TableBase::TableAndChairMaterialPath = FString(TEXT("/Game/Materials/CAD3D_MatTableAndChair.CAD3D_MatTableAndChair"));


ACAD3D_TableBase::ACAD3D_TableBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    LocalBoundsMax = FVector( 
        (TableSizeLowerLimitX / 2),  
        (TableSizeLowerLimitY / 2),  
        (TableSizeLowerLimitZ / 2));

    LocalBoundsMin = FVector(
        -(TableSizeLowerLimitX / 2), 
        -(TableSizeLowerLimitY / 2), 
        -(TableSizeLowerLimitZ / 2));

    TableOriginComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CTC_TableOrigin"));
    RootComponent = TableOriginComponent;

    // Init TableTop
    TableTopMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("CTC_TableMesh"));
    TableTopMesh->SetupAttachment(TableOriginComponent);
    CreateTableTopMesh();

	ConstructorHelpers::FObjectFinder<UMaterial> TableAndChairMaterial(*TableAndChairMaterialPath);
    if(TableAndChairMaterial.Succeeded())
    {
        TableTopMesh->SetMaterial(0, TableAndChairMaterial.Object);   
    }

    // Init TableLegs
    ConstructorHelpers::FObjectFinder<UStaticMesh> LegMesh(*LegMeshPath);
    if( LegMesh.Succeeded() && TableAndChairMaterial.Succeeded())
    {
        TableLegMeshOrigin = LegMesh.Object->GetBounds().Origin;
        TableLegMeshExtent = LegMesh.Object->GetBounds().BoxExtent;
        
        TableLegMeshes.Reset(NumLegs);
        for(int32 i=0; i<NumLegs; i++)
        {
            FString LegMeshName = "TableLegMesh_" + FString::FromInt(i);
            UStaticMeshComponent* NewMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName(*LegMeshName));
            NewMeshComponent->SetStaticMesh(LegMesh.Object);
            NewMeshComponent->SetMaterial(0, TableAndChairMaterial.Object);
            NewMeshComponent->SetupAttachment(TableOriginComponent);
            
            TableLegMeshes.Emplace(NewMeshComponent);
        }
        UpdateTableLegLocations();
    }
}

void ACAD3D_TableBase::UpdateLocalBoundsMax(FVector NewLocalBoundsMax)
{
    if ((NewLocalBoundsMax.X - LocalBoundsMin.X) < TableSizeLowerLimitX)
    {
        NewLocalBoundsMax.X = LocalBoundsMin.X + TableSizeLowerLimitX;
    }
    if ((NewLocalBoundsMax.Y - LocalBoundsMin.Y) < TableSizeLowerLimitY)
    {
        NewLocalBoundsMax.Y = LocalBoundsMin.Y + TableSizeLowerLimitY;
    }
    LocalBoundsMax = NewLocalBoundsMax;

    UpdateTableTopMesh();
    UpdateTableLegLocations();
    Update();
}

void ACAD3D_TableBase::UpdateLocalBoundsMin(FVector NewLocalBoundsMin)
{
    if ((LocalBoundsMax.X - NewLocalBoundsMin.X) < TableSizeLowerLimitX)
    {
        NewLocalBoundsMin.X = LocalBoundsMax.X - TableSizeLowerLimitX;
    }
    if ((LocalBoundsMax.Y - NewLocalBoundsMin.Y) < TableSizeLowerLimitY)
    {
        NewLocalBoundsMin.Y = LocalBoundsMax.Y - TableSizeLowerLimitY;
    }
    LocalBoundsMin = NewLocalBoundsMin;

    UpdateTableTopMesh();
    UpdateTableLegLocations();
    Update();
} 

void ACAD3D_TableBase::Update()
{
} 

FVector ACAD3D_TableBase::GetLegMeshOrigin()
{
    ConstructorHelpers::FObjectFinder<UStaticMesh> LegMesh(*LegMeshPath);
    if (LegMesh.Succeeded())
    {        
        return LegMesh.Object->GetBounds().Origin;
    }

    return FVector();
}

FVector ACAD3D_TableBase::GetLegMeshExtent()
{
    ConstructorHelpers::FObjectFinder<UStaticMesh> LegMesh(*LegMeshPath);
    if (LegMesh.Succeeded())
    {
        return LegMesh.Object->GetBounds().BoxExtent;
    }
    return FVector();
}

void ACAD3D_TableBase::CreateTableTopMesh()
{
    if(!!TableTopMesh)
    {
        TableTopMeshVertexData.NumVertices = 8;

        TableTopMeshVertexData.
            VertexPositions = { FVector(LocalBoundsMin.X,  LocalBoundsMax.Y,  TableHeight + LocalBoundsMax.Z),
                                FVector(LocalBoundsMin.X,  LocalBoundsMin.Y,  TableHeight + LocalBoundsMax.Z),
                                FVector(LocalBoundsMin.X,  LocalBoundsMax.Y,  TableHeight + LocalBoundsMin.Z),
                                FVector(LocalBoundsMin.X,  LocalBoundsMin.Y,  TableHeight + LocalBoundsMin.Z),
                                FVector(LocalBoundsMax.X,  LocalBoundsMax.Y,  TableHeight + LocalBoundsMax.Z),
                                FVector(LocalBoundsMax.X,  LocalBoundsMin.Y,  TableHeight + LocalBoundsMax.Z),
                                FVector(LocalBoundsMax.X,  LocalBoundsMax.Y,  TableHeight + LocalBoundsMin.Z),
                                FVector(LocalBoundsMax.X,  LocalBoundsMin.Y,  TableHeight + LocalBoundsMin.Z)};

        TableTopMeshVertexData.
            VertexNormals = {   FVector(-1.0f,  1.0f,  1.0f),
                                FVector(-1.0f, -1.0f,  1.0f),
                                FVector(-1.0f,  1.0f, -1.0f),
                                FVector(-1.0f, -1.0f, -1.0f),
                                FVector( 1.0f,  1.0f,  1.0f),
                                FVector( 1.0f, -1.0f,  1.0f),
                                FVector( 1.0f,  1.0f, -1.0f),
                                FVector( 1.0f, -1.0f, -1.0f)};

        TableTopMeshVertexData.
            Triangles = {  0, 1, 2,
                           2, 1, 3,
                           4, 5, 0,
                           0, 5, 1,
                           6, 7, 4,
                           4, 7, 5,
                           2, 7, 6,
                           3, 7, 2,
                           4, 0, 6,
                           6, 0, 2,
                           1, 5, 3,
                           3, 5, 7,};

        TableTopMesh->CreateMeshSection_LinearColor(
            0, 
            TableTopMeshVertexData.VertexPositions, 
            TableTopMeshVertexData.Triangles, 
            TableTopMeshVertexData.VertexNormals, 
            TableTopMeshVertexData.VertexUVs, 
            TArray<FLinearColor>(), 
            TArray<FProcMeshTangent>(), 
            false 
        );
    }

}

void ACAD3D_TableBase::UpdateTableTopMesh()
{
    TableTopMeshVertexData.
            VertexPositions = { FVector(LocalBoundsMin.X,  LocalBoundsMax.Y,  TableHeight + LocalBoundsMax.Z),
                                FVector(LocalBoundsMin.X,  LocalBoundsMin.Y,  TableHeight + LocalBoundsMax.Z),
                                FVector(LocalBoundsMin.X,  LocalBoundsMax.Y,  TableHeight + LocalBoundsMin.Z),
                                FVector(LocalBoundsMin.X,  LocalBoundsMin.Y,  TableHeight + LocalBoundsMin.Z),
                                FVector(LocalBoundsMax.X,  LocalBoundsMax.Y,  TableHeight + LocalBoundsMax.Z),
                                FVector(LocalBoundsMax.X,  LocalBoundsMin.Y,  TableHeight + LocalBoundsMax.Z),
                                FVector(LocalBoundsMax.X,  LocalBoundsMax.Y,  TableHeight + LocalBoundsMin.Z),
                                FVector(LocalBoundsMax.X,  LocalBoundsMin.Y,  TableHeight + LocalBoundsMin.Z)};

     TableTopMesh->UpdateMeshSection_LinearColor(
         0,
         TableTopMeshVertexData.VertexPositions,
         TArray<FVector>(),
         TArray<FVector2D>(), 
         TArray<FLinearColor>(),
         TArray<FProcMeshTangent>()
     );

}

void ACAD3D_TableBase::UpdateTableLegLocations()
{
    float ZCoord = TableHeight + LocalBoundsMin.Z - TableLegMeshExtent.Z;

    FVector LegLocations[] = {  FVector(LocalBoundsMax.X-TableLegMeshExtent.X, 
                                        LocalBoundsMax.Y-TableLegMeshExtent.Y, 
                                        ZCoord),
                                FVector(LocalBoundsMax.X-TableLegMeshExtent.X, 
                                        LocalBoundsMin.Y+TableLegMeshExtent.Y, 
                                        ZCoord),
                                FVector(LocalBoundsMin.X+TableLegMeshExtent.X, 
                                        LocalBoundsMax.Y-TableLegMeshExtent.Y, 
                                        ZCoord),
                                FVector(LocalBoundsMin.X+TableLegMeshExtent.X, 
                                        LocalBoundsMin.Y+TableLegMeshExtent.Y, 
                                        ZCoord)};

    if(TableLegMeshes.Num() == NumLegs)
    {
        for (int i = 0; i < NumLegs; i++)
        {
            TableLegMeshes[i]->SetRelativeLocation(LegLocations[i]);
        }
    }
}

