# Sword Online - Hệ Thống Game và Anti-Cheat

## Tổng Quan

Đây là mã nguồn của game **Sword Online** (Võ Lâm Truyền Kỳ) bao gồm:
- Game client và server
- Hệ thống bảo vệ chống hack/cheat
- Công cụ đóng gói và cập nhật
- Hệ thống license protection

## Cấu Trúc Dự Án

```
ANTI_JX/
├── ClientAnti_JX1/
│   └── SwordOnline/
│       └── Sources/              # Mã nguồn chính
│           ├── zbuffer/          # Networking layer (client/server)
│           ├── Core/             # Thư viện cốt lõi
│           ├── Engine/           # Game engine
│           ├── Kernel/           # Kernel layer
│           ├── Network/          # Network utilities
│           ├── S3Client/         # Game client chính
│           ├── S3Server/         # Game server
│           ├── Anti-Cheat-Client/       # Module chống hack
│           ├── Pack/             # Công cụ đóng gói
│           ├── Autoupdate/       # Hệ thống cập nhật tự động
│           ├── WinLicense/       # License protection SDK
│           ├── CoreClient/       # Core library cho client (C#)
│           └── ...               # Các module khác
│
└── Project_ProtectJX/
    └── JxOnline.sln              # Project bảo vệ game client
```

## Các Component Chính

### 1. **zbuffer** (Networking Layer)
- **client**: Client networking library
- **server**: Server networking library
- Xử lý giao tiếp mạng giữa client và server

### 2. **Core** (Thư Viện Cốt Lõi)
- Chứa các function và class cơ bản được dùng chung
- Được sử dụng bởi cả client và server

### 3. **S3Client** (Game Client)
- Game client chính
- Phụ thuộc vào: Core, Engine, zbuffer/client
- Xử lý giao diện, gameplay, render

### 4. **S3Server** (Game Server)
- Game server chính
- Xử lý logic game, player data, world state

### 5. **Anti-Cheat-Client/fProtectClient** (Anti-Cheat)
- Module bảo vệ chống hack và cheat
- Kiểm tra process, memory, file integrity
- Giao tiếp với server để xác thực

### 6. **Pack** (Packing Tool)
- Công cụ đóng gói file game
- Nén và mã hóa assets

### 7. **Autoupdate** (Auto-Update System)
- Hệ thống cập nhật tự động
- Download và apply patches

### 8. **WinLicense** (License Protection)
- SDK bảo vệ bản quyền
- Trial management, license generation
- Anti-debug, anti-crack

## Yêu Cầu Hệ Thống

### Môi Trường Phát Triển
- **IDE**: Visual Studio 2017 hoặc mới hơn
- **OS**: Windows 7/8/10/11
- **Platform Toolset**: v141 hoặc tương thích
- **SDK**: Windows SDK 10.0 trở lên

### Thư Viện Phụ Thuộc
- DirectX SDK (cho graphics rendering)
- Lua (cho scripting - đã tích hợp)
- Detours (cho hooking - đã tích hợp)
- WinLicense SDK (cho protection)

## Tài Liệu Chi Tiết

Xem các file hướng dẫn sau để biết thêm chi tiết:

1. **[BUILD_GUIDE.md](BUILD_GUIDE.md)** - Hướng dẫn build từng component theo đúng thứ tự
2. **[USAGE_GUIDE.md](USAGE_GUIDE.md)** - Hướng dẫn sử dụng và chạy game
3. **[DEVELOPMENT_GUIDE.md](DEVELOPMENT_GUIDE.md)** - Hướng dẫn chỉnh sửa và phát triển

## Quick Start

### Build Toàn Bộ Dự Án

```bash
# 1. Build zbuffer (client/server)
# Mở ClientAnti_JX1/SwordOnline/Sources/zbuffer/zbuffer.sln
# Build cả client và server projects

# 2. Build Core
# Mở ClientAnti_JX1/SwordOnline/Sources/Core/Core.vcproj
# Build project

# 3. Build S3Client
# Mở ClientAnti_JX1/SwordOnline/Sources/S3Client/S3Client.sln
# Build project

# 4. Build Anti-Cheat-Client (Protection)
# Mở ClientAnti_JX1/SwordOnline/Sources/fProtectClient.sln
# Build project

# Chi tiết xem BUILD_GUIDE.md
```

### Chạy Game

```bash
# 1. Copy file build output vào thư mục game
# 2. Copy resources (assets, config files)
# 3. Chạy S3Client.exe

# Chi tiết xem USAGE_GUIDE.md
```

## Lưu Ý Quan Trọng

⚠️ **Bảo Mật**:
- Không chia sẻ mã nguồn module Anti-Cheat-Client/Protection
- Bảo vệ các key/certificate trong WinLicense

⚠️ **Dependencies**:
- Phải build theo đúng thứ tự (zbuffer → Core → S3Client)
- Đảm bảo các .lib file được link đúng

⚠️ **Testing**:
- Test kỹ trước khi release
- Kiểm tra anti-cheat hoạt động đúng
- Verify license protection

## Liên Hệ & Hỗ Trợ

- Xem tài liệu trong thư mục `docs/` (nếu có)
- Check log files trong `Logs/` khi gặp lỗi
- Đọc code comments để hiểu logic chi tiết
- Face : AD Võ Lâm MU
- Zalo : 0978174557
## License

[Thêm thông tin license của dự án nếu có]

---

**Phiên bản**: 1.0
**Ngày cập nhật**: 2025-10-10
