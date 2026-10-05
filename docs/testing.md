# Testing Evidence

This document separates results recorded in the original 2025 report from behavior introduced by the 2026 portfolio reconstruction.

## Vehicle-Detection Examples

The report states that the detector's tracked count matched the visible number of vehicles in illustrated tests:

| Direction | Vehicles shown / reported |
| --- | ---: |
| North | 6 |
| East | 4 |
| South | 2 |

## Green-Light Duration Tests

### Low / Default
2 detected vehicles were categorized as low traffic and produced a 20-second green-light duration.

### Medium
5 detected vehicles were reported with a green-light duration of default + 10 seconds, producing 30 seconds.

### Busy / High-Duration
The busy/high-duration section shows 6 detected vehicles and reports 40 seconds (default + 20 seconds).

## Internal Inconsistencies in the Report

The source report is not fully consistent:

1. The medium test states that 5 vehicles produce 30 seconds.
2. The busy/high-duration test shows 6 vehicles and reports 40 seconds, while its wording also calls the condition medium.
3. The conclusion states that 4-6 vehicles are medium and says the green duration is 10 seconds, conflicting with the 30-second medium test.
4. The conclusion states that more than 6 vehicles are busy/high and receive default + 20 seconds.

Because of these conflicts, the reconstructed command sender does **not** infer duration from vehicle-count thresholds. It accepts explicit duration input, while optional `low`, `medium`, and `high` aliases map only to the 20/30/40-second durations shown in the testing section.

## Suggested Reconstruction Checks

- Run the Python client with `--dry-run`.
- Send short test durations to North, East, South, and West.
- Confirm all non-active approaches remain red.
- Confirm the selected direction transitions green -> yellow -> red.
- Send malformed commands and verify `ERR,INVALID_COMMAND`.

These are checks for the reconstructed implementation, not original 2025 test records.
