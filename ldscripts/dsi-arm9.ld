OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_dsi_start)

__ram_start     =   ORIGIN(ram);
__ram_end       =	ORIGIN(ram) + LENGTH(ram);
end = __ram_end;

/*__itcm_start    =   ORIGIN(itcm);
__itcm_end      =   ORIGIN(itcm) + LENGTH(itcm);
__dtcm_start    =   ORIGIN(dtcm);
__dtcm_end      =   ORIGIN(dtcm) + LENGTH(dtcm);*/

/* what is this? */
/* gbatek: The ARM9 Exception Vectors are located at FFFF0000h. The IRQ handler redirects to [DTCM+3FFCh]. */
__irq_flags	    =	__dtcm_end - 0x08;
__irq_vector	=	__dtcm_end - 0x04;

__sp_svc	    =	__dtcm_end - 0x100;
__sp_irq	    =	__sp_svc - 0x100;
__sp_usr	    =	__sp_irq - 0x100;

MEMORY {
	ram 	: ORIGIN = 0x02000000, LENGTH = 16M
	vectors	: ORIGIN = 0x01000000, LENGTH = 256
	itcm	: ORIGIN = 0x01000100, LENGTH = 32K - 256
	dtcm	: ORIGIN = 0x0b000000, LENGTH = 16K
}

PHDRS {
	main    PT_LOAD FLAGS(7);
	itcm    PT_LOAD FLAGS(7);
	dtcm    PT_LOAD FLAGS(7);
	vectors PT_LOAD FLAGS(7);
}

