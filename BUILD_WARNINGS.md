# Build Warnings & Dependencies - Hướng Dẫn Lưu Ý Khi Build

## 📋 Khuôn Mẫu Lưu Ý

Mỗi component sẽ được mô tả theo khuôn mẫu sau:

```
┌─────────────────────────────────────────────────┐
│ [COMPONENT NAME]                                │
├─────────────────────────────────────────────────┤
│ ⚠️ TRƯỚC KHI BUILD                              │
│ - Điều kiện tiên quyết                          │
│ - Dependencies phải có                          │
│                                                 │
│ 🔧 TRONG KHI BUILD                              │
│ - Configuration cần chú ý                       │
│ - Settings đặc biệt                             │
│                                                 │
│ ✅ SAU KHI BUILD                                │
│ - File output cần kiểm tra                      │
│ - Bước tiếp theo                                │
│                                                 │
│ ⚡ COMMON ISSUES                                │
│ - Lỗi thường gặp và cách fix                    │
└─────────────────────────────────────────────────┘
```

---

## 1️⃣ zbuffer (Networking Layer)

### 📦 Component: zbuffer/client

```
┌─────────────────────────────────────────────────────────────────┐
│ zbuffer/client - Client Networking Library                      │
├─────────────────────────────────────────────────────────────────┤
│ ⚠️ TRƯỚC KHI BUILD                                              │
│ ✓ Không phụ thuộc component nào khác                            │
│ ✓ Kiểm tra Windows SDK đã cài đặt                               │
│ ✓ Đảm bảo có thư viện ws2_32.lib (Windows Sockets)              │
│                                                                 │
│ 🔧 TRONG KHI BUILD                                              │
│ ⚠️ PHẢI build với configuration: Release|Win32                  │
│ ⚠️ Platform Toolset: v141 hoặc v142                             │
│ ⚠️ Character Set: Multi-Byte (không dùng Unicode)               │
│ ⚠️ Linker → Additional Dependencies: ws2_32.lib winmm.lib       │
│                                                                 │
│ ✅ SAU KHI BUILD                                                │
│ ✓ Kiểm tra file: zbuffer/client/Release/client.lib              │
│ ✓ Kích thước file: ~50-200KB (nếu quá nhỏ có thể lỗi)           │
│ ✓ QUAN TRỌNG: Copy client.lib vào thư mục lib chung             │
│   → Các component khác sẽ cần link tới file này                 │
│                                                                 │
│ ⚡ COMMON ISSUES                                                │
│ ❌ Lỗi: "ws2_32.lib not found"                                  │
│    → Fix: Project Properties → Linker → Input → Add ws2_32.lib │
│ ❌ Lỗi: "unresolved external symbol __imp__socket"             │
│    → Fix: Thiếu ws2_32.lib trong linker dependencies            │
└─────────────────────────────────────────────────────────────────┘
```

### 📦 Component: zbuffer/server

```
┌─────────────────────────────────────────────────────────────────┐
│ zbuffer/server - Server Networking Library                      │
├─────────────────────────────────────────────────────────────────┤
│ ⚠️ TRƯỚC KHI BUILD                                              │
│ ✓ Không phụ thuộc component nào khác                            │
│ ✓ Tương tự zbuffer/client                                       │
│                                                                 │
│ 🔧 TRONG KHI BUILD                                              │
│ ⚠️ CẨN THẬN: Định nghĩa preprocessor khác với client            │
│ ⚠️ Thêm define: _SERVER                                         │
│ ⚠️ Linker settings tương tự client                              │
│                                                                 │
│ ✅ SAU KHI BUILD                                                │
│ ✓ Kiểm tra: zbuffer/server/Release/server.lib                   │
│ ✓ KHÔNG dùng nhầm với client.lib khi link vào server projects   │
│                                                                 │
│ ⚡ COMMON ISSUES                                                │
│ ❌ Lỗi: Link conflict với client.lib                            │
│    → Fix: Đảm bảo server projects chỉ link server.lib           │
└─────────────────────────────────────────────────────────────────┘
```

**🔗 MỐI QUAN HỆ:**
```
zbuffer/client ─┬─> Sẽ được dùng bởi: Core (client), S3Client, Network
                │
zbuffer/server ─┴─> Sẽ được dùng bởi: Core (server), S3Server
```

---

## 2️⃣ Core (Core Library)

