Oculus Monitor
Copyright (C) 2018 Kojack (rajetic@gmail.com)

Oculus Monitor is released under the MIT License  
https://opensource.org/licenses/MIT


Oculus Monitor is a tool for viewing the current state of various aspects of the Oculus VR environment.
Currently Oculus Monitor can show you:
Headset:
- Serial number
- Firmware
- Product name
- Tracking capability flags
- Panel resolution
- Oculus runtime version
- turn on any performance hud mode
- Pixel density to render resolution calculator

Controllers:
- Which controllers are active (left touch, right touch, remote, xbox, 4 VR objects)
- Every button on the remote
- Every button, touch state, analog axis and tracking data for both touch controllers

Room layout:
- Guardian outer boundary
- Guardian play boundary
- Touch and headset position and orientation
- World origin
- Sensor coverage

Change History
v0.1.0 - Initial release. Built against Oculus SDK 1.26

Installation
Oculus Monitor can be placed anywhere. No explicit installation is required.

Room Layout Display
The guardian boundary is shown as a white outline.
The play area is shown as a cyan outline.
Sensors are purple. Note, this is using leveled poses, which means it doesn't take into account sensors angled up or down. Rendering that in 2D in a meaningful way is tricky.
Touch controllers are white circles with a three axis orientation marker.
The headset is a yellow circle with a three axis orientation marker.
The current origin (when you reset the view) is a red circle with a three axis orientation marker.
The three axis orientation markers are red to the right, green upwards and blue forwards.

Note: Position tracking of Touch controllers doesn't work unless the headset proximity sensor is active (by wearing the headset, putting an object like a cloth on it, or forced on by the Oculus Debug Tool). The orientation of Touch controllers may also be wrong until they can be calibrated by the headset proximity sensor being active at least once (after that they are tracked with the IMU gyroscopes).
