#pragma once
#include "Player.h"
#include "StaticPolygon.h"
#include "DirectionVolume.h"
#include "SoftBox.h"
#include "SlowShot.h"
#include "WebDrop.h"
#include "BoostPad.h"
#include "PowerUp.h"
#include "HUD.h"

static int id()
{
	static int x = 0;
	x++;
	return x;
}