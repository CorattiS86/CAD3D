// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CAD3D_ChairPoolComponent.generated.h"

class UCAD3D_ChairComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CAD3D_API UCAD3D_ChairPoolComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCAD3D_ChairPoolComponent();

    UCAD3D_ChairComponent* GetReusable();
    void AddReusable(UCAD3D_ChairComponent* Reusable);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	 

    const int32 InitialPoolSize = 20;
    const int32 IncreasePoolSize = 4;
	TArray<UCAD3D_ChairComponent*> Pool;

private:
	bool IncreasePool(int32 IncreasePoolSize);
};