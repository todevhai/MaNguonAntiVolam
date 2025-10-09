# Hướng Dẫn Build Dự Án Sword Online

## Mục Lục
1. [Chuẩn Bị Môi Trường](#1-chuẩn-bị-môi-trường)
2. [Thứ Tự Build](#2-thứ-tự-build)
3. [Build Từng Component](#3-build-từng-component)
4. [Xử Lý Lỗi Thường Gặp](#4-xử-lý-lỗi-thường-gặp)
5. [Kiểm Tra Build](#5-kiểm-tra-build)

---

## 1. Chuẩn Bị Môi Trường

### 1.1. Yêu Cầu Phần Mềm

| Phần mềm | Phiên bản | Bắt buộc | Ghi chú |
|----------|-----------|----------|---------|
| Visual Studio | 2017 hoặc mới hơn | ✅ | Khuyến nghị VS 2017/2019 |
| Windows SDK | 10.0 trở lên | ✅ | Tích hợp trong VS |
| Platform Toolset | v141 hoặc v142 | ✅ | Tương thích với VS 2017/2019 |
| DirectX SDK | June 2010 | ⚠️ | Cho rendering (nếu cần) |
| .NET Framework | 4.7.2 trở lên | ⚠️ | Cho CoreClient (C#) |

### 1.2. Cấu Hình Visual Studio

**Bước 1: Cài đặt Workloads**
- Desktop development with C++
- .NET desktop development
- Game development with C++ (optional, nhưng khuyến nghị)

**Bước 2: Cấu Hình Include Directories**
```
Tools → Options → Projects and Solutions → VC++ Directories
```

Thêm các đường dẫn:
```
Include Directories:
  - $(ProjectDir)\..\Core
  - $(ProjectDir)\..\Engine
  - $(ProjectDir)\..\Kernel
  - $(ProjectDir)\..\Network
  - $(ProjectDir)\..\zbuffer\client

Library Directories:
  - $(ProjectDir)\..\Core\Release
  - $(ProjectDir)\..\Engine\Release
  - $(ProjectDir)\..\zbuffer\client\Release
```

---

## 2. Thứ Tự Build

⚠️ **QUAN TRỌNG**: Phải build theo đúng thứ tự sau vì có dependencies giữa các project.

```
Thứ tự build:
1. zbuffer (client & server)    ← Không phụ thuộc gì
2. Core                          ← Phụ thuộc: zbuffer
3. Engine                        ← Phụ thuộc: Core
4. Kernel                        ← Phụ thuộc: Core, Engine
5. Network                       ← Phụ thuộc: zbuffer
6. S3Client                      ← Phụ thuộc: Core, Engine, Kernel, zbuffer/client
7. S3Server                      ← Phụ thuộc: Core, Kernel, zbuffer/server
8. Anti-Cheat-Client (fProtectClient)   ← Phụ thuộc: Core, Engine
9. Pack                          ← Có thể build độc lập
10. Autoupdate                   ← Có thể build độc lập
11. CoreClient (C#)              ← Có thể build độc lập
12. Project_ProtectJX            ← Build cuối cùng
```

---

## 3. Build Từng Component

### 3.1. Build zbuffer (Networking Layer)

**Đường dẫn**: `ClientAnti_JX1/SwordOnline/Sources/zbuffer/`

```bash
# Mở solution
ClientAnti_JX1/SwordOnline/Sources/zbuffer/zbuffer.sln
```

**Các bước:**

1. **Build zbuffer/client**:
   - Click chuột phải vào project `client`
   - Chọn `Build`
   - Configuration: `Release | Win32`
   - Output: `zbuffer/client/Release/client.lib`

2. **Build zbuffer/server**:
   - Click chuột phải vào project `server`
   - Chọn `Build`
   - Configuration: `Release | Win32`
   - Output: `zbuffer/server/Release/server.lib`

**Kiểm tra:**
```
✅ zbuffer/client/Release/client.lib
✅ zbuffer/server/Release/server.lib
```

---

### 3.2. Build Core (Thư Viện Cốt Lõi)

**Đường dẫn**: `ClientAnti_JX1/SwordOnline/Sources/Core/`

```bash
# Mở solution (hoặc project file)
ClientAnti_JX1/SwordOnline/Sources/S3Client/S3Client.sln
# Core project sẽ có trong solution này
```

**Các bước:**

1. Trong Solution Explorer, tìm project `Core`
2. Click chuột phải → `Build`
3. Configuration: `Client Release | Win32`
4. Output: `Core/Release/Core.lib` hoặc `Core/Client Release/Core.lib`

**Lưu ý:**
- Core có 2 configurations: `Client Release` và `Server Release`
- Build cả 2 nếu cần cả client và server

**Kiểm tra:**
```
✅ Core/Client Release/Core.lib
✅ Core/Server Release/Core.lib (nếu build server)
```

---

### 3.3. Build Engine

**Đường dẫn**: `ClientAnti_JX1/SwordOnline/Sources/Engine/`

**Các bước:**

1. Mở Engine.vcproj/Engine.vcxproj
2. Đảm bảo đã link đúng Core.lib:
   ```
   Project Properties → Linker → Input → Additional Dependencies
   → Thêm: ../Core/Release/Core.lib
   ```
3. Build với configuration `Release | Win32`
4. Output: `Engine/Release/Engine.lib`

**Kiểm tra:**
```
✅ Engine/Release/Engine.lib
```

---

### 3.4. Build Kernel

**Đường dẫn**: `ClientAnti_JX1/SwordOnline/Sources/Kernel/`

**Các bước:**

1. Mở Kernel.vcproj/Kernel.vcxproj
2. Đảm bảo link đúng dependencies:
   - Core.lib
   - Engine.lib
3. Build với configuration `Release | Win32`
4. Output: `Kernel/Release/Kernel.lib`

**Kiểm tra:**
```
✅ Kernel/Release/Kernel.lib
```

---

### 3.5. Build Network

**Đường dẫn**: `ClientAnti_JX1/SwordOnline/Sources/Network/`

**Các bước:**

1. Mở Network.vcproj/Network.vcxproj
2. Đảm bảo link đúng zbuffer/client.lib
3. Build với configuration `Release | Win32`
4. Output: `Network/Release/Network.lib`

---

### 3.6. Build S3Client (Game Client Chính)

**Đường dẫn**: `ClientAnti_JX1/SwordOnline/Sources/S3Client/`

```bash
# Mở solution
ClientAnti_JX1/SwordOnline/Sources/S3Client/S3Client.sln
```

**Các bước:**

1. Kiểm tra dependencies đã build:
   - ✅ Core.lib
   - ✅ Engine.lib
   - ✅ Kernel.lib
   - ✅ zbuffer/client.lib

2. Set S3Client làm StartUp Project (click chuột phải → Set as StartUp Project)

3. Kiểm tra Linker settings:
   ```
   Project Properties → Linker → Input → Additional Dependencies
   ```
   Đảm bảo có:
   ```
   ../Core/Release/Core.lib
   ../Engine/Release/Engine.lib
   ../Kernel/Release/Kernel.lib
   ../zbuffer/client/Release/client.lib
   ws2_32.lib
   winmm.lib
   ```

4. Build với configuration `Release | Win32`

5. Output: `S3Client/Release/S3Client.exe`

**Kiểm tra:**
```
✅ S3Client/Release/S3Client.exe
✅ S3Client/Release/*.dll (nếu có)
```

---

### 3.7. Build S3Server (Game Server)

**Đường dẫn**: `ClientAnti_JX1/SwordOnline/Sources/S3Server/`

**Các bước:**

1. Tương tự S3Client nhưng dùng `Server Release` configuration
2. Link với zbuffer/server.lib thay vì client.lib
3. Build với configuration `Server Release | Win32`
4. Output: `S3Server/Release/S3Server.exe`

---

### 3.8. Build Anti-Cheat-Client (Anti-Cheat Protection)

**Đường dẫn**: `ClientAnti_JX1/SwordOnline/Sources/Anti-Cheat-Client/`

```bash
# Mở solution
ClientAnti_JX1/SwordOnline/Sources/fProtectClient.sln
```

**Các bước:**

1. Project này là `fProtectClient` trong solution
2. Đảm bảo đã link:
   - Core.lib
   - Engine.lib
   - detours.lib (đã có trong thư mục Anti-Cheat-Client)

3. Build với configuration `Release | Win32`

4. Output: `Anti-Cheat-Client/Release/fProtectClient.dll`

**Lưu ý:**
- Module này inject vào S3Client.exe
- Chứa các check chống hack: process, memory, file integrity

**Kiểm tra:**
```
✅ Anti-Cheat-Client/Release/fProtectClient.dll
```

---

### 3.9. Build Pack (Packing Tool)

**Đường dẫn**: `ClientAnti_JX1/SwordOnline/Sources/Pack/`

```bash
# Mở solution
ClientAnti_JX1/SwordOnline/Sources/Pack/Pack.sln
```

**Các bước:**

1. Build độc lập, không phụ thuộc nhiều vào các lib khác
2. Configuration: `Release | Win32`
3. Output: `Pack/Release/Pack.exe`

**Chức năng:**
- Đóng gói file game
- Nén và mã hóa assets

---

### 3.10. Build Autoupdate

**Đường dẫn**: `ClientAnti_JX1/SwordOnline/Sources/Autoupdate/`

```bash
# Mở solution
ClientAnti_JX1/SwordOnline/Sources/Autoupdate/Autoupdate.sln
```

**Các bước:**

1. Build độc lập
2. Configuration: `Release | Win32`
3. Output: `Autoupdate/Release/Autoupdate.exe`

---

### 3.11. Build CoreClient (C#)

**Đường dẫn**: `ClientAnti_JX1/SwordOnline/Sources/CoreClient/`

**Các bước:**

1. Mở CoreClient.csproj bằng Visual Studio
2. Restore NuGet packages (nếu có)
3. Build → Build Solution
4. Configuration: `Release | Any CPU`
5. Output: `CoreClient/bin/Release/CoreClient.dll`

---

### 3.12. Build Project_ProtectJX (Wrapper/Loader)

**Đường dẫn**: `Project_ProtectJX/`

```bash
# Mở solution
Project_ProtectJX/JxOnline.sln
```

**Các bước:**

1. Đây là project C++ wrapper/loader cho game client
2. Đảm bảo S3Client.exe đã build xong
3. Build với configuration `Release | x86`
4. Output: `Project_ProtectJX/Project1/Release/Safe.exe` hoặc `JxOnline.exe`

**Chức năng:**
- Load và protect S3Client.exe
- Inject fProtectClient.dll
- Apply WinLicense protection

---

## 4. Xử Lý Lỗi Thường Gặp

### 4.1. Lỗi "Cannot open file 'xxx.lib'"

**Nguyên nhân**: Chưa build dependency trước

**Giải pháp:**
```bash
1. Kiểm tra lại thứ tự build
2. Build lại dependency bị thiếu
3. Kiểm tra đường dẫn linker trong Project Properties
```

### 4.2. Lỗi "Unresolved external symbol"

**Nguyên nhân**:
- Thiếu lib file trong linker
- Function/class không được implement

**Giải pháp:**
```bash
# Kiểm tra linker settings
Project Properties → Linker → Input → Additional Dependencies

# Đảm bảo có đủ các .lib cần thiết
```

### 4.3. Lỗi "Platform toolset not found"

**Nguyên nhân**: Project dùng toolset cũ hơn VS hiện tại

**Giải pháp:**
```bash
# Chuột phải vào project → Properties
# General → Platform Toolset
# Chọn toolset phù hợp với VS của bạn (v141, v142, v143)
```

### 4.4. Lỗi "Windows SDK version not found"

**Nguyên nhân**: Project yêu cầu SDK version không có

**Giải pháp:**
```bash
# Chuột phải vào project → Properties
# General → Windows SDK Version
# Chọn version đã cài (10.0.xxxxx.0)
```

### 4.5. Lỗi DirectX Headers

**Nguyên nhân**: Thiếu DirectX SDK

**Giải pháp:**
```bash
1. Download DirectX SDK (June 2010)
2. Cài đặt
3. Thêm vào Include Directories:
   C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include
4. Thêm vào Library Directories:
   C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Lib\x86
```

### 4.6. Lỗi C++ Standard Version

**Nguyên nhân**: Code dùng C++ features mới hơn

**Giải pháp:**
```bash
Project Properties → C/C++ → Language → C++ Language Standard
→ Chọn: ISO C++14 hoặc C++17
```

---

## 5. Kiểm Tra Build

### 5.1. Checklist Build Thành Công

Sau khi build xong, kiểm tra các file sau:

#### Core Libraries:
```
✅ zbuffer/client/Release/client.lib
✅ zbuffer/server/Release/server.lib
✅ Core/Release/Core.lib
✅ Engine/Release/Engine.lib
✅ Kernel/Release/Kernel.lib
✅ Network/Release/Network.lib
```

#### Executables:
```
✅ S3Client/Release/S3Client.exe
✅ S3Server/Release/S3Server.exe
✅ Pack/Release/Pack.exe
✅ Autoupdate/Release/Autoupdate.exe
```

#### Protection/Helper:
```
✅ Anti-Cheat-Client/Release/fProtectClient.dll
✅ CoreClient/bin/Release/CoreClient.dll
✅ Project_ProtectJX/Project1/Release/Safe.exe
```

### 5.2. Test Build

**Test S3Client:**
```bash
cd ClientAnti_JX1/SwordOnline/Sources/S3Client/Release
S3Client.exe

# Kiểm tra:
- Ứng dụng mở được không
- Có log lỗi gì không
- Check file Logs/*.log
```

**Test với fProtectClient.dll:**
```bash
# Copy fProtectClient.dll vào thư mục chứa S3Client.exe
# Chạy lại S3Client.exe
# Kiểm tra protection module có load không
```

### 5.3. Batch Build Script

Để build tất cả một lúc, có thể dùng script:

```batch
@echo off
echo Building Sword Online Project...

REM Build zbuffer
echo [1/12] Building zbuffer...
devenv "ClientAnti_JX1\SwordOnline\Sources\zbuffer\zbuffer.sln" /build "Release|Win32"

REM Build Core
echo [2/12] Building Core...
devenv "ClientAnti_JX1\SwordOnline\Sources\S3Client\S3Client.sln" /build "Client Release|Win32" /project Core

REM Build Engine
echo [3/12] Building Engine...
devenv "ClientAnti_JX1\SwordOnline\Sources\Engine\Engine.vcxproj" /build "Release|Win32"

REM ... (tiếp tục cho các project khác)

echo Build completed!
pause
```

---

## 6. Build Configurations

### 6.1. Debug vs Release

| Configuration | Mục đích | Optimization | Debug Info | Size |
|---------------|----------|--------------|------------|------|
| **Debug** | Development | Tắt | Có | Lớn |
| **Release** | Production | Bật | Không | Nhỏ |

**Khi nào dùng Debug:**
- Đang code và test
- Cần debug với breakpoint
- Trace lỗi

**Khi nào dùng Release:**
- Build để release cho user
- Performance testing
- Production deployment

### 6.2. Client vs Server Configurations

Một số project có cả `Client Release` và `Server Release`:

- **Client Release**: Build cho game client
- **Server Release**: Build cho game server

Chú ý defines và settings khác nhau giữa 2 modes.

---

## 7. Tips & Best Practices

### 7.1. Build Performance

**Parallel Build:**
```
Tools → Options → Projects and Solutions → Build and Run
→ Maximum number of parallel project builds: 8 (tùy CPU)
```

**Incremental Build:**
- Chỉ build project thay đổi
- Dùng `Build` thay vì `Rebuild` khi có thể

### 7.2. Clean Build

Khi gặp lỗi lạ:
```bash
1. Clean Solution (Build → Clean Solution)
2. Xóa thủ công các thư mục Debug/Release
3. Xóa .vs, .suo, .user files
4. Rebuild từ đầu
```

### 7.3. Version Control

Không commit vào git:
```
*.obj
*.pch
*.pdb
*.ilk
*.exe (trừ binaries release)
*.dll (trừ dependencies)
Debug/
Release/
.vs/
*.suo
*.user
```

---

## Tổng Kết

Sau khi hoàn thành tất cả các bước build:

1. ✅ Kiểm tra tất cả .lib và .exe/.dll đã build xong
2. ✅ Copy các file cần thiết vào thư mục deploy
3. ✅ Test chạy S3Client.exe
4. ✅ Đọc tiếp [USAGE_GUIDE.md](USAGE_GUIDE.md) để biết cách chạy game

---

**Thời gian build ước tính**: 15-30 phút (tùy máy)

**Dung lượng output**: ~500MB - 1GB (bao gồm cả debug symbols)
