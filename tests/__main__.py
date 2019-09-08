#!/usr/bin/env python
"""
Runs all the tests for every type of gym environment.

Tests for the screen environment will only be run if
the screen environment was registered in the gym registry
"""
from gym import envs
import unittest

from tests.grid_env_test import GridGymTest
# from tests.ram_env_test import RamGymTest

# only test the screen environment if its available
if "agario-screen-v0" in [env_spec.id for env_spec in envs.registry.all()]:
    from tests.screen_env_test import ScreenGymTest

if __name__ == "__main__":
    unittest.main()
