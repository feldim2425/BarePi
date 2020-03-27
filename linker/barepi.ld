/**
 * BarePi Linker Script
 */


SECTIONS {

    . = __start_addr;
    .text : {
        /* Combine all program (text) segments into the .text section */
        KEEP(*(.text.boot))
        *(.text)
    }

    . = ALIGN(__page_size);
    /* This is apperently used for pre initialization constructors (C++) and functions (C) but I didn't find much info about the actual usage
     * Used the default linker script as reference "ld --verbose"
     *
     * Some info can also be found here: https://docs.oracle.com/cd/E23824_01/html/819-0690/chapter7-1.html
     */
    .preinit_array : {
        __preinit_array_start = .;
        *(.preinit_array)
        __preinit_array_end = .;
    }

    
    . = ALIGN(__page_size);
    /* Used by by for C++ constructors and apperently __attribute__((constructor)) */
    .init_array : {
        __init_array_start = .;
        *(.init_array*)
        __init_array_end = .;
    }

    . = ALIGN(__page_size);
    /* Used by by for C++ destructors and apperently __attribute__((destructor)) */
    .fini_array : {
        . = ALIGN(4);
        __fini_array_start = .;
        *(.fini_array*)
        __fini_array_end = .;
    }

    . = ALIGN(__page_size);
    /* not sure it .ctors and .dtors is required or .init_array and .fini_array replaces that. Many sources suggest it isn't needet anymore */

    .ARM.extab :
    {  
       *(.ARM.extab*)  
    }  

    .ARM.exidx :
    {
        __exidx_start = .;
        *(.ARM.exidx*)
        __exidx_end = .;
    }

    . = ALIGN(__page_size);
    .eh_frame : {
        *(.eh_frame*)
    }

    . = ALIGN(__page_size);
    .rodata : {
        /* Combine all constant data segments into the .rodata section*/
        *(.rodata*)
    }


    . = ALIGN(__page_size);
    .data : {
        *(.data*)
    }

    . = ALIGN(__page_size);
    .bss : {
        __bss_start = .;
        PROVIDE(__bss_start__ = .);
        *(.bss*)
        *(COMMON)
        __bss_end = .;
        PROVIDE(__bss_end__ = .);
    }
    
    _end = .;
}