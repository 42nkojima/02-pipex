# Pipex Bonus実装の残り作業量見積もり

## 🚨 重要なルール：_bonus サフィックス必須

PDFのCommon Instructions（3-4ページ）より：
> **"Bonuses must be placed in _bonus.{c/h} files, unless the subject specifies otherwise."**

**全てのBonus実装ファイルは `_bonus.{c/h}` というサフィックスを付けなければならない。**

- MandatoryとBonusの評価は**完全に別々**
- `make` → Mandatory用ソースをコンパイル
- `make bonus` → Bonus用ソース（`*_bonus.c`）をコンパイル
- 同じ実行ファイル名 `./pipex` を生成（上書き）
- `main.c` と `main_bonus.c` が両方存在してもOK（別々にコンパイルされるため）

---

## 現在の状況

### 完了している部分（Mandatory part）
- ✅ 基本的なパイプ実装（2コマンド固定）
- ✅ ファイル入出力のリダイレクト
- ✅ コマンドのPATH解決・引数パース
- ✅ エラーハンドリング（exit code 0/1/126/127対応）
- ✅ プロセス管理（fork/execve/waitpid）
- ✅ ファイルディスクリプタ管理

**評価:** Mandatory partは95-100%完成（本番レベル）
**重要:** Mandatoryファイル（`src/*.c`, `includes/pipex.h`）は一切変更しない

---

## Bonus part要件

### 1. 複数パイプ対応（Multiple pipes）
```bash
./pipex file1 cmd1 cmd2 cmd3 ... cmdn file2
# 動作: < file1 cmd1 | cmd2 | cmd3 ... | cmdn > file2
```

### 2. Here_doc対応
```bash
./pipex here_doc LIMITER cmd cmd1 file
# 動作: cmd << LIMITER | cmd1 >> file
```

---

## 残りの実装量見積もり

### 📊 実装量サマリー

| 項目 | 新規コード行数 | 所要時間（目安） |
|------|---------------|------------------|
| 複数パイプ対応 | 150-200行 | 4-6時間 |
| Here_doc対応 | 80-120行 | 2-3時間 |
| テスト・デバッグ | - | 2-3時間 |
| **合計** | **230-320行** | **8-12時間** |

---

## 詳細な実装計画

### 1️⃣ 複数パイプ対応（4-6時間）

#### 必要な変更箇所

**A. 新規ファイル: src/main_bonus.c（80-100行）**
- Mandatory の `main.c` は一切変更しない
- 新しく `main_bonus.c` を作成
- 可変長引数に対応（argc - 3 = コマンド数）
- 複数パイプの動的配列を使用

```c
// src/main_bonus.c（新規作成）
int main(int argc, char **argv, char **envp)
{
    int **pipes;      // 複数パイプ: pipes[0], pipes[1], ...
    pid_t *pids;      // 複数プロセス: pids[0], pids[1], ...
    int cmd_count;    // コマンド数を計算

    if (argc < 5)
        error_exit("Usage: ./pipex file1 cmd1 cmd2 ... cmdn file2");

    cmd_count = argc - 3;  // file1とfile2を除く
    // 複数パイプ処理...
}
```

**B. 新規ファイル: src/multi_pipe_bonus.c（70-80行）**
以下の関数が必要：
- `create_multiple_pipes()` - 複数パイプの作成・初期化
- `exec_first_child()` - 最初のコマンド実行
- `exec_middle_child()` - 中間コマンド実行（ループで使用）
- `exec_last_child()` - 最後のコマンド実行
- `close_all_pipes()` - 全パイプのクローズ
- `wait_all_children()` - 全プロセスの待機

**C. 新規ファイル: src/process_bonus.c（60-80行）**
- Mandatoryの`process.c`は変更しない
- Bonus用のプロセス実行関数を新規作成
- 複数パイプに対応したfd管理

#### 実装の複雑さ

**中程度** - 以下の点に注意が必要：
1. パイプの数 = コマンド数 - 1
2. 各プロセスで不要なfdを全てcloseする必要がある
3. メモリ管理（動的配列の確保・解放）

```
コマンド数3の例: cmd1 | cmd2 | cmd3
- パイプ2つ必要: pipe[0][0,1], pipe[1][0,1]
- プロセス3つ: pid[0], pid[1], pid[2]

cmd1: pipe[0]の書き込み側を使用
cmd2: pipe[0]の読み取り側、pipe[1]の書き込み側を使用
cmd3: pipe[1]の読み取り側を使用
```

---

