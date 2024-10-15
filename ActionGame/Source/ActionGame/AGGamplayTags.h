// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NativeGameplayTags.h"

namespace AGGameplayTags
{
	/**** Input ****/
	// Action
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Action_Move);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Action_Roll);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Action_Block);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Action_Attack_Light);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Action_Attack_Heavy);
	
	// Camera
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Camera_Look);

	/**** State ****/
	// Movement
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Movement_Idle);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Movement_Walk);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Movement_Run);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Movement_Block);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Movement_Block_Attack);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Movement_Block_Roll);

	/**** DataTable ****/
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(DataTable_AttackPattern);

	/**** Event ****/
	// Montage
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Montage_SaveAttack);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Montage_RollFinish);

	// Hit
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Hit_Enemy);

	/**** Ability ****/
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ComboAttack);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Roll);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Hit);

	/**** Animation ****/
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Animation_Attack_Light_First);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Animation_Attack_Light_Second);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Animation_Attack_Light_Third);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Animation_Attack_Light_Fourth);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Animation_Attack_Heavy_First);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Animation_Attack_Heavy_Second);

	
}