SECTIONS {
	.crt0 : {
		__text_start = .;
		KEEP (*(.crt0))
		. = ALIGN(4);
	} >ram :main = 0x00

	.init : {
		*(SORT_NONE(.init))
	} >ram :main

	.text : {
		KEEP (*(.text .text.* .stub .gnu.warning .gnu.linkonce.t* .glue_7 .glue_7t))
		. = ALIGN(4);
	} >ram :main

	.fini : {
		KEEP (*(.fini))
	} >ram :main =0xff

	__text_end = . ;

	.rodata : {
		*(.rodata .roda .rodata.* .gnu.linkonce.r*)
		*all.rodata*(*)
		SORT(CONSTRUCTORS)
		. = ALIGN(4);
	} >ram :main = 0xff

    /* ARM exception indices */
	.ARM.extab : { *(.ARM.extab* .gnu.linkonce.armextab.*) } >ram :main
 	__exidx_start = .;
	ARM.exidx   : { *(.ARM.exidx* .gnu.linkonce.armexidx.*) } >ram :main
 	__exidx_end = .;

	/*	Ensure the __preinit_array_start label is properly aligned.  We
		could instead move the label definition inside the section, but
		the linker would then create the section even if it turns out to
		be empty, which isn't pretty.  */

	. = ALIGN(32 / 8);

	PROVIDE (__preinit_array_start = .);
	.preinit_array : { KEEP (*(.preinit_array)) } >ram :main = 0xff
	PROVIDE (__preinit_array_end = .);
	PROVIDE (__init_array_start = .);
	.init_array : {
		KEEP (*(SORT(.init_array.*)))
		KEEP (*(.init_array))
	} >ram :main = 0xff
	PROVIDE (__init_array_end = .);
	PROVIDE (__fini_array_start = .);
	.fini_array : {
		KEEP (*(.fini_array))
		KEEP (*(SORT(.fini_array.*)))
	} >ram :main = 0xff

	PROVIDE (__fini_array_end = .);

	.ctors : {
	/*	gcc uses crtbegin.o to find the start of the constructors, so
		we make sure it is first.  Because this is a wildcard, it
		doesn't matter if the user does not actually link against
		crtbegin.o; the linker won't look for a file to match a
		wildcard.  The wildcard also means that it doesn't matter which
		directory crtbegin.o is in.  */
		KEEP (*crtbegin.o(.ctors))
		KEEP (*(EXCLUDE_FILE (*crtend.o) .ctors))
		KEEP (*(SORT(.ctors.*)))
		KEEP (*(.ctors))
		. = ALIGN(4);
	} >ram :main = 0xff

	.dtors : {
		KEEP (*crtbegin.o(.dtors))
		KEEP (*(EXCLUDE_FILE (*crtend.o) .dtors))
		KEEP (*(SORT(.dtors.*)))
		KEEP (*(.dtors))
		. = ALIGN(4);
	} >ram :main = 0xff

	.eh_frame : {
		KEEP (*(.eh_frame))
		. = ALIGN(4);
	} >ram :main = 0xff

	.gcc_except_table : {
		*(.gcc_except_table)
		. = ALIGN(4);
	} >ram :main = 0xff
	.jcr : { KEEP (*(.jcr)) } >ram :main = 0
	.got : { *(.got.plt) *(.got) *(.rel.got) } >ram :main = 0

	.ewram ALIGN(4) : {
		__ewram_start = ABSOLUTE(.) ;
		*(.ewram)
		*ewram.*(.text)
		. = ALIGN(4);   /* REQUIRED. LD is flaky without it. */
	} >ram :main = 0xff

	.data ALIGN(4) : {
		__data_start = ABSOLUTE(.);
		*(EXCLUDE_FILE(*.twl*) .data)
		*(EXCLUDE_FILE(*.twl*) .data.*)
		*(EXCLUDE_FILE(*.twl*) .gnu.linkonce.d*)
		CONSTRUCTORS
		. = ALIGN(4);
		__data_end = ABSOLUTE(.) ;
	} >ram :main = 0xff

	__bss_vma = . ;

	.dtcm : {
		__dtcm_base = LOADADDR(.dtcm);
		__dtcm_start = ABSOLUTE(.);
		*(.dtcm)
		*(.dtcm.*)
		. = ALIGN(4);
		__dtcm_end = ABSOLUTE(.);
	} >dtcm AT>ram :dtcm = 0xff

	.itcm : {
		__itcm_base = LOADADDR(.itcm);
		__itcm_start = ABSOLUTE(.);
		*(.itcm)
		*.itcm*(.text .stub .text.*)
		. = ALIGN(4);
		__itcm_end = ABSOLUTE(.);
	} >itcm AT>ram :itcm = 0xff

	.vectors : {
		__vectors_lma = LOADADDR(.vectors);
		__vectors_start = ABSOLUTE(.);
		KEEP(*(.vectors .vectors.*))
		. = ALIGN(4);
		__vectors_end = ABSOLUTE(.);
	} >vectors AT>ram :vectors = 0xff

	.sbss __dtcm_end (NOLOAD): {
		__sbss_start = ABSOLUTE(.);
		*(.sbss)
		. = ALIGN(4);    /* REQUIRED. LD is flaky without it. */
		__sbss_end = ABSOLUTE(.);
	} >dtcm :NONE

	.bss __bss_vma (NOLOAD): {
		__bss_start__ = ABSOLUTE(.);
		*(EXCLUDE_FILE(*.twl*) .dynbss)
		*(EXCLUDE_FILE(*.twl*) .gnu.linkonce.b*)
		*(EXCLUDE_FILE(*.twl*) .bss*)
		*(EXCLUDE_FILE(*.twl*) COMMON)
		. = ALIGN(4);    /* REQUIRED. LD is flaky without it. */
		__bss_end__ = ABSOLUTE(.) ;
		__end__ = ABSOLUTE(.) ;
	} >ram :NONE

	/* Stabs debugging sections.  */
	.stab 0 : { *(.stab) }
	.stabstr 0 : { *(.stabstr) }
	.stab.excl 0 : { *(.stab.excl) }
	.stab.exclstr 0 : { *(.stab.exclstr) }
	.stab.index 0 : { *(.stab.index) }
	.stab.indexstr 0 : { *(.stab.indexstr) }
	.comment 0 : { *(.comment) }
	/*	DWARF debug sections.
		Symbols in the DWARF debugging sections are relative to the beginning
		of the section so we begin them at 0.  */
	/* DWARF 1 */
	.debug          0 : { *(.debug) }
	.line           0 : { *(.line) }
	/* GNU DWARF 1 extensions */
	.debug_srcinfo  0 : { *(.debug_srcinfo) }
	.debug_sfnames  0 : { *(.debug_sfnames) }
	/* DWARF 1.1 and DWARF 2 */
	.debug_aranges  0 : { *(.debug_aranges) }
	.debug_pubnames 0 : { *(.debug_pubnames) }
	/* DWARF 2 */
	.debug_info     0 : { *(.debug_info) }
	.debug_abbrev   0 : { *(.debug_abbrev) }
	.debug_line     0 : { *(.debug_line) }
	.debug_frame    0 : { *(.debug_frame) }
	.debug_str      0 : { *(.debug_str) }
	.debug_loc      0 : { *(.debug_loc) }
	.debug_macinfo  0 : { *(.debug_macinfo) }
	/* SGI/MIPS DWARF 2 extensions */
	.debug_weaknames 0 : { *(.debug_weaknames) }
	.debug_funcnames 0 : { *(.debug_funcnames) }
	.debug_typenames 0 : { *(.debug_typenames) }
	.debug_varnames  0 : { *(.debug_varnames) }
	.stack 0x80000 : { _stack = .; *(.stack) }
	/* These must appear regardless of  .  */
}
