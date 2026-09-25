/*
 * Hardcore NPC & Creature Scaling Module for AzerothCore
 * License: MIT
 */

#include "ScriptMgr.h"
#include "AllCreatureScript.h"
#include "UnitScript.h"
#include "WorldScript.h"
#include "Creature.h"
#include "Unit.h"
#include "Config.h"
#include "GameTime.h"

#include <cmath>
#include <ctime>

namespace
{
    bool   g_enabled        = true;
    float  g_healthMult     = 1.25f;
    float  g_damageMult     = 1.30f;
    int32  g_expansion      = 0;    // 0 = Vanilla/Classic (1-60), -1 = All expansions
    uint32 g_scheduleMode   = 1;    // 0 = Always, 1 = Night Only (Default), 2 = Day Only
    uint32 g_nightStartHour = 20;   // 8:00 PM
    uint32 g_nightEndHour   = 6;    // 6:00 AM
    float  g_fastClockSpeed = 0.0f; // 0.0 = server real time, 3.0 = fast 4h/4h cycle

    uint32 GetCurrentHour()
    {
        time_t now = GameTime::GetGameTime().count();

        if (g_fastClockSpeed > 0.0f)
        {
            double visualSeconds = std::fmod(static_cast<double>(now) * g_fastClockSpeed, 86400.0);
            if (visualSeconds < 0.0)
                visualSeconds += 86400.0;
            return static_cast<uint32>(visualSeconds / 3600.0) % 24;
        }

        tm localTm;
#if defined(_WIN32) || defined(_WIN64)
        localtime_s(&localTm, &now);
#else
        localtime_r(&now, &localTm);
#endif
        return static_cast<uint32>(localTm.tm_hour);
    }

    bool IsScheduleActive()
    {
        if (g_scheduleMode == 0) // Always active
            return true;

        uint32 hour = GetCurrentHour();
        bool isNight = false;
        if (g_nightStartHour > g_nightEndHour)
            isNight = (hour >= g_nightStartHour || hour < g_nightEndHour);
        else
            isNight = (hour >= g_nightStartHour && hour < g_nightEndHour);

        if (g_scheduleMode == 1) // Night only
            return isNight;
        if (g_scheduleMode == 2) // Day only
            return !isNight;

        return true;
    }

    bool IsTargetCreature(Creature* creature)
    {
        if (!creature)
            return false;
        if (creature->IsPet() || creature->IsTotem() || creature->IsControlledByPlayer())
            return false;

        CreatureTemplate const* templateData = creature->GetCreatureTemplate();
        if (!templateData)
            return false;

        if (g_expansion >= 0 && templateData->expansion != uint8(g_expansion))
            return false;

        return true;
    }
}

class HardcoreScaling_WorldScript : public WorldScript
{
public:
    HardcoreScaling_WorldScript() : WorldScript("HardcoreScaling_WorldScript") { }

    void OnAfterConfigLoad(bool /*reload*/) override
    {
        g_enabled        = sConfigMgr->GetOption<bool>("HardcoreScaling.Enable", true, false);
        g_healthMult     = sConfigMgr->GetOption<float>("HardcoreScaling.HealthMultiplier", 1.25f, false);
        g_damageMult     = sConfigMgr->GetOption<float>("HardcoreScaling.DamageMultiplier", 1.30f, false);
        g_expansion      = sConfigMgr->GetOption<int32>("HardcoreScaling.Expansion", 0, false);
        g_scheduleMode   = sConfigMgr->GetOption<uint32>("HardcoreScaling.ScheduleMode", 1, false);
        g_nightStartHour = sConfigMgr->GetOption<uint32>("HardcoreScaling.NightStartHour", 20, false);
        g_nightEndHour   = sConfigMgr->GetOption<uint32>("HardcoreScaling.NightEndHour", 6, false);
        g_fastClockSpeed = sConfigMgr->GetOption<float>("HardcoreScaling.FastClockSpeed", 0.0f, false);
    }
};

class HardcoreScaling_AllCreatureScript : public AllCreatureScript
{
public:
    HardcoreScaling_AllCreatureScript() : AllCreatureScript("HardcoreScaling_AllCreatureScript") { }

    void OnCreatureAddWorld(Creature* creature) override
    {
        if (!g_enabled || g_healthMult <= 1.0f || !IsScheduleActive())
            return;
        if (!IsTargetCreature(creature))
            return;

        uint32 baseHealth = creature->GetCreateHealth();
        if (baseHealth == 0)
            return;

        uint32 newMax = uint32(baseHealth * g_healthMult);
        creature->SetMaxHealth(newMax);
        creature->SetHealth(newMax);
    }
};

class HardcoreScaling_UnitScript : public UnitScript
{
public:
    HardcoreScaling_UnitScript()
        : UnitScript("HardcoreScaling_UnitScript", true, { UNITHOOK_ON_DAMAGE }) { }

    void OnDamage(Unit* attacker, Unit* /*victim*/, uint32& damage) override
    {
        if (!g_enabled || g_damageMult <= 1.0f || !attacker || !IsScheduleActive())
            return;
        if (!IsTargetCreature(attacker->ToCreature()))
            return;

        damage = uint32(damage * g_damageMult);
    }
};

void Addmod_hardcore_scalingScripts()
{
    new HardcoreScaling_WorldScript();
    new HardcoreScaling_AllCreatureScript();
    new HardcoreScaling_UnitScript();
}
