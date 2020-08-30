// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CAD3D_TableWithChairs.h"
#include "CAD3D_DragAndDropComponent.h"
#include "CAD3D_DragAndDropObserver.h"
#include "CAD3D_Table.generated.h"


class UCAD3D_DragAndDropComponent;

/**
 * 
 */
UCLASS()
class CAD3D_API ACAD3D_Table : public ACAD3D_TableWithChairs, public CAD3D_IDragAndDropObserver
{
	GENERATED_BODY()
	
public:
    ACAD3D_Table();

protected: 

    virtual void Update() override; 
	
private:

    enum BOUNDS_XY 
    { 
        MAX_X_MAX_Y, 
        MAX_X_MIN_Y, 
        MIN_X_MAX_Y, 
        MIN_X_MIN_Y 
    };

	const int32 NumDraggers = 4;
    TArray<UCAD3D_DragAndDropComponent*> Draggers;
    TMap< UCAD3D_DragAndDropComponent*, BOUNDS_XY> DraggersMap;

    void UpdateDraggersLocation();
     
    // Inherited via CTC_IDragAndDropObserver
    virtual void OnChangeAlongX(UCAD3D_DragAndDropComponent * Dragger, float DeltaX) override;
    virtual void OnChangeAlongY(UCAD3D_DragAndDropComponent * Dragger, float DeltaY) override;

};
