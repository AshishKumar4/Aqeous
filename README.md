## Aqeous- An Orphan OS

>__Kernel Code resides in Kernel directory, FASM directory contains FASM source ported for AqeousOS, Custom C Library for user programs in LibC including a test program__

The ZIP File contains these two files:  
* _hdd.img_  
  
  Hard Disk Image with both, kernel and grub built in, as well as AFS filesystem also made.
* _HDD_Template.img_

  Template image with only Kernel and grub built in. Use when _hdd.img_ corrupts. Copy it and rename it as '_hdd.img_'
  Extract _hdd.img_ and put in same folder in order to run the OS. 
### To Compile: 
>_The OS_ -> _make_

>_The LibC_ -> __cd LibC__
            ->__make__ 
               
      => This would generate a crt0 file (For now crt0, crti etc all functionality is within this, sorry laziness :p )
      
      => Then Compile your test program against this library (crt0.o) and Transfer it to virtual hard disk (hdd.img) 
          using the AqFS Editor (Check my other repository).
          
      => Simply type '<filename>' in the Shell.
       
### A few extra stuff:
> To Run in Qemu, use command '__make qemu__'

> Optional: To Install/Reinstall the Aqfs2 FileSystem, type '__init aqfs2__' in console, 

hdd.img comes with preinstalled FS and files! Dont Reinstall Aqfs until necessary! 

### A final note:
I know. The whole thing is a bit convoluted. We'll fix it. Just that I (a humble contributor) am currently bust with my exams. They will end by April, after which the pace should increase
