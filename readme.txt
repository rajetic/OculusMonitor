Oculus Monitor
Copyright (C) 2020 Kojack (rajetic@gmail.com)

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
v0.2.0 - Recording, replay and export added
v0.2.1 - Added Collada export
v0.2.2 - Added raw and no dead zone values to the triggers and thumbsticks.
v0.2.3 - Added headset tracking (pos/orientation/vel) display. Added tracking origin toggle (eye or floor).

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


Recording/Replay
A new Playback panel has been added. All major properties (touch tracking and state, headset tracking, sensor poses) can now be recorded and played back. There 7 controls in this panel:
- Record : Start recording the state. Frames are captured based on your monitor framerate. Press record a second time to stop recording. Each time you start recording, it will wipe the previous recording.
- Play : Start replaying the recording. Most panels will show the replay data (not all data is captured per frame, such as headset resolution and serial number, since they don't change at runtime).
- Stop : Stop playing or recording and go back to live mode (live data is displayed).
- Pause : Pause the recording or playback.
- Export CSV : save the tracking data to a CSV file. You can open this in most spreadsheet applications like Excel.
- Export DAE : save the tracking data to a Collada DAE file. You can open this in Blender (and maybe other 3D software).
- Time Slider : This lets you scrub through the timeline.
