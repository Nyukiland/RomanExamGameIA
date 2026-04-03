#include "EXEnemyCPP.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "Components/SkeletalMeshComponent.h"
#include "TimerManager.h"
#include "Animation/AnimSequence.h"

AEXEnemyCPP::AEXEnemyCPP()
{
    PrimaryActorTick.bCanEverTick = true;
    CurrentState = EEnemyState::Idle;
    bIsAttacking = false;

    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = 420.f; 
    }
    
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AEXEnemyCPP::BeginPlay()
{
    Super::BeginPlay();
}

void AEXEnemyCPP::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsAttacking) return;

    HandleVisionAndState();
    ExecuteState();
}

void AEXEnemyCPP::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEXEnemyCPP::HandleVisionAndState()
{
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!PlayerPawn) return;

    FVector Start = GetActorLocation();
    FVector End = PlayerPawn->GetActorLocation();
    float DistanceToPlayer = FVector::Distance(Start, End);

    if (DistanceToPlayer > SightDistance)
    {
        CurrentState = EEnemyState::Idle;
        return;
    }

    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);

    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, QueryParams);

    if (bHit && HitResult.GetActor() == PlayerPawn)
    {
        if (DistanceToPlayer <= AttackDistance)
        {
            CurrentState = EEnemyState::Attack;
        }
        else
        {
            CurrentState = EEnemyState::Chase;
        }
    }
    else
    {
        CurrentState = EEnemyState::Idle;
    }
}

void AEXEnemyCPP::ExecuteState()
{
    switch (CurrentState)
    {
    case EEnemyState::Idle:
        if (GetController())
        {
            GetController()->StopMovement();
        }
        break;

    case EEnemyState::Chase:
    {
        APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
        if (PlayerPawn && GetController())
        {
            //missing my line
        }
        break;
    }

    case EEnemyState::Attack:
        PerformAttack();
        break;
    }
}

void AEXEnemyCPP::PerformAttack()
{
    bIsAttacking = true;
    
    if (GetController())
    {
        GetController()->StopMovement();
    }

    if (AttackAnimation && GetMesh())
    {
        GetMesh()->PlayAnimation(AttackAnimation, false);
    }

    GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &AEXEnemyCPP::EndAttack, 1.5f, false);
}

void AEXEnemyCPP::EndAttack()
{
    bIsAttacking = false;
    CurrentState = EEnemyState::Idle; 
    
    if (GetMesh())
    {
        GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
    }
}