### 2️⃣ Here_doc対応（2-3時間）

#### 必要な変更箇所

**A. src/main_bonus.c の分岐処理（20-30行追加）**
- Bonus用のmain関数内で分岐
- Mandatoryの`main.c`は一切変更しない

```c
// src/main_bonus.c
int main(int argc, char **argv, char **envp)
{
    if (argc > 1 && strcmp(argv[1], "here_doc") == 0)
        handle_heredoc(argc, argv, envp);  // here_doc専用関数
    else
        handle_multi_pipe(argc, argv, envp);   // 複数パイプ処理
}
```

**B. 新規ファイル: src/heredoc_bonus.c（60-90行）**
以下の機能が必要：
- `read_heredoc()` - 標準入力から終了文字列まで読み込み
- `handle_heredoc()` - here_doc全体の処理
- `open_append_file()` - ファイルをappendモードで開く（`>>`の実装）

#### 実装の詳細

```c
void read_heredoc(char *limiter)
{
    char *line;
    int pipefd[2];

    pipe(pipefd);
    while (1)
    {
        write(STDOUT_FILENO, "> ", 2);  // プロンプト表示
        line = get_next_line(STDIN_FILENO);  // libftのGNL使用
        if (strcmp(line, limiter) == 0)
            break;
        write(pipefd[1], line, strlen(line));
        free(line);
    }
    // pipefd[0]を標準入力にdup2する
}
```

#### 実装の複雑さ

**低-中程度** - 以下の点がポイント：
1. `get_next_line()`の活用（libftに既にある想定）
2. ファイルオープンモードを`O_APPEND`に変更
3. 引数数のバリデーション変更（通常5個 vs here_doc 6個）

---

## ファイル構成の変更

### ✅ Mandatoryファイル（変更なし・そのまま残す）

```
src/
├── main.c              # Mandatory用main関数
├── process.c           # 2コマンド固定の処理
├── pipe.c              # 基本パイプ処理
├── path.c              # PATH解決（Bonusで再利用可能）
├── file.c              # ファイル操作（Bonusで再利用可能）
├── error.c             # エラー処理（Bonusで再利用可能）
└── utils.c             # ユーティリティ（Bonusで再利用可能）

includes/
└── pipex.h             # Mandatory用ヘッダー
```

### ⭐ 新規作成が必要なBonusファイル

```
src/
├── main_bonus.c        # Bonus用main関数（新規）★
├── multi_pipe_bonus.c  # 複数パイプ処理（新規）★
├── heredoc_bonus.c     # here_doc処理（新規）★
└── process_bonus.c     # Bonus用プロセス管理（新規）★

includes/
└── pipex_bonus.h       # Bonus用ヘッダー（新規）★
```

### 🔄 共通ファイルの再利用戦略

**Bonusでも使えるファイル（変更不要）：**
- `src/path.c` - コマンド解決ロジックはそのまま
- `src/file.c` - ファイルオープン処理はそのまま（heredocは別途実装）
- `src/error.c` - エラーハンドリング関数はそのまま
- `src/utils.c` - ユーティリティ関数はそのまま

**Makefileで使い分ける：**
```makefile
# Mandatory
SRCS = src/main.c src/process.c src/pipe.c src/path.c src/file.c src/error.c src/utils.c

# Bonus（共通ファイル + Bonus専用ファイル）
SRCS_BONUS = src/main_bonus.c src/multi_pipe_bonus.c src/heredoc_bonus.c src/process_bonus.c \
             src/path.c src/file.c src/error.c src/utils.c
```

---

## テスト項目（2-3時間）

### 複数パイプのテスト
```bash
# 3コマンド
./pipex infile "cat" "grep a" "wc -l" outfile
< infile cat | grep a | wc -l > expected

# 4コマンド
./pipex infile "cat" "grep a" "sort" "wc -l" outfile
< infile cat | grep a | sort | wc -l > expected

# 5コマンド以上
./pipex infile "cmd1" "cmd2" "cmd3" "cmd4" "cmd5" outfile
```

### Here_docのテスト
```bash
./pipex here_doc EOF "cat" "wc -l" outfile
cat << EOF | wc -l >> expected

# 複数行入力
./pipex here_doc STOP "grep a" "wc -l" outfile
grep a << STOP | wc -l >> expected
```

### エッジケース
- コマンド数が多い場合（10個以上）
- 中間コマンドが失敗するケース
- ファイルディスクリプタの上限テスト

---

## 難易度評価

