# SimpleHazel
Base library for testing out IMGUI panels

## Building
Just run bootstrap-msvc.bat to create projects for visual studio using vcpkg as the package manager. Otherwise you can use cmake to make project files for whatever build system you use.

## Details

**Projects**
- DockingRangeSelectImGUI: The version of Dear IMGUI we will be basing our work on, it combines the Docking branch with the Range select feature.
- ForcePanel is an example of an ImGUI panel
- Hazel: the engine we are using as a test bed for panels

## New Panels
To add a new panel simply copy and rename the ForcePanel project to your working repo. You will also need to create a new layer for your panel inside Editor.cpp
