{
  "utc": "2026-10-15T13:00:04.662889+00:00",
  "reader": "root Codex direct",
  "facts": [
    "finite trace one replay stops generating at declared count",
    "consume may send final CQ head doorbell; last CQE alone insufficient",
    "PCIe direction FIFO includes active head; switch consumes synchronously without own queue",
    "post-complete-event pending messages and both FIFO sizes zero can certify this simulator transport empty",
    "Stop_simulation has no call site under src (one declaration one definition)",
    "GC ongoing operation may be postponed before NAND execution",
    "scaled geometry effective GC threshold=max(floor(64*.05),10)=10 blocks, hard threshold max(floor(64*.005),1)=1; no source changes"
  ],
  "items": [
    {
      "path": "iommu-platform/out/d-persistent-source-20261015/offline/MQSim-51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16/src/host/PCIe_Link.cpp",
      "source_relative": "src/host/PCIe_Link.cpp",
      "sha256": "4b8987eb6fcdb8804d0bd429e75a2ec6477b63bfa6c76e50d37bbd17ff848ff4",
      "read_ranges": [
        [
          30,
          52
        ],
        [
          59,
          79
        ]
      ],
      "url": "https://github.com/CMU-SAFARI/MQSim/blob/51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16/src/host/PCIe_Link.cpp"
    },
    {
      "path": "iommu-platform/out/d-persistent-source-20261015/offline/MQSim-51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16/src/host/PCIe_Link.h",
      "source_relative": "src/host/PCIe_Link.h",
      "sha256": "6b1052f42f8cd51b8166b7e921272e4083b54dde596751e845ef5b56969a30ea",
      "read_ranges": [
        [
          33,
          65
        ]
      ],
      "url": "https://github.com/CMU-SAFARI/MQSim/blob/51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16/src/host/PCIe_Link.h"
    },
    {
      "path": "iommu-platform/out/d-persistent-source-20261015/offline/MQSim-51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16/src/host/PCIe_Switch.cpp",
      "source_relative": "src/host/PCIe_Switch.cpp",
      "sha256": "5a479b6f23beb13b47cd825794070ae0dafa7ee0ee5d7adfd517c4b8a3a58227",
      "read_ranges": [
        [
          10,
          18
        ]
      ],
      "url": "https://github.com/CMU-SAFARI/MQSim/blob/51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16/src/host/PCIe_Switch.cpp"
    },
    {
      "path": "iommu-platform/out/d-persistent-source-20261015/offline/MQSim-51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16/src/host/PCIe_Root_Complex.cpp",
      "source_relative": "src/host/PCIe_Root_Complex.cpp",
      "sha256": "760090c69ef312348d505f58f5b25a36409a7edf86a701ce7c3a61ab616f06f7",
      "read_ranges": [
        [
          9,
          29
        ],
        [
          32,
          46
        ],
        [
          49,
          77
        ]
      ],
      "url": "https://github.com/CMU-SAFARI/MQSim/blob/51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16/src/host/PCIe_Root_Complex.cpp"
    },
    {
      "path": "iommu-platform/out/d-persistent-source-20261015/offline/MQSim-51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16/src/host/PCIe_Root_Complex.h",
      "source_relative": "src/host/PCIe_Root_Complex.h",
      "sha256": "3e6f06714be7ab1139176c54fc2d5b1085dcfd372cced62dd3bd609045a21303",
      "read_ranges": [
        [
          24,
          42
        ]
      ],
      "url": "https://github.com/CMU-SAFARI/MQSim/blob/51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16/src/host/PCIe_Root_Complex.h"
    },
    {
      "path": "iommu-platform/out/d-persistent-source-20261015/offline/MQSim-51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16/src/host/IO_Flow_Trace_Based.cpp",
      "source_relative": "src/host/IO_Flow_Trace_Based.cpp",
      "sha256": "962cd00dacd0dff696a1b635a0dbc5cf6b89fa74eaa057eefda4a0d9d8ee59aa",
      "read_ranges": [
        [
          27,
          76
        ],
        [
          78,
          126
        ],
        [
          132,
          170
        ]
      ],
      "url": "https://github.com/CMU-SAFARI/MQSim/blob/51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16/src/host/IO_Flow_Trace_Based.cpp"
    },
    {
      "path": "iommu-platform/out/d-persistent-source-20261015/offline/MQSim-51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16/src/host/IO_Flow_Base.h",
      "source_relative": "src/host/IO_Flow_Base.h",
      "sha256": "046bee3bfb2c0b33ab6f5212a0b23a2de1983c262c423b3aab995fba8cf3957e",
      "read_ranges": [
        [
          46,
          100
        ]
      ],
      "url": "https://github.com/CMU-SAFARI/MQSim/blob/51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16/src/host/IO_Flow_Base.h"
    },
    {
      "path": "iommu-platform/out/d-persistent-source-20261015/offline/MQSim-51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16/src/host/IO_Flow_Base.cpp",
      "source_relative": "src/host/IO_Flow_Base.cpp",
      "sha256": "9816311f8a2043377e35e9d7d2859c127b4dac6138ab86ba1136f5cff90209ee",
      "read_ranges": [
        [
          247,
          340
        ],
        [
          533,
          546
        ]
      ],
      "url": "https://github.com/CMU-SAFARI/MQSim/blob/51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16/src/host/IO_Flow_Base.cpp"
    },
    {
      "path": "iommu-platform/out/d-persistent-source-20261015/offline/MQSim-51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16/src/ssd/GC_and_WL_Unit_Base.cpp",
      "source_relative": "src/ssd/GC_and_WL_Unit_Base.cpp",
      "sha256": "4fd111f2bee10d1c0427df90010e8d87cf6ae05b95178f4934685a3f9dcc7eb2",
      "read_ranges": [
        [
          21,
          36
        ],
        [
          105,
          166
        ]
      ],
      "url": "https://github.com/CMU-SAFARI/MQSim/blob/51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16/src/ssd/GC_and_WL_Unit_Base.cpp"
    },
    {
      "path": "iommu-platform/out/d-persistent-source-20261015/offline/MQSim-51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16/src/ssd/GC_and_WL_Unit_Page_Level.cpp",
      "source_relative": "src/ssd/GC_and_WL_Unit_Page_Level.cpp",
      "sha256": "e6d99b1ff57a1b9b3cb4970eef4f457bab25e3ac36be64f6019d51be32a49dc1",
      "read_ranges": [
        [
          43,
          58
        ],
        [
          138,
          192
        ]
      ],
      "url": "https://github.com/CMU-SAFARI/MQSim/blob/51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16/src/ssd/GC_and_WL_Unit_Page_Level.cpp"
    },
    {
      "path": "iommu-platform/out/d-persistent-source-20261015/offline/MQSim-51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16/src/ssd/Host_Interface_NVMe.cpp",
      "source_relative": "src/ssd/Host_Interface_NVMe.cpp",
      "sha256": "2ceb36d1bde7b8448dd89a8176aaf715050782f434d9be50d77b1171882a6901",
      "read_ranges": [
        [
          98,
          141
        ]
      ],
      "url": "https://github.com/CMU-SAFARI/MQSim/blob/51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16/src/ssd/Host_Interface_NVMe.cpp"
    },
    {
      "path": "iommu-platform/out/d-persistent-source-20261015/offline/MQSim-51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16/src/ssd/Address_Mapping_Unit_Page_Level.cpp",
      "source_relative": "src/ssd/Address_Mapping_Unit_Page_Level.cpp",
      "sha256": "2a306c386f0f03766eb1754218c1002c193ef03f6844689b160d38ddf967bc7a",
      "read_ranges": [
        [
          1146,
          1195
        ],
        [
          1665,
          1685
        ],
        [
          1730,
          1845
        ],
        [
          1860,
          1935
        ]
      ],
      "url": "https://github.com/CMU-SAFARI/MQSim/blob/51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16/src/ssd/Address_Mapping_Unit_Page_Level.cpp"
    },
    {
      "path": "iommu-platform/out/d-persistent-source-20261015/offline/MQSim-51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16/src/exec/SSD_Device.cpp",
      "source_relative": "src/exec/SSD_Device.cpp",
      "sha256": "6cf5ca72163417e0e59acc0bc2ab00a79bf8bfa97449c1b5346eebc8d3d3e9e3",
      "read_ranges": [
        [
          295,
          322
        ]
      ],
      "url": "https://github.com/CMU-SAFARI/MQSim/blob/51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16/src/exec/SSD_Device.cpp"
    },
    {
      "path": "iommu-platform/out/d-persistent-source-20261015/offline/MQSim-51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16/src/main.cpp",
      "source_relative": "src/main.cpp",
      "sha256": "6ceb0f90859ecb5b6972b5f431245272174482a2246ded342d8e4a87d0ba42cb",
      "read_ranges": [
        [
          16,
          37
        ],
        [
          39,
          85
        ],
        [
          107,
          145
        ]
      ],
      "url": "https://github.com/CMU-SAFARI/MQSim/blob/51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16/src/main.cpp"
    },
    {
      "path": "iommu-platform/out/d-persistent-source-20261015/offline/MQSim-51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16/Makefile",
      "source_relative": "Makefile",
      "sha256": "283cbc48f00a749e51d30be909f13de969eb30b5a94f2bf0952faead279d7378",
      "read_ranges": [
        [
          1,
          37
        ]
      ],
      "url": "https://github.com/CMU-SAFARI/MQSim/blob/51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16/Makefile"
    },
    {
      "path": "iommu-platform/out/d-persistent-source-20261015/offline/MQSim-51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16/src/sim/Engine.cpp",
      "source_relative": "src/sim/Engine.cpp",
      "sha256": "030f290767a41843fff5b4909b4b2c296de7e4d77953a2f867a2bd6679469c86",
      "read_ranges": [
        [
          80,
          106
        ]
      ],
      "url": "https://github.com/CMU-SAFARI/MQSim/blob/51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16/src/sim/Engine.cpp"
    },
    {
      "path": "iommu-platform/out/d-persistent-source-20261015/offline/MQSim-51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16/src/ssd/TSU_Base.h",
      "source_relative": "src/ssd/TSU_Base.h",
      "sha256": "5a47a25ef4d30b319252a4b1ded8fda6352f250512aba9a2b44ea7bbed2df7e9",
      "read_ranges": [
        [
          34,
          64
        ],
        [
          85,
          120
        ]
      ],
      "url": "https://github.com/CMU-SAFARI/MQSim/blob/51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16/src/ssd/TSU_Base.h"
    },
    {
      "path": "iommu-platform/out/d-persistent-source-20261015/offline/MQSim-51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16/src/ssd/TSU_OutofOrder.cpp",
      "source_relative": "src/ssd/TSU_OutofOrder.cpp",
      "sha256": "e76875319b03e26baa39b7300a7c9f718f347d6cdaea6b580e89a255ff233bb4",
      "read_ranges": [
        [
          145,
          260
        ]
      ],
      "url": "https://github.com/CMU-SAFARI/MQSim/blob/51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16/src/ssd/TSU_OutofOrder.cpp"
    },
    {
      "path": "iommu-platform/out/d-persistent-source-20261015/offline/MQSim-51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16/src/ssd/Data_Cache_Manager_Flash_Advanced.cpp",
      "source_relative": "src/ssd/Data_Cache_Manager_Flash_Advanced.cpp",
      "sha256": "884f622a39514fe591d9cdfa4e7af589fb9d5b53566dbc33fab0274cd25baa9e",
      "read_ranges": [
        [
          355,
          390
        ]
      ],
      "url": "https://github.com/CMU-SAFARI/MQSim/blob/51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16/src/ssd/Data_Cache_Manager_Flash_Advanced.cpp"
    },
    {
      "path": "iommu-platform/out/d-persistent-source-20261015/offline/MQSim-51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16/src/exec/Host_System.h",
      "source_relative": "src/exec/Host_System.h",
      "sha256": "b6782daf113bd2cf98479ee8c71726bae3bf44ef6a03bc375c1a28693274c129",
      "read_ranges": [
        [
          20,
          41
        ]
      ],
      "url": "https://github.com/CMU-SAFARI/MQSim/blob/51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16/src/exec/Host_System.h"
    },
    {
      "path": "iommu-platform/out/d-persistent-source-20261015/offline/MQSim-51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16/src/exec/SSD_Device.h",
      "source_relative": "src/exec/SSD_Device.h",
      "sha256": "dbf56e389ce8519f5b8688057d2e79ab234d5385ab1217f19ca14dc26058c817",
      "read_ranges": [
        [
          30,
          59
        ]
      ],
      "url": "https://github.com/CMU-SAFARI/MQSim/blob/51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16/src/exec/SSD_Device.h"
    },
    {
      "path": "iommu-platform/out/d-persistent-source-20261015/offline/MQSim-51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16/src/ssd/FTL.h",
      "source_relative": "src/ssd/FTL.h",
      "sha256": "2f13925a9ab4967826a0a20bee38acd24fa864854bffe9fd10d0eb7beb9aadc3",
      "read_ranges": [
        [
          28,
          55
        ]
      ],
      "url": "https://github.com/CMU-SAFARI/MQSim/blob/51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16/src/ssd/FTL.h"
    },
    {
      "path": "iommu-platform/out/d-persistent-source-20261015/offline/MQSim-51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16/src/host/Host_IO_Request.h",
      "source_relative": "src/host/Host_IO_Request.h",
      "sha256": "f18438b760a21a3b504f664db3f975853adb6866178ab2a097384ee104e65c7d",
      "read_ranges": [
        [
          7,
          22
        ]
      ],
      "url": "https://github.com/CMU-SAFARI/MQSim/blob/51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16/src/host/Host_IO_Request.h"
    }
  ]
}
