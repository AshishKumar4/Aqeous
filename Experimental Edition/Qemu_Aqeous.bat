qemu-system-x86_64 -kernel aqeous.bin -smp 4 -m 4G -monitor stdio -drive file=hdd.img,if=none,id=hdd0 -device ich9-ahci,id=ahci -device ide-drive,drive=hdd0,bus=ahci.1