```
┌─────────────────────────────────────────────────────────────────┐
│ Core - Thư Viện Cốt Lõi                                         │
├─────────────────────────────────────────────────────────────────┤
│ ⚠️ TRƯỚC KHI BUILD                                              │
│ ✓ DEPENDENCIES BẮT BUỘC:                                        │
│   [Client] → zbuffer/client/Release/client.lib                  │
│   [Server] → zbuffer/server/Release/server.lib                  │
│                                                                 │
│ ⚠️ KIỂM TRA LINKER INPUT:                                       │
│   Project Properties → Linker → Input → Additional Dependencies │
│   Phải có: ../zbuffer/client/Release/client.lib (cho client)    │
│                                                                 │
│ 🔧 TRONG KHI BUILD                                              │
│ ⚠️ Core có 2 configurations:                                    │
│   • Client Release → Build cho client (dùng zbuffer/client)     │
│   • Server Release → Build cho server (dùng zbuffer/server)     │
│                                                                 │
│ ⚠️ PHẢI build CẢ 2 nếu cần cả client và server:                 │
│   1. Build → Configuration Manager                              │
│   2. Chọn "Client Release" → Build                              │
│   3. Chọn "Server Release" → Build                              │
│                                                                 │
│ ⚠️ Include Directories phải có:                                 │
│   $(ProjectDir)\..\zbuffer\client (cho client build)            │
│                                                                 │
│ ✅ SAU KHI BUILD                                                │
│ ✓ Kiểm tra files:                                               │
│   - Core/Client Release/Core.lib                                │
│   - Core/Server Release/Core.lib                                │
│ ✓ Kích thước: ~500KB - 2MB mỗi file                             │
│                                                                 │
│ ⚠️ LƯU Ý QUAN TRỌNG:                                            │
│   → Core.lib (client) và Core.lib (server) KHÁC NHAU!           │
│   → Không dùng nhầm khi link vào các project sau                │
│                                                                 │
│ ⚡ COMMON ISSUES                                                │
│ ❌ Lỗi: "client.lib not found"                                  │
│    → Fix: Chưa build zbuffer/client hoặc sai đường dẫn          │
│    → Kiểm tra: Linker → Additional Library Directories          │
│                                                                 │
│ ❌ Lỗi: "unresolved external symbol" từ zbuffer                 │
│    → Fix: Đảm bảo đã link đúng client.lib/server.lib            │
│    → Kiểm tra configuration (Client Release vs Server Release)  │
│                                                                 │
│ ❌ Warning: "multiply defined symbols"                          │
│    → Fix: Đang link cả client.lib và server.lib cùng lúc        │
│    → Chỉ link một trong hai tùy theo build type                 │
└─────────────────────────────────────────────────────────────────┘
```

**🔗 MỐI QUAN HỆ:**
```
zbuffer/client ──> Core (Client Release) ─┬─> Engine, Kernel, S3Client
                                          │
zbuffer/server ──> Core (Server Release) ─┴─> S3Server
```

---

## 3️⃣ Engine (Rendering Engine)

```
┌─────────────────────────────────────────────────────────────────┐
│ Engine - Game Rendering Engine                                  │
├─────────────────────────────────────────────────────────────────┤
│ ⚠️ TRƯỚC KHI BUILD                                              │
│ ✓ DEPENDENCIES BẮT BUỘC:                                        │
│   [1] Core/Client Release/Core.lib                              │
│   [2] zbuffer/client/Release/client.lib (gián tiếp qua Core)    │
│                                                                 │
│ ⚠️ KIỂM TRA TRƯỚC:                                              │
│   □ Core (Client Release) đã build thành công                   │
│   □ Core.lib có trong thư mục Core/Client Release/              │
│   □ DirectX SDK đã cài đặt (nếu cần)                            │
│                                                                 │
│ 🔧 TRONG KHI BUILD                                              │
│ ⚠️ Linker → Input → Additional Dependencies:                    │
│   ../Core/Client Release/Core.lib                               │
│   d3d9.lib        (DirectX 9)                                   │
│   d3dx9.lib       (DirectX 9 Extensions)                        │
│   winmm.lib       (Multimedia)                                  │
│                                                                 │
│ ⚠️ Include Directories:                                         │
│   $(ProjectDir)\..\Core                                         │
│   $(ProjectDir)\..\zbuffer\client                               │
│   $(DXSDK_DIR)\Include  (nếu dùng DirectX)                      │
│                                                                 │
│ ⚠️ Library Directories:                                         │
│   $(ProjectDir)\..\Core\Client Release                          │
│   $(DXSDK_DIR)\Lib\x86                                          │
│                                                                 │
│ ⚠️ Preprocessor Definitions:                                    │
│   _CLIENT                                                       │
│   WIN32                                                         │
│   NDEBUG (cho Release)                                          │
│                                                                 │
│ ✅ SAU KHI BUILD                                                │
│ ✓ Kiểm tra: Engine/Release/Engine.lib                           │
│ ✓ Kích thước: ~1-5MB                                            │
│                                                                 │
│ ⚠️ NẾU build thành công nhưng S3Client crash khi chạy:          │
│   → Có thể thiếu DirectX runtime DLLs                            │
│   → Cài DirectX End-User Runtime (June 2010)                    │
│                                                                 │
│ ⚡ COMMON ISSUES                                                │
│ ❌ Lỗi: "Core.lib not found"                                    │
│    → Fix: Build Core trước, hoặc sai đường dẫn library          │
│                                                                 │
│ ❌ Lỗi: "d3d9.lib not found"                                    │
│    → Fix: Chưa cài DirectX SDK                                  │
│    → Download: DirectX SDK (June 2010)                          │
│    → Thêm $(DXSDK_DIR)\Lib\x86 vào Library Directories          │
│                                                                 │
│ ❌ Lỗi: "unresolved external symbol D3DXCreateTexture"         │
│    → Fix: Thiếu d3dx9.lib trong linker dependencies             │
│                                                                 │
│ ❌ Warning: "LNK4099: PDB not found"                            │
│    → Info: Không ảnh hưởng, chỉ là warning                      │
│    → Có thể ignore trong Release build                          │
└─────────────────────────────────────────────────────────────────┘
```

**🔗 MỐI QUAN HỆ:**
```
Core (Client) ──> Engine ─┬─> Kernel
                          └─> S3Client
```

---

## 4️⃣ Kernel (Game Object Layer)

