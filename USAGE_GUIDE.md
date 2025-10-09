# Hướng Dẫn Sử Dụng Sword Online

## Mục Lục
1. [Cài Đặt và Chuẩn Bị](#1-cài-đặt-và-chuẩn-bị)
2. [Chạy Game Client](#2-chạy-game-client)
3. [Chạy Game Server](#3-chạy-game-server)
4. [Sử Dụng Các Công Cụ](#4-sử-dụng-các-công-cụ)
5. [Cấu Hình Game](#5-cấu-hình-game)
6. [Troubleshooting](#6-troubleshooting)

---

## 1. Cài Đặt và Chuẩn Bị

### 1.1. Yêu Cầu Hệ Thống

**Minimum:**
- OS: Windows 7 (32-bit/64-bit)
- CPU: Intel Core 2 Duo hoặc tương đương
- RAM: 2GB
- GPU: DirectX 9 compatible
- HDD: 5GB trống

**Recommended:**
- OS: Windows 10/11 (64-bit)
- CPU: Intel Core i5 hoặc tương đương
- RAM: 4GB+
- GPU: DirectX 11 compatible
- SSD: 10GB trống

### 1.2. Cấu Trúc Thư Mục Game

Sau khi build xong, tổ chức thư mục như sau:

```
GameClient/                      # Thư mục game client
├── S3Client.exe                 # Game client executable
├── fProtectClient.dll           # Anti-cheat module
├── CoreClient.dll               # Core client library
├── *.dll                        # Các DLL dependencies
├── Data/                        # Game data
│   ├── Spr/                    # Sprites
│   ├── Ui/                     # UI resources
│   ├── Sound/                  # Sound files
│   ├── Music/                  # Music files
│   └── Script/                 # Lua scripts
├── Settings/                    # Cấu hình
│   ├── client.ini              # Client config
│   └── server.ini              # Server connection config
├── Logs/                        # Log files
└── SaveData/                    # Saved game data

GameServer/                      # Thư mục game server
├── S3Server.exe                 # Server executable
├── Settings/                    # Server configs
├── Data/                        # Server data
├── Scripts/                     # Server scripts
└── Logs/                        # Server logs

Tools/                           # Công cụ
├── Pack.exe                     # Packing tool
├── Autoupdate.exe               # Update tool
└── WinLicense/                  # License tools
```

### 1.3. Copy Files Sau Build

**Bước 1: Tạo thư mục deploy**
```bash
mkdir GameDeploy
cd GameDeploy
mkdir GameClient GameServer Tools
```

**Bước 2: Copy client files**
```bash
# Copy executable và DLLs
copy Sources\S3Client\Release\S3Client.exe GameDeploy\GameClient\
copy Sources\Anti-Cheat-Client\Release\fProtectClient.dll GameDeploy\GameClient\
copy Sources\CoreClient\bin\Release\CoreClient.dll GameDeploy\GameClient\

# Copy dependencies (nếu có)
copy Sources\Engine\Release\*.dll GameDeploy\GameClient\
```

**Bước 3: Copy server files**
```bash
copy Sources\S3Server\Release\S3Server.exe GameDeploy\GameServer\
```

**Bước 4: Copy resources**
```bash
# Copy toàn bộ thư mục Data, Settings
xcopy /E /I Sources\ClientRelease\Data GameDeploy\GameClient\Data
xcopy /E /I Sources\ClientRelease\Settings GameDeploy\GameClient\Settings
```

**Bước 5: Copy tools**
```bash
copy Sources\Pack\Release\Pack.exe GameDeploy\Tools\
copy Sources\Autoupdate\Release\Autoupdate.exe GameDeploy\Tools\
```

---

## 2. Chạy Game Client

### 2.1. Chạy Client Cơ Bản

**Cách 1: Chạy trực tiếp**
```bash
cd GameDeploy\GameClient
S3Client.exe
```

**Cách 2: Chạy với Protection Wrapper**
```bash
# Nếu đã build Project_ProtectJX
cd Project_ProtectJX\Project1\Release
Safe.exe
```

### 2.2. Tham Số Command Line

S3Client.exe hỗ trợ các tham số:

```bash
S3Client.exe [options]

Options:
  -server <ip>          Địa chỉ IP server (mặc định: localhost)
  -port <port>          Port kết nối (mặc định: 5001)
  -windowed             Chạy ở chế độ cửa sổ (không fullscreen)
  -width <width>        Chiều rộng cửa sổ (mặc định: 1024)
  -height <height>      Chiều cao cửa sổ (mặc định: 768)
  -nolog               Tắt logging
  -debug               Bật debug mode

# Ví dụ:
S3Client.exe -server 192.168.1.100 -port 5001 -windowed -width 1280 -height 720
```

### 2.3. Đăng Nhập Game

**Bước 1: Khởi động game**
- Double-click S3Client.exe
- Đợi splash screen load

**Bước 2: Màn hình đăng nhập**
- Nhập username
- Nhập password
- Click "Đăng nhập" hoặc nhấn Enter

**Bước 3: Chọn server**
- Chọn server từ danh sách
- Click "Vào game"

**Bước 4: Chọn nhân vật**
- Chọn nhân vật có sẵn hoặc tạo mới
- Click "Bắt đầu"

### 2.4. Controls Cơ Bản

**Di chuyển:**
- Mouse: Click chuột trái để di chuyển
- Keyboard: W/A/S/D hoặc mũi tên

**Combat:**
- Space: Tấn công cơ bản
- 1-9: Sử dụng skill (skill bar)
- Tab: Chuyển target
- Z: Nhặt đồ

**UI:**
- I: Mở inventory
- C: Mở character info
- K: Mở skill tree
- M: Mở map
- ESC: Mở menu

**Chat:**
- Enter: Mở chat box
- /say <message>: Chat gần
- /party <message>: Chat party
- /guild <message>: Chat guild
- /whisper <name> <message>: Chat riêng

### 2.5. Cấu Hình Graphics

**In-Game Settings:**
```
ESC → Cài đặt → Đồ họa

- Resolution: 800x600, 1024x768, 1280x720, 1920x1080
- Fullscreen: Bật/Tắt
- Shadow Quality: Low/Medium/High
- Effects Quality: Low/Medium/High
- Anti-aliasing: Bật/Tắt
```

**File Config (client.ini):**
```ini
[Graphics]
Width=1024
Height=768
Fullscreen=1
ShadowQuality=2
EffectsQuality=2
AntiAliasing=1

[Performance]
FPSLimit=60
VSync=1
```

---

## 3. Chạy Game Server

### 3.1. Chuẩn Bị Database

**Bước 1: Cài MySQL/MariaDB**
```bash
# Download và cài MySQL Server
# Tạo database cho game
```

**Bước 2: Import schema**
```sql
mysql -u root -p
CREATE DATABASE sword_online;
USE sword_online;
SOURCE db/schema.sql;
```

**Bước 3: Cấu hình kết nối database**

File: `GameServer/Settings/database.ini`
```ini
[Database]
Host=localhost
Port=3306
Username=root
Password=yourpassword
Database=sword_online
```

### 3.2. Chạy Server

**Bước 1: Kiểm tra config**

File: `GameServer/Settings/server.ini`
```ini
[Server]
Name=Sword Online Server
Port=5001
MaxPlayers=1000
ThreadCount=4

[GameWorld]
Map=World01
SaveInterval=300
```

**Bước 2: Khởi động server**
```bash
cd GameDeploy\GameServer
S3Server.exe

# Hoặc với config file khác:
S3Server.exe -config custom_config.ini
```

**Bước 3: Kiểm tra server đã chạy**
```
Console output:
[INFO] Loading server configuration...
[INFO] Connecting to database...
[INFO] Database connected successfully.
[INFO] Loading game world...
[INFO] Server started on port 5001
[INFO] Waiting for connections...
```

### 3.3. Quản Lý Server

**Commands trong Server Console:**
```bash
# Xem danh sách player online
/list

# Kick player
/kick <playername> <reason>

# Ban player
/ban <playername> <duration> <reason>

# Broadcast message
/broadcast <message>

# Reload config
/reload

# Save game
/save

# Shutdown server (graceful)
/shutdown

# Shutdown ngay lập tức
/stop
```

### 3.4. Monitoring Server

**Xem logs:**
```bash
# Real-time log
tail -f GameServer/Logs/server.log

# Windows
type GameServer\Logs\server.log
```

**Log formats:**
```
[2025-10-10 10:30:45] [INFO] Player "TestUser" connected from 192.168.1.100
[2025-10-10 10:31:20] [WARN] Database query slow: 1.5s
[2025-10-10 10:32:00] [ERROR] Failed to save player data: Connection timeout
```

---

## 4. Sử Dụng Các Công Cụ

### 4.1. Pack Tool (Đóng Gói Assets)

**Mục đích**: Đóng gói các file assets (sprites, sounds, etc.) thành .pak files

**Cách dùng:**
```bash
cd Tools
Pack.exe [options]

Options:
  -input <directory>    Thư mục chứa file cần pack
  -output <file>        File .pak output
  -compress            Nén file (khuyến nghị)
  -encrypt             Mã hóa file (khuyến nghị)

# Ví dụ: Pack toàn bộ Data/Spr
Pack.exe -input ..\GameClient\Data\Spr -output ..\GameClient\Data\Spr.pak -compress -encrypt
```

**Unpack (để edit):**
```bash
Pack.exe -unpack -input ..\GameClient\Data\Spr.pak -output ..\GameClient\Data\Spr_Extracted
```

### 4.2. Autoupdate Tool (Cập Nhật Game)

**Cách dùng:**

**Server side: Tạo update package**
```bash
cd Tools
Autoupdate.exe -create -version 1.0.1 -source ..\GameClient -output Updates\v1.0.1.update
```

**Client side: Apply update**
```bash
# Tự động chạy khi mở game (nếu cấu hình)
# Hoặc chạy thủ công:
Autoupdate.exe -apply -update Updates\v1.0.1.update -target ..\GameClient
```

**Cấu hình auto-update:**

File: `GameClient/Settings/autoupdate.ini`
```ini
[AutoUpdate]
Enabled=1
UpdateServer=http://updates.yourgame.com
CheckInterval=3600
AutoDownload=1
AutoApply=0
```

### 4.3. WinLicense Tools (License Management)

**Generate License:**
```bash
cd Sources\WinLicense\WinLicenseSDK\Generators\LicenseGenerator
LicenseGenerator.exe

# GUI sẽ mở ra:
1. Chọn License Type: Trial/Full
2. Nhập User Name
3. Nhập Hardware ID (nếu cần lock hardware)
4. Set Expiration Date
5. Click "Generate"
6. Save license file (.lic)
```

**Apply License vào Game:**
```bash
# Copy .lic file vào thư mục game
copy license.lic GameClient\

# Khi game chạy sẽ tự động check license
```

**Check Hardware ID:**
```bash
cd Sources\WinLicense\WinLicenseSDK\ExamplesSDK\Get HardwareID
GetHardwareID.exe
# Copy Hardware ID để generate license
```

---

## 5. Cấu Hình Game

### 5.1. Client Configuration

**File: GameClient/Settings/client.ini**

```ini
[Graphics]
Width=1024
Height=768
Fullscreen=1
ShadowQuality=2          # 0=Off, 1=Low, 2=Medium, 3=High
EffectsQuality=2
AntiAliasing=1
VSync=1

[Performance]
FPSLimit=60
RenderDistance=100
MaxParticles=1000

[Audio]
MasterVolume=100         # 0-100
MusicVolume=80
SoundVolume=90
Enable3DSound=1

[Network]
ServerIP=127.0.0.1
ServerPort=5001
Timeout=30
KeepAlive=10

[Game]
Language=vi-VN           # vi-VN, en-US
CameraDistance=15
AutoTarget=1
ShowDamageNumbers=1
ShowPlayerNames=1

[Protection]
EnableAntiCheat=1
ReportSuspicious=1
```

### 5.2. Server Configuration

**File: GameServer/Settings/server.ini**

```ini
[Server]
Name=Sword Online Server
Port=5001
MaxPlayers=1000
ThreadCount=4
TickRate=20              # Updates per second

[Database]
ConfigFile=database.ini

[GameWorld]
StartMap=Village01
SaveInterval=300         # Seconds
BackupInterval=3600
MaxLevel=200
BaseExpRate=1.0
DropRate=1.0

[Combat]
PvPEnabled=1
PvPLevelGap=10          # Max level gap for PvP
PKPenalty=1

[Economy]
StartingGold=10000
TradeEnabled=1
TradeTax=0.05           # 5%

[Security]
RequireAntiCheat=1
MaxLoginAttempts=3
BanDuration=3600        # Seconds
LogAllActions=1
```

### 5.3. Script Configuration

**Lua Scripts**: `GameClient/Data/Script/`

Ví dụ script tùy chỉnh UI:

**File: Data/Script/UI/MainMenu.lua**
```lua
-- Tùy chỉnh main menu
function OnMainMenuLoad()
    -- Set position
    SetWindowPosition("MainMenu", 100, 100)

    -- Set button text
    SetButtonText("btnStart", "Bắt đầu chơi")
    SetButtonText("btnSettings", "Cài đặt")
    SetButtonText("btnExit", "Thoát game")
end

function OnStartButtonClick()
    -- Custom logic khi click Start
    print("Start button clicked")
    LoadCharacterSelection()
end
```

**Reload scripts in-game:**
```
# Nhấn F12 (nếu debug mode bật)
# Hoặc dùng command:
/reloadscripts
```

---

## 6. Troubleshooting

### 6.1. Game Không Khởi Động

**Triệu chứng**: Click S3Client.exe nhưng không có gì xảy ra

**Giải pháp:**

1. **Kiểm tra dependencies:**
```bash
# Dùng Dependency Walker hoặc check manually
# Đảm bảo có đủ các .dll cần thiết
```

2. **Kiểm tra log:**
```bash
# Xem file Logs/client.log
type GameClient\Logs\client.log
```

3. **Chạy với admin:**
```bash
# Click chuột phải → Run as Administrator
```

4. **Kiểm tra DirectX:**
```bash
# Cài lại DirectX Runtime (June 2010)
dxwebsetup.exe
```

### 6.2. Lỗi "Cannot connect to server"

**Nguyên nhân:**
- Server chưa chạy
- Sai IP/Port
- Firewall block

**Giải pháp:**

1. **Kiểm tra server đang chạy:**
```bash
# Check server console
# Hoặc dùng telnet
telnet localhost 5001
```

2. **Kiểm tra firewall:**
```bash
# Windows Firewall → Allow app
# Thêm S3Client.exe và S3Server.exe vào whitelist
```

3. **Kiểm tra config:**
```ini
# File: GameClient/Settings/client.ini
[Network]
ServerIP=127.0.0.1      # Đổi thành IP server thật
ServerPort=5001
```

### 6.3. Lỗi Graphics/Render

**Triệu chứng**:
- Màn hình đen
- Texture bị lỗi
- FPS thấp

**Giải pháp:**

1. **Update graphics driver**
2. **Giảm settings:**
```ini
[Graphics]
ShadowQuality=0
EffectsQuality=1
AntiAliasing=0
```

3. **Chạy ở chế độ windowed:**
```bash
S3Client.exe -windowed -width 800 -height 600
```

4. **Compatibility mode:**
```bash
# Properties → Compatibility
# Chọn: Windows 7
# Tick: Disable fullscreen optimizations
```

### 6.4. Lỗi "License expired" hoặc "Invalid license"

**Nguyên nhân:**
- Trial đã hết hạn
- License file bị corrupt
- Hardware ID thay đổi

**Giải pháp:**

1. **Generate license mới:**
```bash
cd Tools\WinLicense
LicenseGenerator.exe
# Generate license mới
```

2. **Copy license file:**
```bash
copy license.lic GameClient\
```

3. **Nếu lock hardware, get hardware ID:**
```bash
GetHardwareID.exe
# Copy ID và generate license với ID này
```

### 6.5. Game Crash khi chơi

**Giải pháp:**

1. **Kiểm tra log:**
```bash
# File: Logs/crash.log hoặc Logs/client.log
# Tìm dòng [ERROR] hoặc stack trace
```

2. **Disable protection tạm thời (để test):**
```bash
# Xóa hoặc rename fProtectClient.dll
ren fProtectClient.dll fProtectClient.dll.bak
```

3. **Verify game files:**
```bash
# So sánh với bản build gốc
# Copy lại files bị thiếu/corrupt
```

4. **Run debug build:**
```bash
# Build lại với Debug configuration
# Chạy trong Visual Studio với debugger
# Xem crash ở đâu
```

### 6.6. Server Crash hoặc Lag

**Triệu chứng:**
- Server tự tắt
- High CPU/RAM usage
- Players bị disconnect

**Giải pháp:**

1. **Kiểm tra log:**
```bash
tail -f GameServer/Logs/server.log
# Tìm [ERROR] hoặc [FATAL]
```

2. **Kiểm tra resources:**
```bash
# Task Manager
# Xem CPU, RAM, Network usage
```

3. **Database optimization:**
```sql
-- Optimize tables
OPTIMIZE TABLE players;
OPTIMIZE TABLE items;
```

4. **Tăng thread count:**
```ini
[Server]
ThreadCount=8           # Tăng lên nếu có nhiều players
```

5. **Restart server định kỳ:**
```bash
# Setup cron job hoặc scheduled task
# Restart server mỗi 6-12 giờ
```

---

## 7. Tips & Tricks

### 7.1. Tối Ưu Performance

**Client side:**
- Giảm render distance
- Tắt shadows nếu GPU yếu
- Giới hạn FPS ở 60
- Đóng background apps

**Server side:**
- Tăng RAM cho database
- Use SSD cho server files
- Optimize queries (add indexes)
- Caching frequently accessed data

### 7.2. Backup & Recovery

**Backup game data:**
```bash
# Backup player saves
xcopy /E /I GameClient\SaveData Backup\SaveData_%DATE%

# Backup server database
mysqldump -u root -p sword_online > backup_db_%DATE%.sql
```

**Recovery:**
```bash
# Restore saves
xcopy /E /I Backup\SaveData_2025-10-10 GameClient\SaveData

# Restore database
mysql -u root -p sword_online < backup_db_2025-10-10.sql
```

### 7.3. Multi-Instance (Chạy nhiều client)

**Để chạy nhiều client cùng lúc:**

1. Copy toàn bộ thư mục GameClient:
```bash
xcopy /E /I GameClient GameClient2
```

2. Đổi config trong GameClient2:
```ini
# client.ini
[Network]
LocalPort=5002          # Đổi port khác
```

3. Chạy cả 2:
```bash
start GameClient\S3Client.exe
start GameClient2\S3Client.exe
```

---

## Tổng Kết

Sau khi làm theo hướng dẫn:

1. ✅ Game client chạy được
2. ✅ Kết nối được tới server
3. ✅ Có thể đăng nhập và chơi
4. ✅ Biết cách config và troubleshoot

**Next steps:**
- Đọc [DEVELOPMENT_GUIDE.md](DEVELOPMENT_GUIDE.md) nếu muốn chỉnh sửa code
- Tham khảo API documentation (nếu có)
- Join community để hỏi đáp

---

**Liên hệ hỗ trợ**: [Thêm thông tin liên hệ]

**Forum**: [Link forum nếu có]
