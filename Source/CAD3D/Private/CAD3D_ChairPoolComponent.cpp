// Fill out your copyright notice in the Description page of Project Settings.

#include "CAD3D_ChairPoolComponent.h"
#include "CAD3D_ChairComponent.h"

// Sets default values for this component's properties
UCAD3D_ChairPoolComponent::UCAD3D_ChairPoolComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


UCAD3D_ChairComponent* UCAD3D_ChairPoolComponent::GetReusable()
{
    if(!Pool.Num())
    {
        if(!IncreasePool(IncreasePoolSize))
        {
            return nullptr;
        }
    }
    
    UCAD3D_ChairComponent* Reusable = Pool.Pop(false);
    Reusable->Enable();
    return Reusable;
}

void UCAD3D_ChairPoolComponent::AddReusable(UCAD3D_ChairComponent * Reusable)
{
    Reusable->Disable();
    Pool.Emplace(Reusable);
} 

void UCAD3D_ChairPoolComponent::BeginPlay()
{
	Super::BeginPlay();
    IncreasePool(InitialPoolSize);
} 

bool UCAD3D_ChairPoolComponent::IncreasePool(int32 IncreasePoolSize)
{
    int32 CurrentPoolSize = Pool.GetSlack();
    
    Pool.Reset(CurrentPoolSize + IncreasePoolSize);  

    int32 NewPoolSize = Pool.GetSlack();

    if(Pool.GetSlack() == CurrentPoolSize)
    {   
        return false;
    }

    for(int32 i=CurrentPoolSize; i < NewPoolSize; i++)
    {
        FString ChairComponentName = "ChairComponent_" + FString::FromInt(i);
        UCAD3D_ChairComponent* NewChairComponent = NewObject<UCAD3D_ChairComponent>(this, FName(*ChairComponentName));
        NewChairComponent->RegisterComponent();
        NewChairComponent->Disable();

        Pool.Emplace(NewChairComponent);
    }

    return true;
}

