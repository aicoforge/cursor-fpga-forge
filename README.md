[English](./README_EN.md) | **繁體中文**

# Cursor-FPGA-Forge

## AI 代理驅動的 FPGA 自動驗證平台

> 將數週的環境設置與驗證工作縮短至數分鐘

---

## 專案簡介

**Cursor-FPGA-Forge** 是 AICOFORGE 平台的 FPGA 自動驗證範例，展示如何透過 AI 代理（Cursor IDE）自動產生完整的 FPGA 驗證環境，包含設計合成、bitstream 產生、主機端測試程式，並完成硬體驗證。

### 核心價值

| 指標 | 成果 |
|:---:|:---:|
| **數分鐘** | 自動產生完整驗證環境（傳統需數週） |
| **2 小時** | 完整 FPGA 流程示範影片 |
| **零錯誤** | AI 自動處理設定檔與記憶體配置 |
| **隨插即用** | 適用不同 FPGA 平台與工具版本 |

---

## 快速入門

本專案提供一個**預先設計的 FPGA 驗證樣板**，讓你可以透過 AI 代理快速驗證自己的 FPGA 電路設計。這裡以簡單的 `adder` 加法器為範例，展示完整的自動化流程。你可以根據實際需求調整工具版本、FPGA 平台等設定。

### 展示影片與操作流程

#### Demo：完整 AI 自動化 FPGA 驗證流程

