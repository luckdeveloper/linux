
1.测试方法
插入内核模块后，自己创建设备文件 /dev/mmaptest

2. 输出
用户态程序 mmap_test_read 输出：
[root@linux mmap]# ./mmap_test_read 
[test 1    ----7fdba144a000]
[test 2    ----7fdba144b000]
[test 3    ----7fdba144c000]
[test 4    ----7fdba144d000]
[test 5    ----7fdba144e000]
[test 6    ----7fdba144f000]
[test 7    ----7fdba1450000]
[test 8    ----7fdba1451000]
[test 9    ----7fdba1452000]
[test 10   ----7fdba1453000]

内核模块日志：
[143931.760532] MMAPTEST: process: mmap_test_read (20678)
[143931.760636] MMAPTEST: map_fault: map VMALLOC：0xffffc3310120a000 (PFN: 0x000000022f1ea000) to vm faulting address: 0x7fdba144a000 , size: 0x1000, page:0 
[143931.760714] MMAPTEST: map_fault: map VMALLOC：0xffffc3310120b000 (PFN: 0x00000002359e6000) to vm faulting address: 0x7fdba144b000 , size: 0x1000, page:1 
[143931.760746] MMAPTEST: map_fault: map VMALLOC：0xffffc3310120c000 (PFN: 0x000000022f0ca000) to vm faulting address: 0x7fdba144c000 , size: 0x1000, page:2 
[143931.760796] MMAPTEST: map_fault: map VMALLOC：0xffffc3310120d000 (PFN: 0x000000022d872000) to vm faulting address: 0x7fdba144d000 , size: 0x1000, page:3 
[143931.760823] MMAPTEST: map_fault: map VMALLOC：0xffffc3310120e000 (PFN: 0x0000000265e93000) to vm faulting address: 0x7fdba144e000 , size: 0x1000, page:4 
[143931.760862] MMAPTEST: map_fault: map VMALLOC：0xffffc3310120f000 (PFN: 0x00000002617f7000) to vm faulting address: 0x7fdba144f000 , size: 0x1000, page:5 
[143931.760889] MMAPTEST: map_fault: map VMALLOC：0xffffc33101210000 (PFN: 0x0000000245942000) to vm faulting address: 0x7fdba1450000 , size: 0x1000, page:6 
[143931.760940] MMAPTEST: map_fault: map VMALLOC：0xffffc33101211000 (PFN: 0x000000022da61000) to vm faulting address: 0x7fdba1451000 , size: 0x1000, page:7 
[143931.760985] MMAPTEST: map_fault: map VMALLOC：0xffffc33101212000 (PFN: 0x0000000265ecf000) to vm faulting address: 0x7fdba1452000 , size: 0x1000, page:8 
[143931.761020] MMAPTEST: map_fault: map VMALLOC：0xffffc33101213000 (PFN: 0x0000000241807000) to vm faulting address: 0x7fdba1453000 , size: 0x1000, page:9 
[143935.924339] MMAPTEST: mapping vma is closed