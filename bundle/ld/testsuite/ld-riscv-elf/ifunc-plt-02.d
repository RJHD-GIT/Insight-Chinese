#...
Disassembly of section .plt:
#...
0+[0-9a-f]+ <(\*ABS\*\+0x[0-9a-f]+@plt|foo@plt|.plt)>:
#...
Disassembly of section .text:
#...
0+[0-9a-f]+ <foo_resolver>:
#...
0+[0-9a-f]+ <bar>:
.*:[ 	]+[0-9a-f]+[ 	]+auipc[ 	]+.*
.*:[ 	]+[0-9a-f]+[ 	]+(lw|ld)[ 	]+.*<(_GLOBAL_OFFSET_TABLE_.*|.*)>
.*:[ 	]+[0-9a-f]+[ 	]+auipc[ 	]+.*
.*:[ 	]+[0-9a-f]+[ 	]+(lw|ld)[ 	]+.*<(__DATA_BEGIN__.*|.*)>
.*:[ 	]+[0-9a-f]+[ 	]+auipc[ 	]+.*
.*:[ 	]+[0-9a-f]+[ 	]+addi[ 	]+.*<(\*ABS\*\+0x[0-9a-f]+@plt|foo@plt|.plt)>
.*:[ 	]+[0-9a-f]+[ 	]+auipc[ 	]+.*
.*:[ 	]+[0-9a-f]+[ 	]+jalr[ 	]+.*<(\*ABS\*\+0x[0-9a-f]+@plt|foo@plt|.plt)>
.*:[ 	]+[0-9a-f]+[ 	]+auipc[ 	]+.*
.*:[ 	]+[0-9a-f]+[ 	]+jalr[ 	]+.*<(\*ABS\*\+0x[0-9a-f]+@plt|foo@plt|.plt)>
#...
