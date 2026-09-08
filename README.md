# SkillDash - Game Feature Plugin

> ⚠️ This plugin is built specifically for the [Bomber](https://github.com/JanSeliv/Bomber) project and is not transferable to other Unreal Engine projects.

---

## Overview

<div align="center">

![SkillDash](https://github.com/user-attachments/assets/966d9847-a21a-40bc-80d5-2aa7326202ea)

</div>

Dash Ability built in GAS that applies a velocity to the player upon pressing input, with a cooldown. When the ability goes on cooldown, a widget appears that shows the time remaining until the cooldown ends, and then disappears.

---

### Implementation highlights

- **Modular GFP architecture** - the ability is fully self-contained; Bomber's core code has no knowledge of SkillDash, making it easy to enable, disable, or remove without touching the core project
- **Event-driven activation** - triggered via gameplay tag through Bomber's custom global message subsystem, forwarded to the ASC via `HandleGameplayEvent`
- **Ping-compensated cooldown** - the cooldown duration gets reduced on the server by the player's RTT, so every player experiences the same cooldown regardless of their ping
- **Full lifecycle management** - everything is cleaned up correctly on GFP deactivation and re-granted on reactivation
- **Data asset driven configuration** - all tunable values live in a data asset, so balancing changes are editor-only with no recompile needed
- **Runtime tuning** - impulse strength and cooldown duration exposed via CVars for live adjustment without recompiling
- **DAL pattern** - data asset self-registers via `UDalSubsystem` and the plugin waits for it to be loaded before accessing it, preventing crashes regardless of load order or storage speed
- **`FFunctionPicker` input binding** - follows Bomber's data-driven input convention; the function called on input is configured in the input action asset, resulting in no `BindAction` calls in C++
- **Data registry widget integration** - the cooldown widget is registered via a data table and `DAL Add Data Registry Source` Game Feature Action, so it's displayed automatically without any manual `CreateWidget` calls
- **Multiplayer-ready** - ability, cooldown, and widget all function correctly across server and multiple clients, with server-authoritative cooldown application and ping compensation for consistent client-side responsiveness
- **Shipping build safe** - all cheat and debug code is wrapped in `#if !UE_BUILD_SHIPPING` preprocessor guards, so nothing extra goes into production builds
