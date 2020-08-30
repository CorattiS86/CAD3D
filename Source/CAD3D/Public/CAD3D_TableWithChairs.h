// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CAD3D_TableBase.h"
#include "CAD3D_TableWithChairs.generated.h"

class UCAD3D_ChairComponent;
class UCAD3D_ChairPoolComponent;

/**
 * 
 */
UCLASS()
class CAD3D_API ACAD3D_TableWithChairs : public ACAD3D_TableBase
{
	GENERATED_BODY()
	
public: 
    ACAD3D_TableWithChairs();

protected:
    virtual void BeginPlay() override;

    virtual void Update() override;


private:
    	
    UCAD3D_ChairPoolComponent* ChairPool;
    TArray<UCAD3D_ChairComponent*> ChairsInUse;

    FVector ChairMeshOrigin;
    FVector ChairMeshExtent;
    
    const float MarginChair = 15.0f;
    float LengthChairWithMargins;

    void UpdateChairs();
    
    UCAD3D_ChairComponent* GetChairAt(int32 IndexChair);
    void PlaceChair(UCAD3D_ChairComponent* Chair, FVector NewLocation, FRotator NewRotation);
};
