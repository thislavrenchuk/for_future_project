#include "BabyContainer.h"

// Sets default values
ABabyContainer::ABabyContainer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a root Scene Component
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
    
	// Create a Static Mesh Component & setup attachement to the RootComponent
	if (!StaticMeshComponent)
	{
		StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
		StaticMeshComponent->SetupAttachment(Root);
	}
	
}

// Called when the game starts or when spawned
void ABabyContainer::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner() == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("BabyContainer's GetOwner is empty")); 
	} else 
	{
		UE_LOG(LogTemp, Warning, TEXT("BabyContainer's GetOwner is populated"));
	}

}

// Called every frame
void ABabyContainer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}