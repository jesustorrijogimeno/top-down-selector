// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeMain.h"

#include "TopDownSelector/Pawns/AICharacter.h"

void AGameModeMain::UnSelectAllUnits()
{
    for (auto Unit : this->UnitsSelected)
    {
        CastChecked<AAICharacter>(Unit->GetPawn())->OnUnSelected();
    }
    this->UnitsSelected.Empty();
}
