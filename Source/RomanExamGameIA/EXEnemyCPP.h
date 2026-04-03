// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EXEnemyCPP.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle,
	Chase,
	Attack
};

UCLASS(Blueprintable)
class ROMANEXAMGAMEIA_API AEXEnemyCPP : public ACharacter
{
	GENERATED_BODY()

public:
	AEXEnemyCPP();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	EEnemyState CurrentState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	class UAnimSequence* AttackAnimation;

	UPROPERTY(EditAnywhere, Category = "AI")
	float SightDistance = 1500.f;

	UPROPERTY(EditAnywhere, Category = "AI")
	float AttackDistance = 150.f;

public: 
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void HandleVisionAndState();
	void ExecuteState();
	void PerformAttack();
	void EndAttack();

	bool bIsAttacking;
	FTimerHandle AttackTimerHandle;
};