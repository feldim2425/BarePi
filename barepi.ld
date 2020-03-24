/**
 * BarePi Linker Script
 */

ENTRY(_start);

SECTIONS {

    .init : {
        *(.init)
    }

    .text : {
        /* Combine all program (text) segments into the .text section */
        *(.text*) 
    }

    /* This is apperently used for pre initialization constructors (C++) and functions (C) but I didn't find much info about the actual usage
     * Used the default linker script as reference "ld --verbose"
     *
     * Some info can also be found here: https://docs.oracle.com/cd/E23824_01/html/819-0690/chapter7-1.html
     */
    .preinit_array : {
        . = ALIGN(4);
        __preinit_array_start = .;
        *(.preinit_array)
        __preinit_array_end = .;
    }

    
    /* Used by by for C++ constructors and apperently __attribute__((constructor)) */
    .init_array : {
        . = ALIGN(4);
        __init_array_start = .;
        SORT(*(.init_array*))
        *(.init_array)
        __init_array_end = .;
    }

    /* Used by by for C++ destructors and apperently __attribute__((destructor)) */
    .fini_array : {
        . = ALIGN(4);
        __fini_array_start = .;
        *(.fini_array)
        SORT(*(.fini_array*))
        __fini_array_end = .;
    }

    /* not sure it .ctors and .dtors is required or .init_array and .fini_array replaces that. Many sources suggest it isn't needet anymore */

    .ARM.exidx : {
        . = ALIGN(4);
        __exidx_start = .;
        *(.ARM.exidx*)
        __exidx_end = .;
    }

    .eh_frame : {
        . = ALIGN(4);
        *(.eh_frame*)
    }

    .rodata : {
        . = ALIGN(4);
        /* Combine all constant data segments into the .rodata section*/
        *(.rodata*)
    }


    .data : {
        . = ALIGN(4);
        *(.data*)
    }

    .bss : {
        . = ALIGN(4);
        __bss_start = .;
        *(.bss*)
        *(COMMON)
        __bss_end = .;
    }
    
    _end = .;
}