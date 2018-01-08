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

>_The LibC_ -> _cd LibC_
               >_make_ 
       _=>_ This would generate a crt0 file (For now crt0, crti etc all functionality is within this, sorry laziness :p )
       _=>_ Then Compile your test program against this library (crt0.o) and Transfer it to virtual hard disk (hdd.img) 
          using the AqFS Editor (Check my other repository).
       _=>_ Simply type '<filename>' in the Shell.
       
### A few extra stuff
>_To Run in Qemu, use command '_make qemu_'
If running first time, type "_init aqfs_" in Aqeous Command Line to initialize the AQFS Filesystem.  
_Qemu_Disk_Partitioner.bat_ is obsolete. _test.bat_ it too outdated.
### A final note
I know. The whole thing is a bit convoluted. We'll fix it. Just that I (a humble contributor) am currently bust with my exams. They will end by April, after which the pace should increase
