// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NativeGameplayTags.h"

namespace AGGameplayTags
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Action_Move);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Action_Roll);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Action_Block);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Action_WeakAttack);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Action_StrongAttack);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Camera_Look);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Movement_Idle);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Movement_Walk);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Movement_Run);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(DataTable_AttackPattern);
	// UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Montage_Attack);
	//
	// UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Attack);
	
}