| 要素 | 難易度 | 理由 |
|------|--------|------|
| **複数パイプ** | ⭐⭐⭐☆☆ (中) | ループ処理・動的配列・fd管理が複雑 |
| **Here_doc** | ⭐⭐☆☆☆ (低-中) | 標準的な実装パターン、GNL活用 |
| **デバッグ** | ⭐⭐⭐⭐☆ (中-高) | プロセス間の動作確認が複雑 |

---

## リスク要因

### 1. ファイルディスクリプタ管理
- **リスク:** パイプが増えると、閉じ忘れでリークが発生しやすい
- **対策:** 各プロセスで使わないfdを確実にcloseするヘルパー関数を作る

### 2. メモリ管理
- **リスク:** 動的配列（pipes, pids）のメモリリーク
- **対策:** エラー時のクリーンアップパスを明確にする

### 3. エラーハンドリング
- **リスク:** 中間コマンドが失敗した時の挙動がbashと異なる可能性
- **対策:** 各ケースでbashと比較テストする

---

## 推奨実装順序

### フェーズ1: Bonusファイルの準備（30分）
1. ✏️ `src/main_bonus.c` の作成（骨格のみ）
2. ✏️ `includes/pipex_bonus.h` の作成
3. ✏️ Makefileに `bonus` ルールを追加

### フェーズ2: 複数パイプ（1日目）
1. ✏️ `src/multi_pipe_bonus.c` の骨格作成（1時間）
2. ✏️ `src/process_bonus.c` の作成（1時間）
3. ✏️ 3コマンドのケースで動作確認（2時間）
4. ✏️ 一般化（N個のコマンド対応）（1時間）
5. 🧪 テスト・デバッグ（1-2時間）

### フェーズ3: Here_doc（2日目）
1. ✏️ `src/heredoc_bonus.c` の作成（1時間）
2. ✏️ `src/main_bonus.c` への統合（30分）
3. 🧪 テスト・デバッグ（1時間）

### フェーズ4: 統合テスト（3日目）
1. 🧪 全テストケース実行
2. 🧪 Valgrind/ASanでメモリチェック
3. ✅ 最終調整
4. ✅ Mandatoryが壊れていないか確認（`make fclean && make`）

---

## まとめ

### Bonusまでの総作業量
- **新規ファイル数:** 4-5ファイル（全て`*_bonus.{c/h}`）
- **コード行数:** 230-320行（全て新規作成、既存コードは変更なし）
- **実装時間:** 8-12時間（デバッグ含む）
- **実装日数:** 2-3日（1日4時間作業想定）

### 新規作成ファイル一覧
1. `src/main_bonus.c` - Bonus用メイン関数
2. `src/multi_pipe_bonus.c` - 複数パイプ処理
3. `src/heredoc_bonus.c` - Here_doc処理
4. `src/process_bonus.c` - Bonus用プロセス管理
5. `includes/pipex_bonus.h` - Bonus用ヘッダー

### 再利用する既存ファイル（変更なし）
- `src/path.c` - PATH解決
- `src/file.c` - ファイル操作
- `src/error.c` - エラー処理
- `src/utils.c` - ユーティリティ

### Makefileの構造
```makefile
# Mandatory
SRCS = src/main.c src/process.c src/pipe.c src/path.c src/file.c src/error.c src/utils.c

# Bonus
SRCS_BONUS = src/main_bonus.c src/multi_pipe_bonus.c src/heredoc_bonus.c src/process_bonus.c \
             src/path.c src/file.c src/error.c src/utils.c

all: $(NAME)

bonus: $(OBJS_BONUS)
	$(CC) $(CFLAGS) $(OBJS_BONUS) -o $(NAME) -L$(LIBFT_DIR) -lft
```

### 重要な注意点
1. **Mandatoryファイルは一切変更しない** - `src/main.c`, `src/process.c`, `src/pipe.c` などは触らない
2. **評価は別々** - `make` でMandatory、`make bonus` でBonusをテスト
3. **main関数が2つ存在してもOK** - `main.c` と `main_bonus.c` は別々にコンパイルされる
4. **同じ実行ファイル名** - どちらも `./pipex` という名前（上書き）

### 現在の状態
- Mandatory partは本番レベル → **すぐにBonus開始可能**
- 既存コードがクリーンで構造化されている → 共通部分を再利用しやすい
- 包括的なテストケースがある → 検証しやすい

### 最終評価
**Bonusは十分実現可能** - 既存実装が堅牢で、`_bonus.c` ファイルとして独立実装できるので、Mandatoryを壊すリスクなく計画的に進められます。
