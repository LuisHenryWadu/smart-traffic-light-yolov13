#!/usr/bin/env python3
"""Send reconstructed traffic-light timing commands to the ESP32."""

from __future__ import annotations

import argparse
import socket
import sys
from dataclasses import dataclass

DEFAULT_PORT = 8080
DEFAULT_TIMEOUT = 5.0
SCENARIO_SECONDS = {"low": 20, "medium": 30, "high": 40}


@dataclass(frozen=True)
class PhaseCommand:
    direction: str
    duration_seconds: int

    def encode(self) -> bytes:
        duration_ms = self.duration_seconds * 1000
        return f"PHASE,{self.direction.upper()},{duration_ms}\n".encode("ascii")


def positive_int(value: str) -> int:
    parsed = int(value)
    if parsed <= 0:
        raise argparse.ArgumentTypeError("duration must be greater than zero")
    return parsed


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        description="Send a traffic-light phase command to the reconstructed ESP32 controller."
    )
    parser.add_argument("--host", required=True, help="ESP32 IPv4/hostname")
    parser.add_argument("--port", type=int, default=DEFAULT_PORT)
    parser.add_argument(
        "--direction",
        required=True,
        choices=("north", "east", "south", "west"),
    )

    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument("--duration", type=positive_int, metavar="SECONDS")
    group.add_argument("--scenario", choices=tuple(SCENARIO_SECONDS))

    parser.add_argument("--timeout", type=float, default=DEFAULT_TIMEOUT)
    parser.add_argument("--dry-run", action="store_true")
    return parser


def resolve_duration(args: argparse.Namespace) -> int:
    if args.duration is not None:
        return args.duration
    return SCENARIO_SECONDS[args.scenario]


def send_command(host: str, port: int, command: PhaseCommand, timeout: float) -> str:
    with socket.create_connection((host, port), timeout=timeout) as sock:
        sock.sendall(command.encode())
        response = sock.recv(256)
    return response.decode("ascii", errors="replace").strip()


def main() -> int:
    args = build_parser().parse_args()
    command = PhaseCommand(args.direction, resolve_duration(args))
    payload = command.encode().decode("ascii").rstrip("\n")

    if args.dry_run:
        print(payload)
        return 0

    try:
        response = send_command(args.host, args.port, command, args.timeout)
    except OSError as exc:
        print(f"Connection failed: {exc}", file=sys.stderr)
        return 1

    print(f"Sent: {payload}")
    print(f"ESP32: {response or '<no response>'}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
