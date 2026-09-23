"""Tests for the supports_heat_cool default resolved in climate_ir code generation."""

from __future__ import annotations

from collections.abc import Callable
from pathlib import Path
import re

import pytest


def _emitted_value(main_cpp: str) -> str:
    """Return the effective HEAT_COOL value; the C++ default (true) emits no call."""
    match = re.search(r"set_supports_heat_cool\((true|false)\)", main_cpp)
    return match.group(1) if match else "true"


@pytest.mark.parametrize(
    ("config", "expected"),
    [
        ("heat_and_cool.yaml", "true"),
        ("cool_only.yaml", "false"),
        ("heat_only.yaml", "false"),
        ("neither.yaml", "false"),
    ],
)
def test_default_requires_heat_and_cool(
    config: str,
    expected: str,
    generate_main: Callable[[str | Path], str],
    component_config_path: Callable[[str], Path],
) -> None:
    """Without the key, HEAT_COOL follows supports_heat and supports_cool."""
    main_cpp = generate_main(component_config_path(config))
    assert _emitted_value(main_cpp) == expected


@pytest.mark.parametrize(
    ("config", "expected"),
    [
        ("cool_only_override_on.yaml", "true"),
        ("heat_and_cool_override_off.yaml", "false"),
    ],
)
def test_explicit_key_overrides_default(
    config: str,
    expected: str,
    generate_main: Callable[[str | Path], str],
    component_config_path: Callable[[str], Path],
) -> None:
    """A cool-only unit can still offer HEAT_COOL, and a heat+cool unit can drop it."""
    main_cpp = generate_main(component_config_path(config))
    assert _emitted_value(main_cpp) == expected