```
┌─────────────────────────────────────────────────────────────────┐
│ Kernel - Game Object & Logic Layer                              │
├─────────────────────────────────────────────────────────────────┤
│ ⚠️ TRƯỚC KHI BUILD                                              │
│ ✓ DEPENDENCIES BẮT BUỘC (theo thứ tự):                          │
│   [1] zbuffer/client/Release/client.lib                         │
│   [2] Core/Client Release/Core.lib                              │
│   [3] Engine/Release/Engine.lib                                 │
│                                                                 │
│ ⚠️ ĐẶC BIỆT LƯU Ý:                                              │
│   → Kernel phụ thuộc CẢ Core VÀ Engine                          │
│   → Phải build cả 2 trước khi build Kernel                      │
│   → Nếu rebuild Core hoặc Engine, phải rebuild Kernel           │
│                                                                 │
│ 🔧 TRONG KHI BUILD                                              │
│ ⚠️ Linker → Input → Additional Dependencies (ĐÚNG THỨ TỰ):      │
│   ../Engine/Release/Engine.lib                                  │
│   ../Core/Client Release/Core.lib                               │
│   ../zbuffer/client/Release/client.lib                          │
│   (Thứ tự quan trọng để tránh link errors!)                     │
│                                                                 │
│ ⚠️ Include Directories (CẢ 3):                                  │
│   $(ProjectDir)\..\Core                                         │
│   $(ProjectDir)\..\Engine                                       │
│   $(ProjectDir)\..\zbuffer\client                               │
│                                                                 │
│ ⚠️ Library Directories (CẢ 3):                                  │
│   $(ProjectDir)\..\Core\Client Release                          │
│   $(ProjectDir)\..\Engine\Release                               │
│   $(ProjectDir)\..\zbuffer\client\Release                       │
│                                                                 │
│ ✅ SAU KHI BUILD                                                │
│ ✓ Kiểm tra: Kernel/Release/Kernel.lib                           │
│ ✓ Kích thước: ~2-10MB (lớn nhất trong các libs)                 │
│                                                                 │
│ ⚠️ VALIDATION:                                                  │
│   → Dùng "dumpbin /SYMBOLS Kernel.lib" để check symbols         │
│   → Phải thấy symbols từ Core và Engine                          │
│                                                                 │
│ ⚡ COMMON ISSUES                                                │
│ ❌ Lỗi: "unresolved external symbol" từ Core hoặc Engine       │
│    → Fix: Kiểm tra đã link đủ cả Core.lib và Engine.lib         │
│    → Kiểm tra thứ tự link (Engine trước, Core sau)              │
│                                                                 │
│ ❌ Lỗi: "LNK2005: already defined" (multiply defined)           │
│    → Fix: Có file .cpp được include 2 lần                       │
│    → Hoặc link thư viện trùng lặp                                │
│    → Kiểm tra không link cả debug và release libs               │
│                                                                 │
│ ❌ Build chậm (>5 phút):                                        │
│    → Normal: Kernel có nhiều code, file lớn                     │
│    → Tip: Bật parallel build trong VS settings                  │
└─────────────────────────────────────────────────────────────────┘
```

**🔗 MỐI QUAN HỆ:**
```
zbuffer/client ──> Core ──> Engine ──> Kernel ──> S3Client
                                                └─> S3Server (dùng Kernel khác)
```

---

## 5️⃣ Network (Network Utilities)

```
┌─────────────────────────────────────────────────────────────────┐
│ Network - Network Utility Layer                                 │
├─────────────────────────────────────────────────────────────────┤
│ ⚠️ TRƯỚC KHI BUILD                                              │
│ ✓ DEPENDENCIES:                                                 │
│   [1] zbuffer/client/Release/client.lib (hoặc server.lib)       │
│   [2] Core/Client Release/Core.lib (optional)                   │
│                                                                 │
│ 🔧 TRONG KHI BUILD                                              │
│ ⚠️ Build song song với Kernel (không phụ thuộc nhau)            │
│ ⚠️ Linker dependencies: tương tự zbuffer                        │
│                                                                 │
│ ✅ SAU KHI BUILD                                                │
│ ✓ Kiểm tra: Network/Release/Network.lib                         │
│                                                                 │
│ ⚡ COMMON ISSUES                                                │
│ ❌ Tương tự zbuffer/client                                      │
└─────────────────────────────────────────────────────────────────┘
```

---

## 6️⃣ S3Client (Game Client Application)

