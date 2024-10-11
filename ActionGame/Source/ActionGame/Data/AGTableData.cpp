// Fill out your copyright notice in the Description page of Project Settings.


#include "AGTableData.h"

UDataTable* UAGTableData::FindDataTableByTag(const FGameplayTag& DataTableTag) const
{
	for (const FAGDataTableStruct& DataTableStruct : DataTables)
	{
		if (IsValid(DataTableStruct.DataTable) && DataTableTag == DataTableStruct.DataTableTag)
		{
			return DataTableStruct.DataTable;
		}
	}

	UE_LOG(LogTemp, Error, TEXT("Can't find Data Table for DataTableTag [%s]"), *DataTableTag.ToString());

	return nullptr;
}
