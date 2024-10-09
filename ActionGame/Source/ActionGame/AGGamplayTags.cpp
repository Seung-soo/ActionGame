// Fill out your copyright notice in the Description page of Project Settings.


#include "AGGamplayTags.h"

namespace AGGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG(Input_Action_Move, "Input.Action.Move");
	UE_DEFINE_GAMEPLAY_TAG(Input_Action_Roll, "Input.Action.Roll");
	UE_DEFINE_GAMEPLAY_TAG(Input_Action_Block, "Input.Action.Block");
	UE_DEFINE_GAMEPLAY_TAG(Input_Action_WeakAttack, "Input.Action.WeakAttack");
	UE_DEFINE_GAMEPLAY_TAG(Input_Action_StrongAttack, "Input.Action.StrongAttack");
	
	UE_DEFINE_GAMEPLAY_TAG(Input_Camera_Look, "Input.Camera.Look");
	
	UE_DEFINE_GAMEPLAY_TAG(State_Movement_Idle, "State.Movement.Idle");
	UE_DEFINE_GAMEPLAY_TAG(State_Movement_Walk, "State.Movement.Walk");
	UE_DEFINE_GAMEPLAY_TAG(State_Movement_Run, "State.Movement.Run");

	UE_DEFINE_GAMEPLAY_TAG(DataTable_AttackPattern, "DataTable.AttackPattern");
	// UE_DEFINE_GAMEPLAY_TAG(Event_Montage_End, "Event.Montage.End");
	// UE_DEFINE_GAMEPLAY_TAG(Event_Montage_Attack, "Event.Montage.Attack");
	// UE_DEFINE_GAMEPLAY_TAG(Ability_Attack, "Ability.Attack");
}