```
┌─────────────────────────────────────────────────────────────────┐
│ S3Client - Game Client Executable (MAIN BUILD)                  │
├─────────────────────────────────────────────────────────────────┤
│ ⚠️ TRƯỚC KHI BUILD                                              │
│ ✓ DEPENDENCIES BẮT BUỘC (TẤT CẢ):                              │
│   [1] zbuffer/client/Release/client.lib                         │
│   [2] Core/Client Release/Core.lib                              │
│   [3] Engine/Release/Engine.lib                                 │
│   [4] Kernel/Release/Kernel.lib                                 │
│   [5] Network/Release/Network.lib (optional)                    │
│                                                                 │
│ ⚠️ QUAN TRỌNG NHẤT:                                             │
│   → Đây là bước CUỐI CÙNG trong build chain                     │
│   → Phải build TẤT CẢ dependencies trước                        │
│   → Nếu rebuild bất kỳ lib nào, phải rebuild S3Client           │
│                                                                 │
│ ⚠️ KIỂM TRA TOÀN BỘ TRƯỚC KHI BUILD:                            │
│   □ client.lib exists và mới nhất                               │
│   □ Core.lib exists và đúng configuration (Client Release)      │
│   □ Engine.lib exists                                           │
│   □ Kernel.lib exists                                           │
│   □ Tất cả .lib files có timestamp gần đây (không cũ)           │
│                                                                 │
│ 🔧 TRONG KHI BUILD                                              │
│ ⚠️ Configuration: PHẢI là Release|Win32                          │
│   (Debug build sẽ crash hoặc chạy cực chậm)                     │
│                                                                 │
│ ⚠️ Linker → Input → Additional Dependencies (THỨ TỰ QUAN TRỌNG):│
│   ../Kernel/Release/Kernel.lib                                  │
│   ../Engine/Release/Engine.lib                                  │
│   ../Core/Client Release/Core.lib                               │
│   ../Network/Release/Network.lib                                │
│   ../zbuffer/client/Release/client.lib                          │
│   ws2_32.lib                                                    │
│   winmm.lib                                                     │
│   d3d9.lib                                                      │
│   d3dx9.lib                                                     │
│   (Thứ tự: từ high-level → low-level)                           │
│                                                                 │
│ ⚠️ Include Directories (TẤT CẢ):                                │
│   $(ProjectDir)\..\Core                                         │
│   $(ProjectDir)\..\Engine                                       │
│   $(ProjectDir)\..\Kernel                                       │
│   $(ProjectDir)\..\Network                                      │
│   $(ProjectDir)\..\zbuffer\client                               │
│   $(ProjectDir)\Login                                           │
│   $(ProjectDir)\Ui                                              │
│                                                                 │
│ ⚠️ Library Directories (TẤT CẢ):                                │
│   $(ProjectDir)\..\Core\Client Release                          │
│   $(ProjectDir)\..\Engine\Release                               │
│   $(ProjectDir)\..\Kernel\Release                               │
│   $(ProjectDir)\..\Network\Release                              │
│   $(ProjectDir)\..\zbuffer\client\Release                       │
│   $(DXSDK_DIR)\Lib\x86                                          │
│                                                                 │
│ ⚠️ Runtime Library:                                             │
│   C/C++ → Code Generation → Runtime Library                     │
│   PHẢI CHỌN: Multi-threaded (/MT) cho Release                   │
│   (Không dùng /MD để tránh phụ thuộc runtime DLLs)              │
│                                                                 │
│ ⚠️ Subsystem:                                                   │
│   Linker → System → Subsystem: Windows (/SUBSYSTEM:WINDOWS)     │
│   Entry Point: WinMainCRTStartup (hoặc để trống)                │
│                                                                 │
│ ✅ SAU KHI BUILD                                                │
│ ✓ Kiểm tra file: S3Client/Release/S3Client.exe                  │
│ ✓ Kích thước: ~2-10MB (tùy features)                            │
│                                                                 │
│ ⚠️ VALIDATION BẮT BUỘC:                                         │
│   1. Check dependencies bằng Dependency Walker:                 │
│      → depends.exe S3Client.exe                                 │
│      → Không được có missing DLLs (màu đỏ)                      │
│                                                                 │
│   2. Test chạy ngay:                                            │
│      cd S3Client\Release                                        │
│      S3Client.exe                                               │
│      → Phải hiện splash screen, không crash                     │
│                                                                 │
│   3. Copy resources cần thiết:                                  │
│      □ Data/ folder                                             │
│      □ Settings/ folder                                         │
│      □ *.ini config files                                       │
│                                                                 │
│ ⚠️ NẾU BUILD THÀNH CÔNG nhưng exe không chạy:                   │
│   → Thiếu resources (Data folder)                               │
│   → Thiếu config files (*.ini)                                  │
│   → Thiếu DirectX runtime                                       │
│   → Check Logs/client.log để xem lỗi                            │
│                                                                 │
│ ⚡ COMMON ISSUES                                                │
│ ❌ Lỗi: "unresolved external symbol" từ Kernel                 │
│    → Fix: Kiểm tra đã link Kernel.lib                           │
│    → Kiểm tra Kernel.lib là bản mới nhất (rebuild nếu cần)      │
│                                                                 │
│ ❌ Lỗi: "LNK1120: unresolved externals" (nhiều symbols)         │
│    → Fix: Thiếu một hoặc nhiều .lib files                       │
│    → Checklist: client.lib, Core.lib, Engine.lib, Kernel.lib    │
│                                                                 │
│ ❌ Build OK nhưng crash ngay khi chạy:                          │
│    → Debug bằng cách chạy trong Visual Studio (F5)              │
│    → Check Exception message                                    │
│    → Thường do:                                                 │
│      • Thiếu Data folder                                        │
│      • Thiếu config.ini                                         │
│      • Mismatched runtime library (/MT vs /MD)                  │
│      • Core/Engine/Kernel build sai configuration               │
│                                                                 │
│ ❌ Lỗi: "MSVCR140.dll not found" khi chạy:                     │
│    → Fix: Runtime library setting sai                           │
│    → Đổi thành /MT (static link) thay vì /MD                    │
│    → Rebuild all                                                │
│                                                                 │
│ ❌ Lỗi: "d3dx9_43.dll not found":                              │
│    → Fix: Cài DirectX End-User Runtime (June 2010)              │
│                                                                 │
│ ❌ Build cực chậm (>10 phút):                                   │
│    → Normal nếu lần đầu build                                   │
│    → Incremental build lần sau sẽ nhanh hơn                     │
│    → Tip: Tắt antivirus scan cho thư mục project                │
└─────────────────────────────────────────────────────────────────┘
```