**影片連結**：[Watch cursor + FPGA verification demo](https://www.youtube.com/watch?v=u_RUluOvOeM)（2 小時完整過程）

**執行日誌**：[View full synthesis & verification log](https://www.dropbox.com/scl/fi/2zua9qlv7qj0nrdeg7fwz/demo.log?rlkey=wbat8307916fzze0437lvcvtt&e=1&dl=0)

#### 影片內容說明

此影片展示 Cursor AI 如何自動完成整個 FPGA 驗證流程：

1. **FPGA 設置檔案產生** — AI 自動產生環境設定、Makefile、編譯設定檔
2. **FPGA 設計合成** — HLS 合成與優化（約 1-2 分鐘）
3. **Bitstream 產生** — 系統整合、佈局佈線（約 2 小時）
4. **主機端測試程式** — 自動產生 XRT API 整合的測試程式
5. **自動測試執行** — 在實體 U50 FPGA 上完成硬體驗證

**觀看建議**：影片長達 2 小時，你可以選擇有興趣的片段觀看，了解 Cursor 如何回應需求並自動化執行各項工作。

#### 執行日誌說明

日誌檔案包含完整的執行細節：

- **C Simulation (csim)** — 功能驗證結果
- **C Synthesis (csynth)** — 合成報告、時序分析、資源使用
- **U50 Hardware Verification** — 實體 FPGA 測試結果與效能數據

---

### 環境需求

```bash
# 你需要：
- Cursor IDE (https://cursor.com) 與 SSH 擴充功能
- SSH 存取 FPGA 開發伺服器
- Xilinx Vitis 2022.1+（或調整為你的版本）
- Xilinx U50 FPGA（或適配你的開發板）
- 具備 FPGA 存取權限的 Linux 環境
```

---

### 操作步驟

#### 1. 連接 FPGA 開發環境

使用 Cursor 的 SSH 功能連接到你的 FPGA 開發伺服器：

```bash
# 在 Cursor IDE 中：
1. 按 Ctrl+Shift+P
2. 輸入 "Remote-SSH: Connect to Host"
3. 設定你的 SSH 連線
4. 連接到 FPGA 伺服器
```

詳細的 SSH 設定說明請參考 [Cursor 文件](https://docs.cursor.com/en/welcome)。

---

#### 2. 克隆專案並測試基礎 HLS 流程

```bash
# 克隆專案
git clone https://github.com/aicoforge/cursor-fpga-forge.git
cd cursor-fpga-forge

# 專案結構：
cursor-fpga-forge/
├── adder_kernel.cpp          # 基礎 HLS kernel
├── adder_kernel.h            # Kernel 標頭檔
├── fpga_verification.md      # 自動化規格文件
├── run_hls.tcl               # HLS 測試設定
├── tb_add.cpp                # 測試平台
├── vitis_env.sh              # 環境設定腳本
└── .vscode/                  # Cursor/VSCode 設定
    └── tasks.json            # 建置任務設定

# 啟用執行權限並測試基礎 HLS 流程
chmod +x vitis_env.sh

# 在 Cursor IDE 中按 Ctrl+Shift+B 執行 HLS 驗證
# 這將執行：./vitis_env.sh vitis_hls -f run_hls.tcl
```

---

#### 3. 使用 AI 自動產生 FPGA 驗證腳本

**關鍵步驟**：讓 Cursor AI 根據規格文件自動產生驗證環境

```bash
# 在 Cursor IDE 中：
1. 開啟此專案目錄
2. 給 Cursor AI 這個提示：

"please follow fpga_verification.md to build fpga verification needed scripts"

3. Cursor 將自動產生完整的驗證框架：
   - setup_verification.sh（環境設定）
   - run_verification.sh（驗證執行）
   - fpga_verification/ 目錄與所有原始檔案及 Makefile
```

---

#### 4. 執行設定與驗證

```bash
# 賦予執行權限
chmod +x setup_verification.sh run_verification.sh

# 執行環境設定
./setup_verification.sh

# 產生的目錄結構：
fpga_verification/
├── src/
│   ├── adder_kernel.cpp       # 包含 HLS pragmas 的 kernel
│   ├── adder_kernel.h         # Kernel 標頭檔
│   └── host.cpp               # 主機端程式（多層記憶體配置）
├── kernels/Hardware/
│   ├── Makefile               # Kernel 編譯 Makefile
│   └── adder-compile.cfg      # Kernel 編譯設定
├── system_hw_link/Hardware/
│   └── Makefile               # 系統連結 Makefile
├── host/build/
│   └── Makefile               # 主機端編譯 Makefile
└── vitis_env.sh               # 環境設定腳本

# 執行完整 FPGA 驗證（合成 + 實作 + 測試）
./run_verification.sh all

# 或僅執行軟體模擬（快速驗證程式碼修改）
./run_verification.sh sw
```

---

### 驗證結果

真實 FPGA 流程與時間數據：

1. **Kernel 編譯**（約 1-2 分鐘）
   - HLS 合成與 pragma 優化
   - 預估 Fmax：411MHz（簡單加法器）

2. **Bitstream 產生**（約 2 小時）
   - 系統整合與平台連結
   - 區塊級合成：111 個平行工作
   - 邏輯佈局與佈線優化
   - 最終 bitstream 大小：約 30MB

3. **主機端編譯**（約 30 秒）
   - XRT API 整合與錯誤處理
   - 多層記憶體配置降級機制

4. **硬體驗證**（約 5 秒）
   - 在實體 U50 FPGA 上執行
   - **記憶體配置**：自動降級至 HBM Bank 0
   - **測試案例**：4 種不同場景 ✓ 全部通過

---

## 環境客製化

你可以根據實際需求調整環境設定：

### 不同的 Vitis 版本？

```bash
# 方法 1：產生前修改 fpga_verification.md
# 找到 Section 1 並修改：
VITIS_VERSION="2023.2"  # 從 2022.1 改為 2023.2
VITIS_PATH="/opt/Xilinx/Vitis/${VITIS_VERSION}"

# 方法 2：產生後直接編輯腳本
# 在 setup_verification.sh 和 vitis_env.sh 中修改
VITIS_VERSION="2023.2"
VITIS_PATH="/opt/Xilinx/Vitis/2023.2"
```

### 不同的 FPGA 平台？

```bash
# 方法 1：修改 fpga_verification.md 規格
# 找到 Section 1 並修改：
DEFAULT_PLATFORM="xilinx_u250_gen3x16_xdma_4_1_202210_1"

# 方法 2：執行前設定環境變數
export FPGA_PLATFORM="xilinx_u250_gen3x16_xdma_4_1_202210_1"
./setup_verification.sh
```

### 不同的安裝路徑？

```bash
# 修改 fpga_verification.md Section 1 中的路徑
XRT_PATH="/opt/xilinx/xrt"                    # 改為你的 XRT 路徑
VITIS_PATH="/opt/Xilinx/Vitis/2022.1"         # 改為你的 Vitis 路徑

# 常見替代路徑：
XRT_PATH="/tools/Xilinx/xrt"                  # 替代 XRT 位置
VITIS_PATH="/tools/Xilinx/Vitis/2023.1"       # 不同版本/位置
```

---

## AI 加速什麼 vs 什麼需要時間

### AI 自動化的部分（數分鐘）：
- **環境設定**：完整驗證框架產生
- **設定檔配置**：建置檔案、HLS 設定、記憶體配置 — 無需試錯
- **錯誤恢復**：智能降級機制，無需手動除錯
- **平台適配**：快速切換 FPGA 平台與工具版本

### 仍需時間的部分（物理限制）：
- **Kernel 編譯**：約 1-2 分鐘（HLS 合成）
- **Bitstream 產生**：約 2 小時（佈局佈線無法加速）
- **硬體測試**：約 5 秒（實體 FPGA 執行）

**影響**：從原本需要**數週**的設定、除錯、修正 pragma 錯誤，縮短至**數分鐘**產生可運作的框架，加上**數小時**的實際 FPGA 編譯。繁瑣易錯的人工作業完全消失。

---

## 體驗預建的 AI 代理 + FPGA 開發環境

如果你對這個透過 AI 代理自動化 FPGA 驗證的範例有興趣，但沒有能力複製類似環境，歡迎聯繫我們：

**聯絡方式**：kevinjan@aicoforge.com

我們可以提供一個**預先建置好的 AI 代理 + FPGA 開發環境**，讓你實際體會 AI 代理如何自動化 FPGA 驗證的完整操作流程。

---

## 相關連結

- **AICOFORGE 官網**：[aicoforge.com](https://aicoforge.com)
- **HLS 優化專案**：[cursor-hls-forge](https://github.com/aicoforge/cursor-hls-forge)
- **Demo 影片播放清單**：[YouTube Channel](https://www.youtube.com/@aicoforge)

---

## 授權條款

MIT License

---

## 關於 AICOFORGE

AICOFORGE 是一個 AI 代理 + FPGA 自動設計驗證平台，致力於讓硬體開發像軟體一樣敏捷。我們與台灣大學電機系賴瑾教授（前威盛電子 CTO 暨創辦人）共同研發 LLM2HLS 理論技術，結合學術創新與產業實戰經驗。

**商業合作**：kevinjan@aicoforge.com
