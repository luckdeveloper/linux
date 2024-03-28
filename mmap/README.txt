
1.测试方法
插入内核模块后，自己创建设备文件 /dev/mmaptest

2. mmap_test_read 测试
运行mmap_test_read的两个实例，可以发现两个mmap_test_read进程mmap得到的物理地址相同

2.1 运行mmap_test_read的第一个实例
➜  ./mmap_test_read            
[test 1    ---- access address: 74cbec69c000]
[test 2    ---- access address: 74cbec69d000]
[test 3    ---- access address: 74cbec69e000]
[test 4    ---- access address: 74cbec69f000]
[test 5    ---- access address: 74cbec6a0000]
[test 6    ---- access address: 74cbec6a1000]
[test 7    ---- access address: 74cbec6a2000]
[test 8    ---- access address: 74cbec6a3000]
[test 9    ---- access address: 74cbec6a4000]
[test 10   ---- access address: 74cbec6a5000]

内核模块记录的日志：
[17559.693386] MMAPTEST: process: mmap_test_read (3523)
[17559.693415] map_fault: map 0xffffa20d81879000 (0x000000002cb29000) to 0x74cbec69c000 , size: 0x1000, page:0 
[17559.693478] map_fault: map 0xffffa20d8187a000 (0x000000000f408000) to 0x74cbec69d000 , size: 0x1000, page:1 
[17559.693486] map_fault: map 0xffffa20d8187b000 (0x000000002ca62000) to 0x74cbec69e000 , size: 0x1000, page:2 
[17559.693492] map_fault: map 0xffffa20d8187c000 (0x0000000010035000) to 0x74cbec69f000 , size: 0x1000, page:3 
[17559.693497] map_fault: map 0xffffa20d8187d000 (0x000000002caee000) to 0x74cbec6a0000 , size: 0x1000, page:4 
[17559.693503] map_fault: map 0xffffa20d8187e000 (0x0000000007044000) to 0x74cbec6a1000 , size: 0x1000, page:5 
[17559.693509] map_fault: map 0xffffa20d8187f000 (0x0000000056474000) to 0x74cbec6a2000 , size: 0x1000, page:6 
[17559.693515] map_fault: map 0xffffa20d81880000 (0x000000000f5bd000) to 0x74cbec6a3000 , size: 0x1000, page:7 
[17559.693520] map_fault: map 0xffffa20d81881000 (0x0000000010123000) to 0x74cbec6a4000 , size: 0x1000, page:8 
[17559.693526] map_fault: map 0xffffa20d81882000 (0x00000000101a1000) to 0x74cbec6a5000 , size: 0x1000, page:9 

2.2 运行mmap_test_read的第二个实例
➜./mmap_test_read 
[test 1    ---- access address: 71c9bdd68000]
[test 2    ---- access address: 71c9bdd69000]
[test 3    ---- access address: 71c9bdd6a000]
[test 4    ---- access address: 71c9bdd6b000]
[test 5    ---- access address: 71c9bdd6c000]
[test 6    ---- access address: 71c9bdd6d000]
[test 7    ---- access address: 71c9bdd6e000]
[test 8    ---- access address: 71c9bdd6f000]
[test 9    ---- access address: 71c9bdd70000]
[test 10   ---- access address: 71c9bdd71000]

内核模块记录的日志：
[17651.842061] MMAPTEST: process: mmap_test_read (3611)
[17651.842074] map_fault: map 0xffffa20d81879000 (0x000000002cb29000) to 0x71c9bdd68000 , size: 0x1000, page:0 
[17651.842092] map_fault: map 0xffffa20d8187a000 (0x000000000f408000) to 0x71c9bdd69000 , size: 0x1000, page:1 
[17651.842094] map_fault: map 0xffffa20d8187b000 (0x000000002ca62000) to 0x71c9bdd6a000 , size: 0x1000, page:2 
[17651.842096] map_fault: map 0xffffa20d8187c000 (0x0000000010035000) to 0x71c9bdd6b000 , size: 0x1000, page:3 
[17651.842098] map_fault: map 0xffffa20d8187d000 (0x000000002caee000) to 0x71c9bdd6c000 , size: 0x1000, page:4 
[17651.842100] map_fault: map 0xffffa20d8187e000 (0x0000000007044000) to 0x71c9bdd6d000 , size: 0x1000, page:5 
[17651.842102] map_fault: map 0xffffa20d8187f000 (0x0000000056474000) to 0x71c9bdd6e000 , size: 0x1000, page:6 
[17651.842104] map_fault: map 0xffffa20d81880000 (0x000000000f5bd000) to 0x71c9bdd6f000 , size: 0x1000, page:7 
[17651.842106] map_fault: map 0xffffa20d81881000 (0x0000000010123000) to 0x71c9bdd70000 , size: 0x1000, page:8 
[17651.842108] map_fault: map 0xffffa20d81882000 (0x00000000101a1000) to 0x71c9bdd71000 , size: 0x1000, page:9 

3.再运行mmap_test_write的第一个实例，
可以看出，mmap_test_write和之前的两个mmap_test_read进程共享物理内存

内核模块记录的日志：
[20593.460003] MMAPTEST: process: mmap_test_write (4427)

[20593.460025] map_fault: map 0xffffa20d81879000 (0x000000002cb29000) to 0x716795745000 , size: 0x1000, page:0 