**🔗 DEPENDENCY CHAIN ĐẦY ĐỦ:**
```
S3Client.exe
    ↓ link
    ├─→ Kernel.lib
    │      ↓ link
    │      ├─→ Engine.lib
    │      │      ↓ link
    │      │      └─→ Core.lib (Client)
    │      │             ↓ link
    │      │             └─→ client.lib
    │      └─→ Core.lib (Client)
    │             ↓ link
    │             └─→ client.lib
    │
    ├─→ Engine.lib
    │      ↓ link
    │      └─→ Core.lib (Client)
    │             ↓ link
    │             └─→ client.lib
    │
    ├─→ Core.lib (Client)
    │      ↓ link
    │      └─→ client.lib
    │
    ├─→ Network.lib
    │      ↓ link
    │      └─→ client.lib
    │
    └─→ client.lib
           ↓ link
           └─→ ws2_32.lib (Windows Socket)
```

---

## 7️⃣ S3Server (Game Server Application)

```
┌─────────────────────────────────────────────────────────────────┐
│ S3Server - Game Server Executable                               │
├─────────────────────────────────────────────────────────────────┤
│ ⚠️ TRƯỚC KHI BUILD                                              │
│ ✓ DEPENDENCIES:                                                 │
│   [1] zbuffer/server/Release/server.lib (KHÔNG phải client.lib) │
│   [2] Core/Server Release/Core.lib (KHÔNG phải Client Release)  │
│   [3] Kernel/Release/Kernel.lib (server version)                │
│                                                                 │
│ ⚠️ QUAN TRỌNG:                                                  │
│   → Dùng SERVER VERSIONS của libs, không dùng client versions   │
│   → zbuffer/server.lib, không phải client.lib                   │
│   → Core (Server Release), không phải Core (Client Release)     │
│                                                                 │
│ 🔧 TRONG KHI BUILD                                              │
│ ⚠️ Configuration: Server Release|Win32                           │
│ ⚠️ Preprocessor: _SERVER, WIN32, NDEBUG                          │
│ ⚠️ Linker dependencies:                                         │
│   ../Kernel/Server Release/Kernel.lib                           │
│   ../Core/Server Release/Core.lib                               │
│   ../zbuffer/server/Release/server.lib                          │
│   ws2_32.lib                                                    │
│   (KHÔNG link d3d9.lib, d3dx9.lib - không cần graphics)         │
│                                                                 │
│ ✅ SAU KHI BUILD                                                │
│ ✓ Kiểm tra: S3Server/Release/S3Server.exe                       │
│ ✓ Test chạy: phải khởi động console và listen port              │
│                                                                 │
│ ⚡ COMMON ISSUES                                                │
│ ❌ Lỗi: Link conflict với client.lib                            │
│    → Fix: Đảm bảo dùng server.lib, không phải client.lib        │
│                                                                 │
│ ❌ Server crash khi start:                                      │
│    → Check database connection settings                         │
│    → Check port đã bị chiếm chưa                                │
└─────────────────────────────────────────────────────────────────┘
```

---

## 8️⃣ Anti-Cheat-Client / fProtectClient (Anti-Cheat)

```
┌─────────────────────────────────────────────────────────────────┐
│ fProtectClient - Anti-Cheat DLL                                 │
├─────────────────────────────────────────────────────────────────┤
│ ⚠️ TRƯỚC KHI BUILD                                              │
│ ✓ DEPENDENCIES:                                                 │
│   [1] Core/Client Release/Core.lib                              │
│   [2] Engine/Release/Engine.lib (optional)                      │
│   [3] detours.lib (đã có sẵn trong thư mục Anti-Cheat-Client)          │
│                                                                 │
│ ⚠️ KIỂM TRA:                                                    │
│   □ File Anti-Cheat-Client/detours.lib exists                          │
│   □ File Anti-Cheat-Client/detours.h exists                            │
│                                                                 │
│ 🔧 TRONG KHI BUILD                                              │
│ ⚠️ Output Type: Dynamic Library (.dll)                          │
│   Project Properties → Configuration Type: DLL                  │
│                                                                 │
│ ⚠️ Linker → Additional Dependencies:                            │
│   ../Core/Client Release/Core.lib                               │
│   detours.lib                                                   │
│   ws2_32.lib                                                    │
│                                                                 │
│ ⚠️ Library Directories:                                         │
│   $(ProjectDir)\..\Core\Client Release                          │
│   $(ProjectDir)  (cho detours.lib)                              │
│                                                                 │
│ ⚠️ Preprocessor:                                                │
│   _WINDLL                                                       │
│   _USRDLL                                                       │
│   FPROTECTCLIENT_EXPORTS                                        │
│                                                                 │
│ ✅ SAU KHI BUILD                                                │
│ ✓ Kiểm tra: Anti-Cheat-Client/Release/fProtectClient.dll               │
│ ✓ Kích thước: ~100KB - 500KB                                    │
│                                                                 │
│ ⚠️ DEPLOYMENT:                                                  │
│   → Copy fProtectClient.dll vào thư mục chứa S3Client.exe       │
│   → DLL này sẽ được load bởi S3Client hoặc wrapper               │
│                                                                 │
│ ⚠️ TESTING:                                                     │
│   → Chạy S3Client.exe với fProtectClient.dll trong cùng folder  │
│   → Check log xem DLL có load thành công không                  │
│   → Test các anti-cheat features                                │
│                                                                 │
│ ⚡ COMMON ISSUES                                                │
│ ❌ Lỗi: "detours.lib not found"                                │
│    → Fix: Kiểm tra file có trong Anti-Cheat-Client/ folder             │
│    → Thêm $(ProjectDir) vào Library Directories                 │
│                                                                 │
│ ❌ DLL build OK nhưng S3Client không load:                      │
│    → Check S3Client code có load DLL không                      │
│    → Dùng Process Explorer xem DLL có được load                 │
│    → Check export functions: dumpbin /EXPORTS fProtectClient.dll│
│                                                                 │
│ ❌ S3Client crash sau khi load DLL:                             │
│    → Hook function có thể conflict                              │
│    → Debug từng hook function                                   │
│    → Check calling convention (__stdcall vs __cdecl)            │
└─────────────────────────────────────────────────────────────────┘
```

