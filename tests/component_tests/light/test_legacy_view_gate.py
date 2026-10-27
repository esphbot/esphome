"""Tests for the gate that keeps get_view_internal() working for out-of-tree lights."""

from pathlib import Path
from types import SimpleNamespace

import pytest

from esphome import loader
from esphome.components import light


def test_in_tree_platform_is_not_out_of_tree() -> None:
    assert not light._is_out_of_tree_platform("esp32_rmt_led_strip")


def test_external_platform_is_out_of_tree(
    monkeypatch: pytest.MonkeyPatch, tmp_path: Path
) -> None:
    module_file = tmp_path / "my_strip" / "light.py"
    manifest = SimpleNamespace(module=SimpleNamespace(__file__=str(module_file)))
    monkeypatch.setattr(loader, "get_platform", lambda domain, platform: manifest)
    assert light._is_out_of_tree_platform("my_strip")


def test_unknown_platform_is_out_of_tree(monkeypatch: pytest.MonkeyPatch) -> None:
    monkeypatch.setattr(loader, "get_platform", lambda domain, platform: None)
    assert light._is_out_of_tree_platform("missing")
