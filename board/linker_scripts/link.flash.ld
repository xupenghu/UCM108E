/* not needed, but we need separate linker scripts anyway */
OUTPUT_ARCH(riscv)
/* required to correctly link newlib */
GROUP( -lc -lgloss -lgcc -lsupc++ )

SEARCH_DIR(.)
__DYNAMIC  =  0;

MEMORY
{
    uc_rom      : ORIGIN = 0x00000000, LENGTH = 0x3000  /*12K uncompressed code sys data, NO RODATA!!!!*/
    instrram    : ORIGIN = 0x00003000, LENGTH = 0x1fd000 /*1M -0x3000*/
    /*uc_rom      : ORIGIN = 0x00019000, LENGTH = 0x3000*/  /*12K uncompressed code sys data, NO RODATA!!!!*/
    /*instrram    : ORIGIN = 0x0001C000, LENGTH = 0x1e3e00*/ /*1M -0x3000*/
    datarom     : ORIGIN = 0x00200000, LENGTH = 0x100000
    /*shareram    : ORIGIN = 0x00300000, LENGTH = 0x400 */          /*1kB for data share*/
    /*dataram     : ORIGIN = 0x00300400, LENGTH = 0x3C800 */ /*d_sram ends at 256K 340000 */
    dataram     : ORIGIN = 0x00300000, LENGTH = 0x3CC00
    stack       : ORIGIN = 0x0033CC00, LENGTH = 0x2000  /* stack and  32ec00 is reserved for trace buffer */ 
}

/* Stack information variables */
_min_stack     = 0x2000;   /* 4K - minimum stack space to reserve */
_stack_len     = LENGTH(stack);
_stack_start   = ORIGIN(stack) + LENGTH(stack);
_rom_end       = ORIGIN(uc_rom) + LENGTH(uc_rom);
_rom_start     = ORIGIN(uc_rom);

/* We have to align each sector to word boundaries as our current s19->slm
 * conversion scripts are not able to handle non-word aligned sections. */

SECTIONS
{

    .vectors :
    {
         KEEP(*(.vectors));  
    } > uc_rom
    
    .boot : ALIGN(512)
    {
        *(.critical)
    } > uc_rom 
    
    .fastfunc :
    {
        . = ALIGN(4);
        *(.reset)
        *(.crt0);
        . += 80;
        . = ALIGN(2048);
        _stbl_end = .;
    } > uc_rom
    
/*functions called only once during startup */
    .init : 
    {
        . = ALIGN(4);
        *(.init)
    } > instrram

    .text : 
	{
        . = ALIGN(4);
        _stext = .;
        *(.pvt)
        *(.text)
        _etext  =  .;
        __CTOR_LIST__ = .;
        LONG((__CTOR_END__ - __CTOR_LIST__) / 4 - 2)
        *(.ctors)
        LONG(0)
        __CTOR_END__ = .;
        __DTOR_LIST__ = .;
        LONG((__DTOR_END__ - __DTOR_LIST__) / 4 - 2)
        *(.dtors)
        LONG(0)
        __DTOR_END__ = .;
        *(.lit)
        *(.shdata)
_endtext = .;
    }  > instrram

    /*--------------------------------------------------------------------*/
    /* Global constructor/destructor segement                             */
    /*--------------------------------------------------------------------*/

	/*
    .preinit_array     :
    {
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP (*(.preinit_array))
        PROVIDE_HIDDEN (__preinit_array_end = .);
    } > datarom
    */

	/*
    .init_array     :
    {
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array ))
        PROVIDE_HIDDEN (__init_array_end = .);
    } > datarom
    */
    
	/*
    .fini_array     :
    {
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP (*(SORT(.fini_array.*)))
        KEEP (*(.fini_array ))
        PROVIDE_HIDDEN (__fini_array_end = .);
    } > datarom
    */

    .rodata : 
	{
        . = ALIGN(4);
		KEEP (*(.dsp_data));
		/*KEEP (*(.rti_fn.*));*/
        *(.rodata);
        *(.rodata.*);
		*(.srodata.*);
		*(.srodata*);

		*(FalPartTable);
		
		/* section information for finsh shell */
		. = ALIGN(4);
		__fsymtab_start = .;
		KEEP(*(SORT(FSymTab)))
		__fsymtab_end = .;
		. = ALIGN(4);
		__vsymtab_start = .;
		KEEP(*(SORT(VSymTab)))
		__vsymtab_end = .;
		. = ALIGN(4);

		. = ALIGN(4);
		__rt_init_start = .;
		KEEP(*(SORT(.rti_fn*)))
		__rt_init_end = .;
		. = ALIGN(4);

		/* section information for modules */
		. = ALIGN(4);
		__rtmsymtab_start = .;
		KEEP(*(SORT(RTMSymTab)))
		__rtmsymtab_end = .;
    } > datarom

    /*
    .share :
    {
        _share_start = .;
        KEEP(*(.share));  
        _share_end = .;
    } > shareram
    */
    
    .shbss :
    {
        . = ALIGN(4);
        *(.shbss)
    } > dataram

    .data : 
	{
        . = ALIGN(4);
        sdata  =  .;
        _sdata  =  .;
        *(.data);
        *(.data.*);
        *(.sdata);
        *(.sdata*);
        edata  =  .;
        _edata  =  .;
    } > dataram

    .bss :
    {
        . = ALIGN(4);
        _bss_start = .;
        *(.bss)
        *(.bss.*)
        *(.sbss)
        *(.sbss.*)
        *(COMMON)
        _bss_end = .;
    } > dataram
    
    .heap : 
	{
        . = ALIGN(4);
    } > dataram 
    .guard : 
	{
        . = ALIGN(4);
        . = . + 16;
    } > dataram 
    
    /* ensure there is enough room for stack */
    .stack (NOLOAD): 
	{
        . = ALIGN(4);
        . = . + _min_stack ;
        . = ALIGN(4);
        stack = . ;
        _stack = . ;
		_sp = . ;
    } > stack

    .stab  0 (NOLOAD) :
    {
        [ .stab ]
    }

    .stabstr  0 (NOLOAD) :
    {
        [ .stabstr ]
    }

    .bss :
    {
        . = ALIGN(4);
        _end = .;
    } > dataram
}

