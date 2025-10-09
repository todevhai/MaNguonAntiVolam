# Hướng Dẫn Phát Triển Sword Online

## Mục Lục
1. [Kiến Trúc Hệ Thống](#1-kiến-trúc-hệ-thống)
2. [Cấu Trúc Code](#2-cấu-trúc-code)
3. [Chỉnh Sửa Game Client](#3-chỉnh-sửa-game-client)
4. [Chỉnh Sửa Game Server](#4-chỉnh-sửa-game-server)
5. [Thêm Tính Năng Mới](#5-thêm-tính-năng-mới)
6. [Debug và Testing](#6-debug-và-testing)
7. [Best Practices](#7-best-practices)

---

## 1. Kiến Trúc Hệ Thống

### 1.1. Tổng Quan Kiến Trúc

```
┌─────────────────────────────────────────────────────────────┐
│                        Game Client                          │
│  ┌──────────────────────────────────────────────────────┐  │
│  │  UI Layer (Lua + C++)                                │  │
│  │  - Login, Character Selection, Main UI               │  │
│  └────────────────┬─────────────────────────────────────┘  │
│                   │                                          │
│  ┌────────────────▼─────────────────────────────────────┐  │
│  │  Game Logic Layer (C++)                              │  │
│  │  - Player, NPC, Item, Skill management               │  │
│  └────────────────┬─────────────────────────────────────┘  │
│                   │                                          │
│  ┌────────────────▼─────────────────────────────────────┐  │
│  │  Engine Layer (C++)                                  │  │
│  │  - Rendering, Physics, Animation                     │  │
│  └────────────────┬─────────────────────────────────────┘  │
│                   │                                          │
│  ┌────────────────▼─────────────────────────────────────┐  │
│  │  Core/Kernel Layer (C++)                             │  │
│  │  - Memory, File I/O, Threading                       │  │
│  └────────────────┬─────────────────────────────────────┘  │
│                   │                                          │
│  ┌────────────────▼─────────────────────────────────────┐  │
│  │  Network Layer (zbuffer/client)                      │  │
│  │  - Socket, Protocol handling                         │  │
│  └────────────────┬─────────────────────────────────────┘  │
└───────────────────┼──────────────────────────────────────┘
                    │
                    │  Network Protocol
                    │
┌───────────────────▼──────────────────────────────────────┐
│                      Game Server                          │
│  ┌──────────────────────────────────────────────────────┐│
│  │  Network Layer (zbuffer/server)                      ││
│  └────────────────┬─────────────────────────────────────┘│
│                   │                                        │
│  ┌────────────────▼─────────────────────────────────────┐│
│  │  Game World Manager                                  ││
│  │  - World state, Players, NPCs, Events                ││
│  └────────────────┬─────────────────────────────────────┘│
│                   │                                        │
│  ┌────────────────▼─────────────────────────────────────┐│
│  │  Database Layer                                      ││
│  │  - Player data, Items, Economy                       ││
│  └──────────────────────────────────────────────────────┘│
└───────────────────────────────────────────────────────────┘
```

### 1.2. Data Flow

**Client → Server:**
```
User Input → UI Event → Game Logic → Packet Encoding → Network Send
```

**Server → Client:**
```
Network Receive → Packet Decoding → Game Logic → World Update → Render
```

### 1.3. Key Components

| Component | Ngôn ngữ | Chức năng | File chính |
|-----------|----------|-----------|------------|
| **Core** | C++ | Utilities, Memory, File I/O | Core.h, Core.cpp |
| **Engine** | C++ | Rendering, Scene management | Engine.h, KEngine.cpp |
| **Kernel** | C++ | Game object base classes | KPlayer.h, KNpc.h |
| **S3Client** | C++ | Client application logic | S3Client.cpp, Login/ |
| **S3Server** | C++ | Server application logic | S3Server.cpp, GameWorld/ |
| **zbuffer** | C++ | Network communication | zbuffer.h, protocol.h |
| **UI** | Lua + C++ | User interface scripting | UiInit.cpp, *.lua |
| **Anti-Cheat-Client** | C++ | Anti-cheat protection | Anti-Cheat-Client.cpp, Protect.cpp |

---

## 2. Cấu Trúc Code

### 2.1. Core Module

**Đường dẫn**: `Sources/Core/`

**Các file quan trọng:**

```cpp
// Core.h - Main header
class KCore {
public:
    static bool Init();                    // Khởi tạo core system
    static void Release();                 // Cleanup
    static void ProcessInput();            // Xử lý input
    static void Update(float deltaTime);   // Update logic
};

// KMemory.h - Memory management
void* KMemAlloc(size_t size);
void KMemFree(void* ptr);

// KFile.h - File I/O
class KFile {
public:
    bool Open(const char* filename, int mode);
    int Read(void* buffer, int size);
    int Write(const void* buffer, int size);
    void Close();
};

// KString.h - String utilities
class KString {
    // String manipulation functions
};
```

**Ví dụ sử dụng:**

```cpp
#include "Core.h"
#include "KFile.h"

// Đọc file config
KFile file;
if (file.Open("config.ini", FILE_READ)) {
    char buffer[1024];
    file.Read(buffer, sizeof(buffer));
    // Parse config...
    file.Close();
}
```

### 2.2. Engine Module

**Đường dẫn**: `Sources/Engine/`

**Các file quan trọng:**

```cpp
// KEngine.h - Main engine
class KEngine {
public:
    bool Init(HWND hWnd);
    void Update(float deltaTime);
    void Render();
    void Release();

    // Rendering
    void DrawSprite(int x, int y, const char* sprFile);
    void DrawText(int x, int y, const char* text, DWORD color);

    // Camera
    void SetCameraPosition(int x, int y);
    void GetCameraPosition(int& x, int& y);
};

// KScene.h - Scene management
class KScene {
public:
    void LoadMap(const char* mapName);
    void AddObject(KSceneObject* obj);
    void RemoveObject(KSceneObject* obj);
    void Update(float deltaTime);
    void Render();
};

// KTexture.h - Texture management
class KTexture {
public:
    bool LoadFromFile(const char* filename);
    void Release();
    int GetWidth() const;
    int GetHeight() const;
};
```

### 2.3. Game Logic (Kernel Module)

**Đường dẫn**: `Sources/Kernel/`

**Các file quan trọng:**

```cpp
// KPlayer.h - Player class
class KPlayer : public KCharacter {
public:
    int m_nLevel;
    int m_nExp;
    int m_nHP;
    int m_nMP;
    int m_nStrength;
    int m_nDexterity;
    // ...

    void LevelUp();
    void AddExp(int exp);
    void UseSkill(int skillId, KCharacter* target);
    void EquipItem(KItem* item);
};

// KNpc.h - NPC class
class KNpc : public KCharacter {
public:
    int m_nNpcId;
    int m_nAI;

    void UpdateAI(float deltaTime);
    void Attack(KCharacter* target);
};

// KItem.h - Item class
class KItem {
public:
    int m_nItemId;
    int m_nType;          // Weapon, Armor, Consumable, etc.
    int m_nAttack;
    int m_nDefense;

    void Apply(KPlayer* player);
};

// KSkill.h - Skill system
class KSkill {
public:
    int m_nSkillId;
    int m_nLevel;
    int m_nDamage;
    int m_nManaCost;
    int m_nCooldown;

    bool CanUse(KPlayer* player);
    void Use(KPlayer* caster, KCharacter* target);
};
```

### 2.4. Network Protocol

**Đường dẫn**: `Sources/Network/` và `Sources/zbuffer/`

**Protocol định nghĩa:**

```cpp
// Protocol.h
enum ProtocolMessages {
    // Client → Server
    MSG_C2S_LOGIN = 100,
    MSG_C2S_SELECT_CHAR = 101,
    MSG_C2S_MOVE = 102,
    MSG_C2S_ATTACK = 103,
    MSG_C2S_USE_SKILL = 104,
    MSG_C2S_CHAT = 105,

    // Server → Client
    MSG_S2C_LOGIN_RESULT = 200,
    MSG_S2C_CHAR_LIST = 201,
    MSG_S2C_PLAYER_MOVE = 202,
    MSG_S2C_DAMAGE = 203,
    MSG_S2C_CHAT = 204,
};

// Packet structures
struct MSG_LOGIN {
    char username[32];
    char password[64];  // MD5 hashed
};

struct MSG_MOVE {
    int playerId;
    int x;
    int y;
};

struct MSG_USE_SKILL {
    int playerId;
    int skillId;
    int targetId;
};
```

**Gửi/nhận packet:**

```cpp
// Client side - Gửi login request
void SendLoginRequest(const char* username, const char* password) {
    MSG_LOGIN packet;
    strcpy(packet.username, username);
    // Hash password
    MD5Hash(password, packet.password);

    // Send packet
    g_Network->SendPacket(MSG_C2S_LOGIN, &packet, sizeof(packet));
}

// Server side - Nhận và xử lý login
void OnReceiveLoginRequest(int clientId, void* data, int size) {
    MSG_LOGIN* packet = (MSG_LOGIN*)data;

    // Verify credentials
    bool success = Database->VerifyUser(packet->username, packet->password);

    // Send response
    MSG_LOGIN_RESULT result;
    result.success = success;
    SendPacket(clientId, MSG_S2C_LOGIN_RESULT, &result, sizeof(result));
}
```

---

## 3. Chỉnh Sửa Game Client

### 3.1. Thêm UI Element Mới

**Ví dụ: Thêm button vào main menu**

**Bước 1: Sửa UI Lua script**

File: `Data/Script/UI/MainMenu.lua`

```lua
-- Thêm button mới
function OnMainMenuLoad()
    -- Tạo button
    local btnNewFeature = CreateButton("btnNewFeature")
    SetButtonPosition(btnNewFeature, 300, 400)
    SetButtonSize(btnNewFeature, 150, 40)
    SetButtonText(btnNewFeature, "Tính năng mới")
    SetButtonImage(btnNewFeature, "Ui/Buttons/button_normal.spr",
                                   "Ui/Buttons/button_hover.spr",
                                   "Ui/Buttons/button_pressed.spr")

    -- Register click handler
    RegisterButtonCallback(btnNewFeature, "OnNewFeatureClick")
end

function OnNewFeatureClick()
    print("New feature button clicked!")
    -- Call C++ function
    CallCppFunction("OpenNewFeatureWindow")
end
```

**Bước 2: Implement C++ handler**

File: `Sources/S3Client/Ui/UiMainMenu.cpp`

```cpp
#include "UiMainMenu.h"
#include "../KLuaScript.h"

// Register C++ function để Lua có thể gọi
void RegisterMainMenuFunctions() {
    g_LuaScript.RegisterFunction("OpenNewFeatureWindow", OpenNewFeatureWindow);
}

// Implement function
int OpenNewFeatureWindow(lua_State* L) {
    // Mở window tính năng mới
    g_NewFeatureWindow.Open();
    return 0;
}
```

### 3.2. Thêm Skill Mới

**Ví dụ: Thêm skill "Fire Blast"**

**Bước 1: Define skill trong database/config**

File: `Data/Skills/Skills.txt`

```
[Skill_500]
Name=Fire Blast
Description=Launches a fireball dealing massive damage
Type=Magic
Damage=500
ManaCost=50
Cooldown=5.0
Range=15
AreaOfEffect=5
Animation=fire_blast.ani
Effect=fire_explosion.spr
Sound=fire_blast.wav
```

**Bước 2: Implement skill logic**

File: `Sources/Kernel/KSkill.cpp`

```cpp
void KSkill::UseSkill_FireBlast(KPlayer* caster, KCharacter* target) {
    // Check mana
    if (caster->m_nMP < 50) {
        caster->ShowMessage("Không đủ mana!");
        return;
    }

    // Check cooldown
    if (m_fCooldownRemaining > 0) {
        caster->ShowMessage("Skill đang cooldown!");
        return;
    }

    // Consume mana
    caster->m_nMP -= 50;

    // Calculate damage
    int damage = 500 + (caster->m_nIntelligence * 2);

    // Apply damage to target
    if (target) {
        target->TakeDamage(damage, caster);

        // Area of effect damage
        KCharacter* nearbyEnemies[10];
        int count = FindNearbyEnemies(target->m_nX, target->m_nY, 5, nearbyEnemies, 10);
        for (int i = 0; i < count; i++) {
            if (nearbyEnemies[i] != target) {
                nearbyEnemies[i]->TakeDamage(damage / 2, caster);
            }
        }
    }

    // Play animation and sound
    PlayAnimation(caster, "fire_blast.ani");
    PlayEffect(target->m_nX, target->m_nY, "fire_explosion.spr");
    PlaySound("fire_blast.wav");

    // Set cooldown
    m_fCooldownRemaining = 5.0f;

    // Send to server
    SendUseSkillPacket(caster->m_nId, m_nSkillId, target->m_nId);
}
```

**Bước 3: Add vào skill bar UI**

File: `Data/Script/UI/SkillBar.lua`

```lua
function AddSkillToBar(skillId, slot)
    if skillId == 500 then  -- Fire Blast
        SetSkillIcon(slot, "Ui/Skills/fire_blast_icon.spr")
        SetSkillTooltip(slot, "Fire Blast\nCooldown: 5s\nMana: 50")
    end
end
```

### 3.3. Thay Đổi Graphics/Rendering

**Ví dụ: Thêm particle effect cho skill**

File: `Sources/Engine/KParticleSystem.cpp`

```cpp
class KParticleSystem {
public:
    void EmitFireBlast(int x, int y) {
        ParticleEmitter emitter;
        emitter.m_nX = x;
        emitter.m_nY = y;
        emitter.m_nParticleCount = 50;
        emitter.m_fLifetime = 2.0f;
        emitter.m_fSpeed = 100.0f;
        emitter.m_Color = RGB(255, 100, 0);  // Orange
        emitter.m_szTexture = "Effects/fire_particle.tga";

        // Emit particles
        for (int i = 0; i < emitter.m_nParticleCount; i++) {
            Particle p;
            p.m_fX = x + RandomFloat(-10, 10);
            p.m_fY = y + RandomFloat(-10, 10);
            p.m_fVelX = RandomFloat(-emitter.m_fSpeed, emitter.m_fSpeed);
            p.m_fVelY = RandomFloat(-emitter.m_fSpeed, emitter.m_fSpeed);
            p.m_fLifetime = emitter.m_fLifetime;
            p.m_Color = emitter.m_Color;
            m_Particles.push_back(p);
        }
    }

    void Update(float deltaTime) {
        for (auto& p : m_Particles) {
            p.m_fX += p.m_fVelX * deltaTime;
            p.m_fY += p.m_fVelY * deltaTime;
            p.m_fLifetime -= deltaTime;
            // Fade out
            p.m_fAlpha = p.m_fLifetime / 2.0f;
        }
        // Remove dead particles
        m_Particles.erase(
            std::remove_if(m_Particles.begin(), m_Particles.end(),
                [](const Particle& p) { return p.m_fLifetime <= 0; }),
            m_Particles.end()
        );
    }

    void Render() {
        for (const auto& p : m_Particles) {
            DrawParticle(p);
        }
    }
};
```

---

## 4. Chỉnh Sửa Game Server

### 4.1. Thêm NPC Mới

**Ví dụ: Thêm merchant NPC**

**Bước 1: Define NPC trong config**

File: `Data/NPC/Merchants.txt`

```
[NPC_1000]
Name=Weapon Merchant
Type=Shop
Map=Village01
PosX=100
PosY=150
Model=npc_merchant.mdl
Dialog=merchant_dialog.txt
ShopInventory=shop_weapons.txt
```

**Bước 2: Implement NPC logic trên server**

File: `Sources/S3Server/NPC/KNpcShop.cpp`

```cpp
class KNpcShop : public KNpc {
public:
    std::vector<KItem*> m_ShopInventory;

    void LoadShopInventory(const char* filename) {
        // Load items from file
        KFile file;
        if (file.Open(filename, FILE_READ)) {
            // Parse items...
        }
    }

    void OnPlayerInteract(KPlayer* player) {
        // Send shop inventory to player
        SendShopInventoryPacket(player, m_ShopInventory);
    }

    void OnPlayerBuyItem(KPlayer* player, int itemId, int count) {
        // Find item in inventory
        KItem* item = FindItemById(itemId);
        if (!item) return;

        // Check price
        int totalPrice = item->m_nPrice * count;
        if (player->m_nGold < totalPrice) {
            SendMessage(player, "Không đủ tiền!");
            return;
        }

        // Check inventory space
        if (!player->HasInventorySpace(count)) {
            SendMessage(player, "Hành trang đầy!");
            return;
        }

        // Transaction
        player->m_nGold -= totalPrice;
        player->AddItem(item, count);

        // Log transaction
        LogTransaction(player->m_nId, itemId, count, totalPrice);

        // Send update
        SendPlayerGoldUpdate(player);
        SendPlayerInventoryUpdate(player);
    }
};
```

**Bước 3: Spawn NPC khi load map**

File: `Sources/S3Server/GameWorld/KMap.cpp`

```cpp
void KMap::LoadNPCs() {
    // Load NPCs từ config
    KConfigFile config("Data/NPC/Merchants.txt");

    for (auto& npcData : config.GetNPCs()) {
        if (npcData.type == "Shop") {
            KNpcShop* npc = new KNpcShop();
            npc->m_nId = npcData.id;
            npc->m_szName = npcData.name;
            npc->m_nX = npcData.posX;
            npc->m_nY = npcData.posY;
            npc->LoadShopInventory(npcData.shopInventory);

            m_NPCs.push_back(npc);
        }
    }
}
```

### 4.2. Thêm Event/Quest

**Ví dụ: Thêm quest "Kill 10 Goblins"**

**Bước 1: Define quest**

File: `Data/Quests/quests.txt`

```
[Quest_100]
Name=Goblin Extermination
Description=Kill 10 goblins terrorizing the village
Giver=NPC_1001
Receiver=NPC_1001
Objectives=Kill:Goblin:10
Rewards=Gold:1000,Exp:500,Item:Potion:5
Level=5
```

**Bước 2: Implement quest system**

File: `Sources/S3Server/Quest/KQuest.cpp`

```cpp
class KQuest {
public:
    int m_nQuestId;
    std::string m_szName;
    std::string m_szDescription;
    std::vector<QuestObjective> m_Objectives;
    std::vector<QuestReward> m_Rewards;

    void OnObjectiveComplete(KPlayer* player, QuestObjective* obj) {
        obj->m_nProgress++;

        // Send progress update
        SendQuestProgressUpdate(player, this);

        // Check if all objectives complete
        if (IsComplete()) {
            OnQuestComplete(player);
        }
    }

    void OnQuestComplete(KPlayer* player) {
        // Give rewards
        for (auto& reward : m_Rewards) {
            switch (reward.m_nType) {
                case REWARD_GOLD:
                    player->m_nGold += reward.m_nValue;
                    break;
                case REWARD_EXP:
                    player->AddExp(reward.m_nValue);
                    break;
                case REWARD_ITEM:
                    player->AddItem(reward.m_nItemId, reward.m_nCount);
                    break;
            }
        }

        // Mark quest as completed
        player->m_CompletedQuests.push_back(m_nQuestId);

        // Send completion message
        SendQuestCompletePacket(player, this);
        SendMessage(player, "Quest completed!");
    }
};

// Hook vào combat system
void KNpc::OnDeath(KCharacter* killer) {
    if (killer->IsPlayer()) {
        KPlayer* player = (KPlayer*)killer;

        // Check active quests
        for (auto& quest : player->m_ActiveQuests) {
            for (auto& obj : quest->m_Objectives) {
                if (obj.m_nType == OBJ_KILL &&
                    obj.m_nTargetNpcId == m_nNpcId) {
                    quest->OnObjectiveComplete(player, &obj);
                }
            }
        }
    }
}
```

### 4.3. Tối Ưu Server Performance

**Ví dụ: Implement spatial partitioning cho collision detection**

File: `Sources/S3Server/GameWorld/KSpatialGrid.cpp`

```cpp
// Grid-based spatial partitioning
class KSpatialGrid {
private:
    static const int GRID_SIZE = 50;  // Mỗi cell 50x50 units
    std::unordered_map<int, std::vector<KCharacter*>> m_Grid;

    int GetGridKey(int x, int y) {
        int gridX = x / GRID_SIZE;
        int gridY = y / GRID_SIZE;
        return (gridY << 16) | gridX;
    }

public:
    void AddCharacter(KCharacter* character) {
        int key = GetGridKey(character->m_nX, character->m_nY);
        m_Grid[key].push_back(character);
    }

    void RemoveCharacter(KCharacter* character) {
        int key = GetGridKey(character->m_nX, character->m_nY);
        auto& cell = m_Grid[key];
        cell.erase(std::remove(cell.begin(), cell.end(), character), cell.end());
    }

    void UpdateCharacterPosition(KCharacter* character, int oldX, int oldY) {
        int oldKey = GetGridKey(oldX, oldY);
        int newKey = GetGridKey(character->m_nX, character->m_nY);

        if (oldKey != newKey) {
            RemoveCharacter(character);
            AddCharacter(character);
        }
    }

    std::vector<KCharacter*> FindNearbyCharacters(int x, int y, int range) {
        std::vector<KCharacter*> result;

        // Check cells trong range
        int gridRange = (range / GRID_SIZE) + 1;
        int centerGridX = x / GRID_SIZE;
        int centerGridY = y / GRID_SIZE;

        for (int dy = -gridRange; dy <= gridRange; dy++) {
            for (int dx = -gridRange; dx <= gridRange; dx++) {
                int key = ((centerGridY + dy) << 16) | (centerGridX + dx);
                auto it = m_Grid.find(key);
                if (it != m_Grid.end()) {
                    for (auto* character : it->second) {
                        int dist = CalculateDistance(x, y,
                                                     character->m_nX,
                                                     character->m_nY);
                        if (dist <= range) {
                            result.push_back(character);
                        }
                    }
                }
            }
        }

        return result;
    }
};

// Sử dụng trong game world
void KGameWorld::Update(float deltaTime) {
    // Chỉ update characters trong active cells
    for (auto& player : m_Players) {
        auto nearbyCharacters = m_SpatialGrid.FindNearbyCharacters(
            player->m_nX, player->m_nY, 500);  // 500 units range

        // Chỉ gửi update cho nearby characters
        SendNearbyUpdates(player, nearbyCharacters);
    }
}
```

---

## 5. Thêm Tính Năng Mới

### 5.1. Ví Dụ: Thêm Pet System

**Bước 1: Define pet class**

File: `Sources/Kernel/KPet.h`

```cpp
class KPet : public KCharacter {
public:
    int m_nPetId;
    int m_nOwnerPlayerId;
    int m_nLevel;
    int m_nExp;
    int m_nLoyalty;        // 0-100
    PetType m_nType;       // Combat, Gathering, Mount
    PetAI m_AI;

    // Basic functions
    void FollowOwner();
    void Attack(KCharacter* target);
    void UseSkill(int skillId);
    void Feed(KItem* food);
    void LevelUp();

    // AI
    void UpdateAI(float deltaTime);
    void OnOwnerAttacked(KCharacter* attacker);
};
```

**Bước 2: Implement pet logic**

File: `Sources/Kernel/KPet.cpp`

```cpp
void KPet::UpdateAI(float deltaTime) {
    KPlayer* owner = FindPlayerById(m_nOwnerPlayerId);
    if (!owner) return;

    switch (m_AI.m_nState) {
        case PET_STATE_FOLLOW:
            FollowOwner();
            break;

        case PET_STATE_COMBAT:
            if (m_pTarget && m_pTarget->IsAlive()) {
                AttackTarget();
            } else {
                m_AI.m_nState = PET_STATE_FOLLOW;
            }
            break;

        case PET_STATE_IDLE:
            // Check if owner in combat
            if (owner->IsInCombat()) {
                m_AI.m_nState = PET_STATE_COMBAT;
                m_pTarget = owner->m_pTarget;
            }
            break;
    }
}

void KPet::FollowOwner() {
    KPlayer* owner = FindPlayerById(m_nOwnerPlayerId);
    if (!owner) return;

    int dist = CalculateDistance(m_nX, m_nY, owner->m_nX, owner->m_nY);

    if (dist > 100) {  // Nếu xa quá, teleport lại
        m_nX = owner->m_nX - 50;
        m_nY = owner->m_nY - 50;
    } else if (dist > 30) {  // Follow
        MoveTowards(owner->m_nX, owner->m_nY);
    }
}

void KPet::Feed(KItem* food) {
    if (food->m_nType != ITEM_TYPE_PET_FOOD) return;

    // Increase loyalty
    m_nLoyalty += 10;
    if (m_nLoyalty > 100) m_nLoyalty = 100;

    // Restore HP/MP
    m_nHP = m_nMaxHP;
    m_nMP = m_nMaxMP;

    // Consume food
    food->m_nCount--;
}
```

**Bước 3: Add UI for pet management**

File: `Data/Script/UI/PetWindow.lua`

```lua
function OnPetWindowLoad()
    -- Create pet window
    local window = CreateWindow("PetWindow", 400, 300, 400, 500)
    SetWindowTitle(window, "Pet Management")

    -- Pet info
    local lblPetName = CreateLabel(window, 20, 50, 200, 30, "Pet Name")
    local lblLevel = CreateLabel(window, 20, 90, 200, 30, "Level: 1")
    local lblLoyalty = CreateLabel(window, 20, 130, 200, 30, "Loyalty: 50")

    -- Progress bars
    local barHP = CreateProgressBar(window, 20, 170, 350, 20)
    local barExp = CreateProgressBar(window, 20, 200, 350, 20)

    -- Buttons
    local btnFeed = CreateButton(window, 20, 250, 100, 40, "Feed")
    RegisterButtonCallback(btnFeed, "OnFeedPetClick")

    local btnSkills = CreateButton(window, 140, 250, 100, 40, "Skills")
    RegisterButtonCallback(btnSkills, "OnPetSkillsClick")

    local btnRelease = CreateButton(window, 260, 250, 100, 40, "Release")
    RegisterButtonCallback(btnRelease, "OnReleasePetClick")
end

function OnFeedPetClick()
    -- Show food selection
    OpenPetFoodSelection()
end

function UpdatePetInfo(petData)
    SetLabelText("lblPetName", petData.name)
    SetLabelText("lblLevel", "Level: " .. petData.level)
    SetLabelText("lblLoyalty", "Loyalty: " .. petData.loyalty)
    SetProgressBarValue("barHP", petData.hp / petData.maxHP)
    SetProgressBarValue("barExp", petData.exp / petData.maxExp)
end
```

**Bước 4: Server-side pet management**

File: `Sources/S3Server/Pet/KPetManager.cpp`

```cpp
class KPetManager {
public:
    void OnPlayerSummonPet(KPlayer* player, int petId) {
        // Check if already has pet summoned
        if (player->m_pPet) {
            SendMessage(player, "Already has pet summoned!");
            return;
        }

        // Load pet data
        PetData data = LoadPetData(player->m_nId, petId);

        // Spawn pet
        KPet* pet = new KPet();
        pet->m_nPetId = petId;
        pet->m_nOwnerPlayerId = player->m_nId;
        pet->m_nLevel = data.level;
        pet->m_nExp = data.exp;
        pet->m_nLoyalty = data.loyalty;
        pet->m_nX = player->m_nX - 50;
        pet->m_nY = player->m_nY - 50;

        player->m_pPet = pet;
        g_GameWorld->AddCharacter(pet);

        // Send to client
        SendSpawnPetPacket(player, pet);
    }

    void OnPlayerDismissPet(KPlayer* player) {
        if (!player->m_pPet) return;

        // Save pet data
        SavePetData(player->m_nId, player->m_pPet);

        // Remove from world
        g_GameWorld->RemoveCharacter(player->m_pPet);
        delete player->m_pPet;
        player->m_pPet = nullptr;

        // Send to client
        SendDismissPetPacket(player);
    }
};
```

### 5.2. Ví Dụ: Thêm Guild System

**Tương tự pet system, bạn cần:**

1. Define guild structures và database schema
2. Implement guild management (create, join, leave, promote, etc.)
3. Add guild UI
4. Implement guild features (guild storage, guild skills, guild wars, etc.)

_(Chi tiết implementation tương tự như pet system)_

---

## 6. Debug và Testing

### 6.1. Setup Debug Environment

**Visual Studio Debugger:**

1. Set configuration thành `Debug`
2. Set breakpoints trong code
3. F5 để start debugging

**Debug macros:**

```cpp
// Trong StdAfx.h hoặc config.h
#ifdef _DEBUG
    #define DEBUG_LOG(msg) printf("[DEBUG] %s\n", msg)
    #define DEBUG_ASSERT(condition, msg) \
        if (!(condition)) { \
            printf("[ASSERT FAILED] %s\n", msg); \
            __debugbreak(); \
        }
#else
    #define DEBUG_LOG(msg)
    #define DEBUG_ASSERT(condition, msg)
#endif

// Sử dụng
DEBUG_LOG("Player position updated");
DEBUG_ASSERT(player != nullptr, "Player is null!");
```

### 6.2. Logging System

**Implement logging:**

File: `Sources/Core/KLog.cpp`

```cpp
class KLog {
private:
    FILE* m_pLogFile;
    LogLevel m_nLevel;

public:
    enum LogLevel {
        LOG_DEBUG,
        LOG_INFO,
        LOG_WARN,
        LOG_ERROR,
        LOG_FATAL
    };

    void Init(const char* filename) {
        m_pLogFile = fopen(filename, "a");
        m_nLevel = LOG_INFO;
    }

    void Log(LogLevel level, const char* format, ...) {
        if (level < m_nLevel) return;

        // Get timestamp
        time_t now = time(nullptr);
        char timeStr[64];
        strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", localtime(&now));

        // Get level string
        const char* levelStr = GetLevelString(level);

        // Format message
        va_list args;
        va_start(args, format);
        char message[1024];
        vsnprintf(message, sizeof(message), format, args);
        va_end(args);

        // Write to file
        fprintf(m_pLogFile, "[%s] [%s] %s\n", timeStr, levelStr, message);
        fflush(m_pLogFile);

        // Also print to console in debug mode
        #ifdef _DEBUG
        printf("[%s] [%s] %s\n", timeStr, levelStr, message);
        #endif
    }
};

// Global instance
extern KLog g_Log;

// Macros for easy use
#define LOG_DEBUG(fmt, ...) g_Log.Log(KLog::LOG_DEBUG, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)  g_Log.Log(KLog::LOG_INFO, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...)  g_Log.Log(KLog::LOG_WARN, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) g_Log.Log(KLog::LOG_ERROR, fmt, ##__VA_ARGS__)

// Usage
LOG_INFO("Player %s logged in from IP %s", player->m_szName, player->m_szIP);
LOG_ERROR("Failed to load map: %s", mapName);
```

### 6.3. Unit Testing

**Ví dụ: Test skill damage calculation**

File: `Tests/TestSkills.cpp`

```cpp
#include "gtest/gtest.h"
#include "../Kernel/KSkill.h"
#include "../Kernel/KPlayer.h"

TEST(SkillTest, FireBlastDamageCalculation) {
    // Setup
    KPlayer caster;
    caster.m_nIntelligence = 100;

    KSkill skill;
    skill.m_nSkillId = 500;  // Fire Blast
    skill.m_nBaseDamage = 500;

    // Execute
    int damage = skill.CalculateDamage(&caster);

    // Assert
    EXPECT_EQ(damage, 500 + (100 * 2));  // 500 + (int * 2) = 700
}

TEST(SkillTest, ManaCostCheck) {
    KPlayer player;
    player.m_nMP = 30;

    KSkill skill;
    skill.m_nManaCost = 50;

    EXPECT_FALSE(skill.CanUse(&player));

    player.m_nMP = 60;
    EXPECT_TRUE(skill.CanUse(&player));
}
```

### 6.4. Performance Profiling

**Measure function execution time:**

```cpp
class KProfiler {
public:
    void StartTimer(const char* name) {
        m_Timers[name] = std::chrono::high_resolution_clock::now();
    }

    void StopTimer(const char* name) {
        auto end = std::chrono::high_resolution_clock::now();
        auto start = m_Timers[name];
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        LOG_INFO("Profiler[%s]: %lld microseconds", name, duration.count());
    }

private:
    std::unordered_map<std::string, std::chrono::time_point<std::chrono::high_resolution_clock>> m_Timers;
};

// Usage
g_Profiler.StartTimer("UpdatePlayers");
UpdateAllPlayers(deltaTime);
g_Profiler.StopTimer("UpdatePlayers");
```

---

## 7. Best Practices

### 7.1. Code Style

**Naming conventions:**

```cpp
// Classes: PascalCase với prefix K
class KPlayer { };
class KNpc { };

// Member variables: m_ prefix
int m_nHealth;
std::string m_szName;
bool m_bIsAlive;

// Constants: ALL_CAPS
const int MAX_PLAYERS = 1000;
const float PI = 3.14159f;

// Functions: PascalCase
void UpdatePosition();
int CalculateDamage();

// Local variables: camelCase
int playerCount = 0;
float deltaTime = 0.016f;
```

**Comments:**

```cpp
// Good: Explain WHY, not WHAT
// Check if player has enough mana before casting spell
// This prevents negative mana bug (#1234)
if (player->m_nMP >= skill->m_nManaCost) {
    CastSpell(skill);
}

// Bad: Just repeating code
// Check if mana is greater than or equal to mana cost
if (player->m_nMP >= skill->m_nManaCost) {
    CastSpell(skill);
}
```

### 7.2. Memory Management

**RAII pattern:**

```cpp
class KResourceHolder {
public:
    KResourceHolder() {
        m_pResource = AllocateResource();
    }

    ~KResourceHolder() {
        if (m_pResource) {
            FreeResource(m_pResource);
            m_pResource = nullptr;
        }
    }

    // Disable copy
    KResourceHolder(const KResourceHolder&) = delete;
    KResourceHolder& operator=(const KResourceHolder&) = delete;

private:
    Resource* m_pResource;
};
```

**Smart pointers (nếu dùng C++11+):**

```cpp
#include <memory>

// Thay vì:
KPlayer* player = new KPlayer();
// ... use player ...
delete player;

// Dùng:
std::unique_ptr<KPlayer> player = std::make_unique<KPlayer>();
// Tự động delete khi out of scope
```

### 7.3. Error Handling

**Check return values:**

```cpp
// Bad
KFile file;
file.Open("config.ini", FILE_READ);
file.Read(buffer, size);

// Good
KFile file;
if (!file.Open("config.ini", FILE_READ)) {
    LOG_ERROR("Failed to open config.ini");
    return false;
}

int bytesRead = file.Read(buffer, size);
if (bytesRead != size) {
    LOG_ERROR("Failed to read complete config, expected %d bytes, got %d", size, bytesRead);
    file.Close();
    return false;
}
```

**Exception safety (nếu dùng exceptions):**

```cpp
void ProcessPlayer(KPlayer* player) {
    try {
        player->Update();
        player->Render();
    } catch (const std::exception& e) {
        LOG_ERROR("Exception processing player %d: %s", player->m_nId, e.what());
        // Cleanup if needed
        player->SetState(STATE_IDLE);
    }
}
```

### 7.4. Performance Tips

**Avoid unnecessary allocations:**

```cpp
// Bad: Creates temporary string every frame
void Update() {
    std::string message = "Player position: " + std::to_string(x) + "," + std::to_string(y);
    LOG_DEBUG(message.c_str());
}

// Good: Only when needed
void OnPlayerMove() {
    char message[128];
    snprintf(message, sizeof(message), "Player position: %d,%d", x, y);
    LOG_DEBUG(message);
}
```

**Cache frequently accessed data:**

```cpp
// Bad: Tính lại mỗi lần
int GetTotalDefense() {
    return m_nBaseDefense + GetArmorDefense() + GetBuffDefense();
}

// Good: Cache và invalidate khi thay đổi
void UpdateEquipment() {
    m_nTotalDefense = m_nBaseDefense + GetArmorDefense() + GetBuffDefense();
    m_bDefenseCacheDirty = false;
}

int GetTotalDefense() {
    if (m_bDefenseCacheDirty) {
        UpdateEquipment();
    }
    return m_nTotalDefense;
}
```

**Use object pooling:**

```cpp
class KBulletPool {
private:
    std::vector<KBullet*> m_Pool;
    std::vector<KBullet*> m_Active;

public:
    KBullet* Spawn() {
        KBullet* bullet;
        if (m_Pool.empty()) {
            bullet = new KBullet();
        } else {
            bullet = m_Pool.back();
            m_Pool.pop_back();
        }
        m_Active.push_back(bullet);
        return bullet;
    }

    void Despawn(KBullet* bullet) {
        auto it = std::find(m_Active.begin(), m_Active.end(), bullet);
        if (it != m_Active.end()) {
            m_Active.erase(it);
            m_Pool.push_back(bullet);
        }
    }
};
```

### 7.5. Version Control

**Git workflow:**

```bash
# Create feature branch
git checkout -b feature/add-pet-system

# Make changes and commit frequently
git add Sources/Kernel/KPet.cpp
git commit -m "Add KPet class implementation"

git add Data/Script/UI/PetWindow.lua
git commit -m "Add pet management UI"

# Push to remote
git push origin feature/add-pet-system

# Create pull request for review

# After review, merge to main
git checkout main
git merge feature/add-pet-system
```

**Commit messages:**

```
Good:
- "Fix crash when player disconnects during trade"
- "Add pet loyalty system"
- "Optimize pathfinding algorithm"

Bad:
- "fix bug"
- "update"
- "changes"
```

---

## Tổng Kết

### Workflow Phát Triển Tính Năng Mới

1. **Design**: Viết design doc, define structures
2. **Prototype**: Implement basic version
3. **Test**: Unit test, integration test
4. **Optimize**: Profile và optimize
5. **Document**: Cập nhật tài liệu
6. **Review**: Code review
7. **Deploy**: Build và release

### Checklist Trước Khi Commit

- ✅ Code compile không lỗi
- ✅ Không có warning quan trọng
- ✅ Test cases pass
- ✅ Code style consistent
- ✅ Comments đầy đủ
- ✅ No memory leaks (check với tools)
- ✅ Performance acceptable

### Resources

- C++ Reference: https://en.cppreference.com/
- Lua Documentation: https://www.lua.org/manual/5.1/
- Game Programming Patterns: http://gameprogrammingpatterns.com/

---

**Happy coding!** 🎮
