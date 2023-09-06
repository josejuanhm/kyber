/*AUTOMATICALLY GENERATED FILE*/

OUTPUT_ARCH( "riscv" )
MEMORY
{

 orca_timer : ORIGIN = 0xFFFE0000 , LENGTH = 0x10000
 ps7_ddr_0 : ORIGIN = 0x00100000 , LENGTH = 0x1ff00000
 ps7_ethernet_0 : ORIGIN = 0xE000B000 , LENGTH = 0x1000
 ps7_gpio_0 : ORIGIN = 0xE000A000 , LENGTH = 0x1000
 ps7_qspi_0 : ORIGIN = 0xE000D000 , LENGTH = 0x1000
 ps7_qspi_linear_0 : ORIGIN = 0xFC000000 , LENGTH = 0x1000000
 ps7_ram_0 : ORIGIN = 0x00000000 , LENGTH = 57K
 ps7_ram_1 : ORIGIN = 57K , LENGTH = 32K
 ps7_sd_0 : ORIGIN = 0xE0100000 , LENGTH = 0x1000
 ps7_uart_1 : ORIGIN = 0xE0001000 , LENGTH = 0x1000
 ps7_usb_0 : ORIGIN = 0xE0002000 , LENGTH = 0x1000
}
/*----------------------------------------------------------------------*/
/* Sections                                                             */
/*----------------------------------------------------------------------*/
ENTRY(_start)

SECTIONS
{

  /* text: test code section */
  . = 0x00000000;
  .text.init . :
  {
	  *(.text.init)
  }>ps7_ram_0
  .text . :
  {
    *(.text)
  }>ps7_ram_0

  /* data segment */
  .data : { *(.data) }>ps7_ram_1
  .rodata : { *(.rodata) }>ps7_ram_1

  .sdata : {
    _gp = . + 0x800;
    *(.srodata.cst16) *(.srodata.cst8) *(.srodata.cst4) *(.srodata.cst2) *(.srodata*)
    *(.sdata .sdata.* .gnu.linkonce.s.*)
  }>ps7_ram_1
  _bss_start = .;
  /* bss segment */
  .sbss : {
    *(.sbss .sbss.* .gnu.linkonce.sb.*)
    *(.scommon)
  }>ps7_ram_1
  .bss : { *(.bss) }>ps7_ram_1

  /* thread-local data segment */
  .tdata :
  {
    _tls_data = .;
    *(.tdata)
  }>ps7_ram_1
  .tbss :
  {
    *(.tbss)
  }>ps7_ram_1

  /* End of uninitalized data segement */
  _end = .;

  _end_of_memory = 128K;
}
