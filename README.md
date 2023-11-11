# RevengeChecksumGen
Recalculates the per-profile checksums and MC02 checksums in Burnout Revenge savegames for PlayStation 2 and Xbox 360. Drag and drop the save file or provide it as the only argument on the command line.

If you're on Xbox 360, use a tool like [Velocity](https://community.wemod.com/t/release-velocity-xbox360-cross-platform-file-manager-v0-1-0-0/2623) to extract and replace save files. The checksum generator doesn't handle STFS packages, nor is it intended to.

Saves for the original Xbox seem to have a different checksum (though I've only glanced at one), so it's unsupported for now. It also has something after the MC02 header that might be a token for online play or another hash. Hard to say without checking multiple samples.

This was originally made so the unlock bools for sponsor vehicles could be set. The Game Crazy car, for example, is not unlockable any other way on retail devices, barring the use of saves transferred from hacked consoles or emulators. However, there is much more that can be modded.
