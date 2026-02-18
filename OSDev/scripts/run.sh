#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"

ISO="build/myuos.iso"
if [[ ! -f "$ISO" ]]; then
  echo "ISO not found: $ISO"
  echo "Run: scripts/build.sh"
  exit 1
fi

qemu-system-x86_64 \
  -m 256M \
  -cdrom "$ISO" \
  -boot d \
  -no-reboot \
  -monitor none \
  -device usb-tablet
