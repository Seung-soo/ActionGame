// Fill out your copyright notice in the Description page of Project Settings.


#include "AGGamplayTags.h"

namespace AGGameplayTags
{
	/**** Input ****/
	// Action
	UE_DEFINE_GAMEPLAY_TAG(Input_Action_Move, "Input.Action.Move");
	UE_DEFINE_GAMEPLAY_TAG(Input_Action_Roll, "Input.Action.Roll");
	UE_DEFINE_GAMEPLAY_TAG(Input_Action_Block, "Input.Action.Block");
	UE_DEFINE_GAMEPLAY_TAG(Input_Action_Attack_Light, "Input.Action.Attack.Light");
	UE_DEFINE_GAMEPLAY_TAG(Input_Action_Attack_Heavy, "Input.Action.Attack.Heavy");
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
	UE_DEFINE_GAMEPLAY_TAG(State_Movement_Block_Hit, "State.Movement.Block.Hit");

	/**** DataTable ****/
	UE_DEFINE_GAMEPLAY_TAG(DataTable_AttackPattern, "DataTable.AttackPattern");

	/**** Event ****/
	// Montage
	UE_DEFINE_GAMEPLAY_TAG(Event_Montage_SaveAttack, "Event.Montage.SaveAttack");
	UE_DEFINE_GAMEPLAY_TAG(Event_Montage_RollFinish, "Event.Montage.RollFinish");
	
	// Hit
	UE_DEFINE_GAMEPLAY_TAG(Event_Hit_Enemy, "Event.Hit.Enemy");

	/**** Ability ****/
	UE_DEFINE_GAMEPLAY_TAG(Ability_ComboAttack, "Ability.ComboAttack");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Roll, "Ability.Roll");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Hit, "Ability.Hit");

	/**** Animation ****/
	UE_DEFINE_GAMEPLAY_TAG(Animation_Attack_Light_First, "Animation.Attack.Light.First");
	UE_DEFINE_GAMEPLAY_TAG(Animation_Attack_Light_Second, "Animation.Attack.Light.Second");
	UE_DEFINE_GAMEPLAY_TAG(Animation_Attack_Light_Third, "Animation.Attack.Light.Third");
	UE_DEFINE_GAMEPLAY_TAG(Animation_Attack_Light_Fourth, "Animation.Attack.Light.Fourth");
	UE_DEFINE_GAMEPLAY_TAG(Animation_Attack_Heavy_First, "Animation.Attack.Heavy.First");
	UE_DEFINE_GAMEPLAY_TAG(Animation_Attack_Heavy_Second, "Animation.Attack.Heavy.Second");
}
