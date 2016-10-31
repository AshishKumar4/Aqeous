# Aqeous
The ZIP File contains these two files-> 
hdd.img --> Hard Disk Image with both, kernel and grub built in, as well as AFS filesystem also made.
HDD_Template.img --> Template image with only Kernel and grub built in. Use when hdd.img corrupts. Copy it and rename it as 'hdd.img'
Extract hdd.img and put in same folder in order to run the OS. 

Qemu_Aqeous.bat file is used to run to Kernel using qemu emulator. 
If running first time, type "init aqfs" in Aqeous Command Line to initialize the AQFS Filesystem. 
Qemu_Disk_Partitioner.bat is obsolete. test.bat it too outdated.
