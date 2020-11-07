########################################
##
## Project: Tone
## type:    Generator
## Author:  Eduardo Hopperdietzel
##
## Desc:    Tone and noise generator
##
########################################

TEMPLATE = subdirs
SUBDIRS = DSP \
          UI

# Compile UI first
UI.depends = DSP

