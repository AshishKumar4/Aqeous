## Aqeous- An OS that runs efficiently

>__INSTALLER BUILD IS OBSOLETE NOW!!! MAIN KERNEL RECIDES IN SOURCE DIRECTORY!!!__

The ZIP File contains these two files:  
* _hdd.img_  
  
  Hard Disk Image with both, kernel and grub built in, as well as AFS filesystem also made.
* _HDD_Template.img_

  Template image with only Kernel and grub built in. Use when _hdd.img_ corrupts. Copy it and rename it as '_hdd.img_'
  Extract _hdd.img_ and put in same folder in order to run the OS. 
### A few extra stuff
_Qemu_Aqeous.bat_ file is used to run to Kernel using qemu emulator.  
If running first time, type "_init aqfs_" in Aqeous Command Line to initialize the AQFS Filesystem.  
_Qemu_Disk_Partitioner.bat_ is obsolete. _test.bat_ it too outdated.
### A final note
I know. The whole thing is a bit convoluted. We'll fix it. Just that I (a humble contributor) am currently bust with my exams. They will end by April, after which the pace should increase
