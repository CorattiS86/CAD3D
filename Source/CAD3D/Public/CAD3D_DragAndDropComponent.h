// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "CAD3D_DragAndDropComponent.generated.h"

class CAD3D_IDragAndDropObserver;
/**
 * 
 */
UCLASS(Blueprintable)
class CAD3D_API UCAD3D_DragAndDropComponent : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public: 
    UCAD3D_DragAndDropComponent();

    virtual void Update();

    FORCEINLINE void SetDeltaMouseWorldSpaceThreshold(float NewThreshold) { DeltaMouseWorldSpaceThreshold = NewThreshold; }
    FORCEINLINE float GetDeltaMouseWorldSpaceThreshold() { return DeltaMouseWorldSpaceThreshold; }

    FORCEINLINE void SetDeltaMeshLocationMovement(float NewDelta) { DeltaMeshLocationMovement = NewDelta; }
    FORCEINLINE float GetDeltaMeshLocationMovement() { return DeltaMeshLocationMovement; }
    
    FORCEINLINE bool IsSelected() { return bSelected; }

    void SetMeshByReference(UStaticMesh* NewMesh);
    void SetMeshByPath(const FString& NewMeshPath);

    void SetMaterialByReference(int32 Index, UMaterial* NewMaterial);
    void SetMaterialByPath(int32 Index, const FString& NewMaterialPath);


public:
    static const FString DraggerMeshPath;
    static const FString DraggerMaterialPath;

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

private:
    
    bool bSelected;

    float DeltaMouseWorldSpaceThreshold;
    float DeltaMeshLocationMovement;

private:
    float LastMouseWorldSpaceLocationX;
    float LastMouseWorldSpaceLocationY;

    void SetMousePositionOverMeshLocation();

    APlayerController* MainPlayer;

    UFUNCTION()
    void OnClick(UPrimitiveComponent* pComponent, FKey ButtonPressed);
    
    UFUNCTION()
    void OnRelease(UPrimitiveComponent* pComponent, FKey ButtonPressed);

public:    
    FORCEINLINE void AddObserver(CAD3D_IDragAndDropObserver* NewObserver) { Observers.Emplace(NewObserver); }

private:
    TSet<CAD3D_IDragAndDropObserver*> Observers;

    void NotifyChangeAlongX(float DeltaX);
    void NotifyChangeAlongY(float DeltaY);
};