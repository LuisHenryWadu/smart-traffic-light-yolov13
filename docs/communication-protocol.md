# Reconstructed Communication Protocol

The original project report states that the ESP32 receives commands from the laptop over Wi-Fi, but it does not preserve the exact transport layer, port number, packet syntax, or original source code.

This portfolio reconstruction therefore uses a minimal TCP line protocol.

## Transport

- Network: Wi-Fi
- Transport: TCP
- Default port: `8080`
- Encoding: ASCII
- Message delimiter: newline (`\n`)

## Command

```text
PHASE,<DIRECTION>,<GREEN_DURATION_MS>
```

Valid directions: `NORTH`, `EAST`, `SOUTH`, `WEST`.

Example:

```text
PHASE,NORTH,20000
```

## Reconstructed ESP32 Sequence

```text
all directions RED
        |
        | 1 s safety transition
        v
selected direction GREEN
        |
        | requested duration
        v
selected direction YELLOW
        |
        | 3 s transition
        v
all directions RED
```

The 1-second all-red and 3-second yellow values are reconstruction choices.

## Responses

Accepted:

```text
ACK,NORTH,20000
```

Invalid:

```text
ERR,INVALID_COMMAND
```
