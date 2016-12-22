struc multiboot_info
	.flags		resd	1	; required
	.memoryLo	resd	1	; memory size. Present if flags[0] is set
	.memoryHi	resd	1
	.bootDevice	resd	1	; boot device. Present if flags[1] is set
	.cmdLine	resd	1	; kernel command line. Present if flags[2] is set
	.mods_count	resd	1	; number of modules loaded along with kernel. present if flags[3] is set
	.mods_addr	resd	1
	.syms0		resd	1	; symbol table info. present if flags[4] or flags[5] is set
	.syms1		resd	1
	.syms2		resd	1
	.mmap_length	resd	1	; memory map. Present if flags[6] is set
	.mmap_addr	resd	1
	.drives_length	resd	1	; phys address of first drive structure. present if flags[7] is set
	.drives_addr	resd	1
	.config_table	resd	1	; ROM configuation table. present if flags[8] is set
	.bootloader_name resd	1	; Bootloader name. present if flags[9] is set
	.apm_table	resd	1	; advanced power management (apm) table. present if flags[10] is set
	.vbe_control_info resd	1	; video bios extension (vbe). present if flags[11] is set
	.vbe_mode_info	resd	1
	.vbe_mode	resw	1
	.vbe_interface_seg resw	1
	.vbe_interface_off resw	1
	.vbe_interface_len resw	1
endstru