---

## 9️⃣ CoreClient (C# Client Library)

```
┌─────────────────────────────────────────────────────────────────┐
│ CoreClient - C# Client Helper Library                           │
├─────────────────────────────────────────────────────────────────┤
│ ⚠️ TRƯỚC KHI BUILD                                              │
│ ✓ Không phụ thuộc C++ libraries                                 │
│ ✓ Kiểm tra .NET Framework SDK đã cài                            │
│                                                                 │
│ 🔧 TRONG KHI BUILD                                              │
│ ⚠️ Target Framework: .NET Framework 4.7.2 hoặc cao hơn          │
│ ⚠️ Platform: Any CPU hoặc x86                                   │
│ ⚠️ Configuration: Release                                       │
│                                                                 │
│ ⚠️ NuGet Packages:                                              │
│   → Restore packages trước khi build                            │
│   → Right-click solution → Restore NuGet Packages               │
│                                                                 │
│ ✅ SAU KHI BUILD                                                │
│ ✓ Kiểm tra: CoreClient/bin/Release/CoreClient.dll               │
│                                                                 │
│ ⚠️ DEPLOYMENT:                                                  │
│   → Copy CoreClient.dll vào thư mục game client                 │
│   → Copy dependencies (nếu có NuGet packages)                   │
│                                                                 │
│ ⚡ COMMON ISSUES                                                │
│ ❌ Lỗi: "Target framework not installed"                       │
│    → Fix: Cài .NET Framework developer pack                     │
│                                                                 │
│ ❌ NuGet restore failed:                                        │
│    → Fix: Check internet connection                             │
│    → Check NuGet source settings                                │
└─────────────────────────────────────────────────────────────────┘
```

---

## 🔟 Project_ProtectJX (Wrapper/Loader)

```
┌─────────────────────────────────────────────────────────────────┐
│ Project_ProtectJX - Game Loader & Protection Wrapper            │
├─────────────────────────────────────────────────────────────────┤
│ ⚠️ TRƯỚC KHI BUILD                                              │
│ ✓ S3Client.exe đã build xong                                    │
│ ✓ fProtectClient.dll đã build xong                              │
│ ✓ WinLicense SDK available (nếu dùng)                           │
│                                                                 │
│ 🔧 TRONG KHI BUILD                                              │
│ ⚠️ Configuration: Release|x86                                    │
│ ⚠️ Output: Safe.exe hoặc JxOnline.exe                            │
│                                                                 │
│ ⚠️ Preprocessor có thể cần:                                     │
│   USE_WINLICENSE                                                │
│                                                                 │
│ ✅ SAU KHI BUILD                                                │
│ ✓ Kiểm tra: Project_ProtectJX/Project1/Release/Safe.exe         │
│                                                                 │
│ ⚠️ DEPLOYMENT & CONFIGURATION:                                  │
│   → Safe.exe phải ở cùng thư mục với S3Client.exe               │
│   → Safe.exe sẽ load và protect S3Client.exe                    │
│   → Safe.exe sẽ inject fProtectClient.dll                       │
│                                                                 │
│ ⚠️ FOLDER STRUCTURE khi deploy:                                 │
│   GameClient/                                                   │
│   ├─ Safe.exe           (Wrapper - chạy file này)               │
│   ├─ S3Client.exe       (Game client - được load bởi Safe.exe)  │
│   ├─ fProtectClient.dll (Anti-cheat - được inject)              │
│   ├─ CoreClient.dll     (Helper library)                        │
│   ├─ Data/                                                      │
│   └─ Settings/                                                  │
│                                                                 │
│ ⚠️ TESTING:                                                     │
│   1. Chạy Safe.exe (KHÔNG chạy S3Client.exe trực tiếp)          │
│   2. Safe.exe phải:                                             │
│      - Load S3Client.exe                                        │
│      - Inject fProtectClient.dll                                │
│      - Apply protection                                         │
│      - Start game                                               │
│                                                                 │
│ ⚡ COMMON ISSUES                                                │
│ ❌ Safe.exe không tìm thấy S3Client.exe:                        │
│    → Fix: Đảm bảo cùng thư mục                                  │
│    → Hoặc config path trong Safe.exe settings                   │
│                                                                 │
│ ❌ Inject fProtectClient.dll failed:                            │
│    → Check DLL có trong thư mục không                           │
│    → Check DLL architecture (x86 vs x64) match với exe          │
│    → Run as Administrator                                       │
│                                                                 │
│ ❌ Game crash khi start từ Safe.exe:                            │
│    → Test chạy S3Client.exe trực tiếp trước                     │
│    → Nếu S3Client.exe chạy được nhưng Safe.exe crash:           │
│      → Debug Safe.exe code                                      │
│      → Check injection code                                     │
└─────────────────────────────────────────────────────────────────┘
```

---

## 1️⃣1️⃣ Pack (Packing Tool)

