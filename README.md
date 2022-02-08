# stewart_platform_ue4

[![Wath the video]](misc/Images/SetwartPlatformVideo.mp4)


https://youtrack.jetbrains.com/issue/RIDER-45008

You've got number of options:

* Ignore whole "RiderLink" folder and let Rider install it for you.
* If you install it to Engine and you use Engine from Epic Games Store, that's it, it won't try to recompile RiderLink and use pre-built binaries.
* If you install it to Engine that you build from source, or you install it to Game project, your .gitignore would be:

      Plugins/Developer/RiderLink/Binaries
      Plugins/Developer/RiderLink/Intermediate

## Collision filtering: 
* https://www.unrealengine.com/en-US/blog/collision-filtering?sessionInvalidated=true
* https://forums.unrealengine.com/t/ability-to-disable-collision-between-specific-components/89929/11

## Good UE4 threading example
* https://benui.ca/unreal/frunnable-threads/

## Randomfloat with Seed
* https://forums.unrealengine.com/t/seeded-fmath-rand/346211/6