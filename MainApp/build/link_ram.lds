OUTPUT_ARCH( "riscv" )

ENTRY( _start )

MEMORY
{
  flash (rxai!w) : ORIGIN = 0x14038220, LENGTH = 824K
  dram (wxa!ri) : ORIGIN = 0x18000000, LENGTH = 164K 
}

SECTIONS
{
  .text           :
  {
      KEEP (*(SORT_NONE(.init)))
      *(.text .text.*)
      *(.gnu.linkonce.t.*)
      PROVIDE( _iram_lma = . );
   } >flash AT>flash


  .iram         :
  {
    . = ALIGN(4);        
    PROVIDE( _iram_start = . );
    *(.iram.entry*);
    *(.iram*);
    *(.rela.*);
    . = ALIGN(4);
    PROVIDE( _iram_end = . );
  } >dram AT>flash

  .lalign         :
  {
    . = ALIGN(4);
    PROVIDE( _data_lma = . );
  } >flash AT>flash


  .data          :
  {
    PROVIDE( _data_start = . );
    *(.data .data.*)
    *(.gnu.linkonce.d.*)
    *(.srodata.cst16)
    *(.srodata.cst8)
    *(.srodata.cst4)
    *(.srodata.cst2)
    *(.srodata .srodata.*)
    *(.sdata .sdata.*)
    *(.gnu.linkonce.s.*)
    . = ALIGN(4);
    PROVIDE( _data_end = . );
  } >dram AT>flash

  .FLASH_align_4KB :
  {
    KEEP (*(SORT_NONE(.fini)))
    *(.rdata)
    *(.rodata .rodata.*)
    *(.gnu.linkonce.r.*)
    . = ALIGN(4);
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array))
    PROVIDE_HIDDEN (__preinit_array_end = .);
    . = ALIGN(4);
    *(.sdata2*);
    . = ALIGN(4);
    . = ALIGN(0x1000);
    PROVIDE(_Resource_Flash_addr = . );
  } >flash AT>flash

  .bss (NOLOAD) :
  {
    PROVIDE( _fbss = . );
    PROVIDE( __bss_start = . );
    *(.sbss*)
    *(.gnu.linkonce.sb.*)
    *(.bss .bss.*)
    *(.gnu.linkonce.b.*)
    *(COMMON)
    . = ALIGN(4);
     PROVIDE( __bss_end = . );
    . = ALIGN(0x400);
  } >dram


  heap_begin = 0x18050000; 
  heap_limit = (heap_begin + 0x1B0000);
  
}