```
┌─────────────────────────────────────────────────────────────────┐
│ Pack - Asset Packing Tool                                       │
├─────────────────────────────────────────────────────────────────┤
│ ⚠️ TRƯỚC KHI BUILD                                              │
│ ✓ Build độc lập, ít dependencies                                │
│                                                                 │
│ 🔧 TRONG KHI BUILD                                              │
│ ⚠️ Configuration: Release|Win32                                  │
│ ⚠️ Output Type: Console Application (.exe)                      │
│                                                                 │
│ ✅ SAU KHI BUILD                                                │
│ ✓ Kiểm tra: Pack/Release/Pack.exe                               │
│ ✓ Test: Pack.exe --help (xem có hiện usage không)               │
│                                                                 │
│ ⚡ COMMON ISSUES                                                │
│ ❌ Minimal issues, tool đơn giản                                │
└─────────────────────────────────────────────────────────────────┘
```

---

## 1️⃣2️⃣ Autoupdate (Update Tool)

```
┌─────────────────────────────────────────────────────────────────┐
│ Autoupdate - Game Auto-Update Utility                           │
├─────────────────────────────────────────────────────────────────┤
│ ⚠️ TRƯỚC KHI BUILD                                              │
│ ✓ Build độc lập                                                 │
│                                                                 │
│ 🔧 TRONG KHI BUILD                                              │
│ ⚠️ Configuration: Release|Win32                                  │
│                                                                 │
│ ✅ SAU KHI BUILD                                                │
│ ✓ Kiểm tra: Autoupdate/Release/Autoupdate.exe                   │
│                                                                 │
│ ⚡ COMMON ISSUES                                                │
│ ❌ Minimal issues                                               │
└─────────────────────────────────────────────────────────────────┘
```

---

## 📊 BUILD ORDER MATRIX (Ma Trận Thứ Tự Build)

```
┌────────┬──────────────────┬──────────────────────────────────────┐
│ ORDER  │ COMPONENT        │ DEPENDENCIES                         │
├────────┼──────────────────┼──────────────────────────────────────┤
│   1    │ zbuffer/client   │ NONE (build first)                   │
│   1    │ zbuffer/server   │ NONE (build parallel với client)     │
├────────┼──────────────────┼──────────────────────────────────────┤
│   2    │ Core (Client)    │ zbuffer/client                       │
│   2    │ Core (Server)    │ zbuffer/server                       │
├────────┼──────────────────┼──────────────────────────────────────┤
│   3    │ Engine           │ Core (Client), zbuffer/client        │
│   3    │ Network          │ zbuffer/client (build parallel)      │
├────────┼──────────────────┼──────────────────────────────────────┤
│   4    │ Kernel           │ Core, Engine, zbuffer/client         │
├────────┼──────────────────┼──────────────────────────────────────┤
│   5    │ S3Client         │ Kernel, Engine, Core, Network        │
│   5    │ S3Server         │ Kernel, Core (Server), zbuffer/svr   │
├────────┼──────────────────┼──────────────────────────────────────┤
│   6    │ fProtectClient   │ Core (Client), detours.lib           │
├────────┼──────────────────┼──────────────────────────────────────┤
│   7    │ Project_ProtectJX│ S3Client.exe, fProtectClient.dll     │
├────────┼──────────────────┼──────────────────────────────────────┤
│  ANY   │ Pack             │ NONE (independent tool)              │
│  ANY   │ Autoupdate       │ NONE (independent tool)              │
│  ANY   │ CoreClient (C#)  │ NONE (independent library)           │
└────────┴──────────────────┴──────────────────────────────────────┘

⚠️ LƯU Ý:
- Components ở cùng ORDER có thể build song song
- Phải build xong ORDER thấp trước khi build ORDER cao
- Nếu rebuild component ở ORDER thấp, phải rebuild tất cả ORDER cao hơn
```

---

## 🎯 CHECKLIST TỔNG THỂ

### ✅ Trước Khi Bắt Đầu Build Toàn Bộ Project

```
□ Visual Studio 2017/2019 installed
□ Windows SDK 10.0 installed
□ DirectX SDK (June 2010) installed (nếu cần graphics)
□ .NET Framework 4.7.2+ SDK installed (cho CoreClient)
□ Disk space: >10GB free
□ RAM: >4GB available
□ Antivirus: Tắt hoặc exclude project folder (build nhanh hơn)
```

### ✅ Build Dependencies Checklist

```
STEP 1: Build zbuffer
□ zbuffer/client/Release/client.lib ← BUILD FIRST
□ zbuffer/server/Release/server.lib ← BUILD FIRST

STEP 2: Build Core
□ Core/Client Release/Core.lib (uses client.lib)
□ Core/Server Release/Core.lib (uses server.lib)

STEP 3: Build Engine & Network (parallel)
□ Engine/Release/Engine.lib (uses Core Client + client.lib)
□ Network/Release/Network.lib (uses client.lib)

STEP 4: Build Kernel
□ Kernel/Release/Kernel.lib (uses Engine + Core + client.lib)

STEP 5: Build Main Applications
□ S3Client/Release/S3Client.exe (uses ALL client libs)
□ S3Server/Release/S3Server.exe (uses server libs)

STEP 6: Build Protection
□ Anti-Cheat-Client/Release/fProtectClient.dll
□ Project_ProtectJX/Project1/Release/Safe.exe

STEP 7: Build Tools (anytime)
□ Pack/Release/Pack.exe
□ Autoupdate/Release/Autoupdate.exe
□ CoreClient/bin/Release/CoreClient.dll
```

### ✅ Validation After Build

