# dyn_proj_m5stack

Unreal Engine ↔ M5Stack ATOM 3 system for dynamic projection mapping control.

- **Transport:** UDP over Wi‑Fi
- **Protocol:** Binary framed messages (debuggable header + optional CRC)
- **Unreal:** UE 5.7 (macOS)
- **Firmware:** MicroPython on M5Stack ATOM 3
- **Phase 1 goal:** ATOM 3 controls a stepper motor and reports angle from a magnetic rotary encoder (AS5600 or via RollerCAN Lite Unit).

## Repository layout

- `unreal/` — Unreal plugin (C++ + Blueprint-friendly component)
- `firmware/atom3-micropython/` — MicroPython firmware
- `shared/protocol/` — Protocol schema + test vectors
- `docs/` — setup, wiring, protocol docs

## Protocol

See:
- `docs/protocol/protocol.md`
- `docs/protocol/message_table.md`