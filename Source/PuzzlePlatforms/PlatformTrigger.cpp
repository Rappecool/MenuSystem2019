// Fill out your copyright notice in the Description page of Project Settings.

#include "PlatformTrigger.h"
#include "Components/BoxComponent.h"

#include "MovingPlatform.h"

// Sets default values
APlatformTrigger::APlatformTrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

		//creates TriggerVolume.
	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(FName("Trigger Volume"));

		//checks for nullptr in TriggerVolume.
	if (!ensure(TriggerVolume != nullptr))
	{
		return;
	}

	RootComponent = TriggerVolume;

}

// Called when the game starts or when spawned
void APlatformTrigger::BeginPlay()
{
	Super::BeginPlay();


	if (!ensure(TriggerVolume != nullptr))
	{
		return;
	}
	//onComponentBeginOverlap starts dynamic, delegate listening event. 
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &APlatformTrigger::OnOverlapBegin);

	//OnComponentEndOverlap ends dynamic, delegate listening event.
	TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &APlatformTrigger::OnOverlapEnd);
	
}

// Called every frame
void APlatformTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlatformTrigger::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
		//way to foreach loop through our TArray.

	for (AMovingPlatform* Platform : PlatformsToTrigger)
	{
		Platform->AddActiveTrigger();
	}
	

	UE_LOG(LogTemp, Warning, TEXT("Overlap activated."));
}

void APlatformTrigger::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	//way to foreach loop through our TArray.

	for (AMovingPlatform* Platform : PlatformsToTrigger)
	{
		Platform->RemoveActiveTrigger();
	}

	UE_LOG(LogTemp, Warning, TEXT("Overlap deactivated"));
}