```
□ Tất cả .lib files có timestamp mới (không phải cache cũ)
□ Tất cả .exe files chạy được (không crash ngay)
□ S3Client.exe: Chạy và hiện splash screen
□ S3Server.exe: Chạy và listen port
□ fProtectClient.dll: Load được bởi S3Client
□ Safe.exe: Launch được S3Client
□ Tools (Pack, Autoupdate): Show help message khi chạy
```

---

## 🚨 CRITICAL WARNINGS (Cảnh Báo Nghiêm Trọng)

### ⛔ KHÔNG BAO GIỜ

```
❌ KHÔNG mix Debug và Release libraries
   → Crash hoặc unexpected behavior

❌ KHÔNG link cả client.lib và server.lib cùng lúc
   → Symbol conflicts

❌ KHÔNG dùng Core (Client Release) cho server builds
   → Runtime errors

❌ KHÔNG skip build dependencies
   → Unresolved symbols

❌ KHÔNG build với /MD khi các libs dùng /MT
   → Runtime library mismatch

❌ KHÔNG commit .lib/.exe files vào git (trừ dependencies như detours.lib)
   → Repo quá lớn

❌ KHÔNG deploy Debug builds cho users
   → Chạy cực chậm, dễ crash, lộ thông tin debug
```

### ⚠️ LƯU Ý KHI REBUILD

```
NẾU rebuild zbuffer/client → Phải rebuild:
  ├─ Core (Client)
  ├─ Engine
  ├─ Kernel
  ├─ Network
  └─ S3Client

NẾU rebuild Core (Client) → Phải rebuild:
  ├─ Engine
  ├─ Kernel
  └─ S3Client

NẾU rebuild Engine → Phải rebuild:
  ├─ Kernel
  └─ S3Client

NẾU rebuild Kernel → Phải rebuild:
  └─ S3Client

NẾU chỉ rebuild S3Client → KHÔNG cần rebuild gì khác
```

---

## 💡 TIPS PRO

### 🔥 Build Nhanh Hơn

```
1. Bật Parallel Build:
   Tools → Options → Projects and Solutions → Build and Run
   → Maximum parallel builds: 8 (hoặc số cores của CPU)

2. Bật Incremental Linking:
   Project Properties → Linker → General
   → Enable Incremental Linking: Yes

3. Dùng SSD cho project folder

4. Tắt antivirus scan thư mục project

5. Precompiled Headers:
   → Đã có stdafx.h/stdafx.cpp, đảm bảo bật option này
```

### 🎯 Batch Build Script

```batch
@echo off
echo ========================================
echo Building Sword Online - Full Build
echo ========================================

REM Step 1: zbuffer
echo [1/7] Building zbuffer...
devenv "ClientAnti_JX1\SwordOnline\Sources\zbuffer\zbuffer.sln" /build "Release|Win32"
if %errorlevel% neq 0 goto :error

REM Step 2: Core
echo [2/7] Building Core...
devenv "ClientAnti_JX1\SwordOnline\Sources\S3Client\S3Client.sln" /build "Client Release|Win32" /project Core
if %errorlevel% neq 0 goto :error

REM Step 3: Engine
echo [3/7] Building Engine...
devenv "ClientAnti_JX1\SwordOnline\Sources\Engine\Engine.vcxproj" /build "Release|Win32"
if %errorlevel% neq 0 goto :error

REM Step 4: Kernel
echo [4/7] Building Kernel...
devenv "ClientAnti_JX1\SwordOnline\Sources\Kernel\Kernel.vcxproj" /build "Release|Win32"
if %errorlevel% neq 0 goto :error

REM Step 5: S3Client
echo [5/7] Building S3Client...
devenv "ClientAnti_JX1\SwordOnline\Sources\S3Client\S3Client.sln" /build "Release|Win32"
if %errorlevel% neq 0 goto :error

REM Step 6: Anti-Cheat-Client
echo [6/7] Building fProtectClient...
devenv "ClientAnti_JX1\SwordOnline\Sources\fProtectClient.sln" /build "Release|Win32"
if %errorlevel% neq 0 goto :error

REM Step 7: Project_ProtectJX
echo [7/7] Building Project_ProtectJX...
devenv "Project_ProtectJX\JxOnline.sln" /build "Release|x86"
if %errorlevel% neq 0 goto :error

echo ========================================
echo BUILD SUCCESSFUL!
echo ========================================
pause
exit /b 0

:error
echo ========================================
echo BUILD FAILED! Check errors above.
echo ========================================
pause
exit /b 1
```

---

## 📚 TÀI LIỆU THAM KHẢO THÊM

- **BUILD_GUIDE.md** - Chi tiết build từng component
- **USAGE_GUIDE.md** - Cách chạy sau khi build
- **DEVELOPMENT_GUIDE.md** - Hướng dẫn chỉnh sửa code

---

**Phiên bản**: 1.0
**Ngày tạo**: 2025-10-10
**Mục đích**: Cảnh báo và lưu ý quan trọng khi build toàn bộ dự án Sword Online

---

## ✅ FINAL CHECKLIST

```
SAU KHI ĐỌC FILE NÀY, BẠN NÊN:

□ Hiểu rõ thứ tự build dependencies
□ Biết component nào phụ thuộc component nào
□ Biết lưu ý gì khi build từng component
□ Biết cách validate build thành công
□ Biết cách troubleshoot khi gặp lỗi
□ Biết rebuild cái nào khi thay đổi code

→ Sẵn sàng build toàn bộ project! 🚀
```
