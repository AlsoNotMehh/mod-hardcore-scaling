# ![logo](https://raw.githubusercontent.com/azerothcore/azerothcore.github.io/master/images/logo-github.png) AzerothCore Module: mod-hardcore-scaling

[![AzerothCore Module](https://img.shields.io/badge/AzerothCore-Module-red?style=flat-square&logo=github)](https://github.com/azerothcore/azerothcore-wotlk)
[![C++20](https://img.shields.io/badge/Language-C++20-00599C?style=flat-square&logo=c%2B%2B)](https://isocpp.org/)
[![Branch 3.3.5a](https://img.shields.io/badge/Branch-3.3.5a-orange?style=flat-square)](https://github.com/azerothcore/azerothcore-wotlk)
[![License MIT](https://img.shields.io/badge/License-MIT-blue?style=flat-square)](LICENSE)
[![GitHub Stars](https://img.shields.io/github/stars/AlsoNotMehh/mod-hardcore-scaling?style=flat-square&color=yellow&logo=github)](https://github.com/AlsoNotMehh/mod-hardcore-scaling/stargazers)

A lightweight, high-performance creature health and damage scaling module for **AzerothCore (WotLK 3.3.5a)** designed for Hardcore realms and challenging PvE progression.

---

## 🔗 Companion Modules

This module works seamlessly with companion modules:

- 🌙 **[`mod-dangerous-nights`](https://github.com/AlsoNotMehh/mod-dangerous-nights):** Multiplies nighttime experience (+50%), gold drops (+50%), and bonus loot rolls to reward players facing scaled night monsters.
- 🌅 **[`mod-fast-day-night`](https://github.com/AlsoNotMehh/mod-fast-day-night):** Accelerates the day/night cycle (4h day / 4h night) so players frequently transition between calm daylight and perilous night scaling.

---

## ✨ Features

- ⚡ **Damage Scaling:** Increases creature attack damage (+30% by default).
- 🛡️ **Health Scaling:** Increases creature max health (+25% by default) derived cleanly from base creation health.
- ⏰ **Schedule Modes:** Configure scaling to activate **Night Only (Default)**, **Always Active**, or **Day Only**.
- 🔍 **Strict Exclusions:** Safely skips player pets, totems, and summoned guardians.
- 🗺️ **Expansion Filter:** Target Classic (1-60), TBC, Wrath, or all content.

---

## ⚙️ Configuration (`mod_hardcore_scaling.conf`)

| Setting | Default | Description |
| :--- | :---: | :--- |
| `HardcoreScaling.Enable` | `1` | Enables NPC health and damage scaling. |
| `HardcoreScaling.HealthMultiplier` | `1.25` | Health multiplier applied to eligible creatures (+25%). |
| `HardcoreScaling.DamageMultiplier` | `1.30` | Damage multiplier applied to all creature damage (+30%). |
| `HardcoreScaling.ScheduleMode` | `1` | `0` = Always, `1` = Night Only (Default), `2` = Day Only. |
| `HardcoreScaling.NightStartHour` | `20` | Hour when night begins (0–23, default 20 = 8:00 PM). |
| `HardcoreScaling.NightEndHour` | `6` | Hour when dawn arrives (0–23, default 6 = 6:00 AM). |
| `HardcoreScaling.FastClockSpeed` | `0.0` | Speed multiplier (`3.0` when used with `mod-fast-day-night`). |
| `HardcoreScaling.Expansion` | `0` | Target expansion (`0` = Vanilla 1-60, `1` = TBC, `2` = WotLK, `-1` = All). |

---

## 🛠️ Installation

1. Place the module in `azerothcore-wotlk/modules/`:
   ```bash
   cd azerothcore-wotlk/modules
   git clone https://github.com/AlsoNotMehh/mod-hardcore-scaling.git
   ```
2. Re-run CMake and compile your server:
   ```bash
   cmake -B build
   cmake --build build --config Release
   ```
3. Copy `conf/mod_hardcore_scaling.conf.dist` to your `worldserver` configs directory as `mod_hardcore_scaling.conf`.

---

## 📄 License

This project is licensed under the [MIT License](LICENSE).
