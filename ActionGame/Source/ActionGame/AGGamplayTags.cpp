// Fill out your copyright notice in the Description page of Project Settings.


#include "AGGamplayTags.h"

namespace AGGameplayTags
{
	/**** Input ****/
	// Action
	UE_DEFINE_GAMEPLAY_TAG(Input_Action_Move, "Input.Action.Move");
	UE_DEFINE_GAMEPLAY_TAG(Input_Action_Roll, "Input.Action.Roll");
	UE_DEFINE_GAMEPLAY_TAG(Input_Action_Block, "Input.Action.Block");
	UE_DEFINE_GAMEPLAY_TAG(Input_Action_LightAttack, "Input.Action.LightAttack");
	UE_DEFINE_GAMEPLAY_TAG(Input_Action_HeavyAttack, "Input.Action.HeavyAttack");
	// Camera
	UE_DEFINE_GAMEPLAY_TAG(Input_Camera_Look, "Input.Camera.Look");

	/**** State ****/
	// Movement
	UE_DEFINE_GAMEPLAY_TAG(State_Movement_Idle, "State.Movement.Idle");
	UE_DEFINE_GAMEPLAY_TAG(State_Movement_Walk, "State.Movement.Walk");
	UE_DEFINE_GAMEPLAY_TAG(State_Movement_Run, "State.Movement.Run");
	UE_DEFINE_GAMEPLAY_TAG(State_Movement_Block, "State.Movement.Block");
	UE_DEFINE_GAMEPLAY_TAG(State_Movement_Block_Attack, "State.Movement.Block.Attack");
	UE_DEFINE_GAMEPLAY_TAG(State_Movement_Block_Roll, "State.Movement.Block.Roll");

	/**** DataTable ****/
	UE_DEFINE_GAMEPLAY_TAG(DataTable_AttackPattern, "DataTable.AttackPattern");

	/**** Event ****/
	// Montage
	UE_DEFINE_GAMEPLAY_TAG(Event_Montage_SaveAttack, "Event.Montage.SaveAttack");
	UE_DEFINE_GAMEPLAY_TAG(Event_Montage_RollFinish, "Event.Montage.RollFinish");

	/**** Ability ****/
	UE_DEFINE_GAMEPLAY_TAG(Ability_ComboAttack, "Ability.ComboAttack");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Roll, "Ability.Roll");
}
