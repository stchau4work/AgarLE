#!/usr/bin/env python
"""
File: __init__.py
Date: 1/27/19 
Author: Jon Deaton (jdeaton@stanford.edu)
"""

from gym.envs.registration import register

register(id='agario-full',
         entry_point='gym_agario.envs:AgarioFull')

register(id='agario-screen',
         entry_point='gym_foo.envs:AgarioScreen')
