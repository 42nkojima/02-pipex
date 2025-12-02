# Pipex Test Cases

このドキュメントは、pipexの動作をbashと比較するためのテストケース集です。

## セットアップ

```bash
# テスト用の入力ファイルを作成
echo -e "a\na\na\nwdwd" > infile
```

---

## 1. 基本動作テスト

### Test 1.1: 基本的なパイプライン

**Pipex:**
```bash
./pipex infile "cat" "wc -l" outfile
cat outfile
```

**Bash (期待される動作):**
```bash
< infile cat | wc -l > expected
cat expected
```

**期待される出力:**
```
       4
```

**期待される終了コード:** `0`

---

### Test 1.2: grep を使ったフィルタリング

**Pipex:**
```bash
./pipex infile "cat" "grep a" outfile
cat outfile
```

**Bash (期待される動作):**
```bash
< infile cat | grep a > expected
cat expected
```

**期待される出力:**
```
a
a
a
```

**期待される終了コード:** `0`

---

### Test 1.3: 複雑なパイプライン

**Pipex:**
```bash
./pipex infile "grep a" "wc -l" outfile
cat outfile
```

**Bash (期待される動作):**
```bash
< infile grep a | wc -l > expected
cat expected
```

**期待される出力:**
```
       3
```

**期待される終了コード:** `0`

---

## 2. エラーハンドリングテスト

### Test 2.1: 第1コマンドが見つからない

**Pipex:**
```bash
./pipex infile "nonexistent_cmd1" "wc -l" outfile 2>&1
echo "Exit code: $?"
cat outfile
```

**Bash (期待される動作):**
```bash
< infile nonexistent_cmd1 | wc -l > expected 2>&1
echo "Exit code: $?"
cat expected
```

**期待される出力:**
```
pipex: nonexistent_cmd1: command not found
       0
Exit code: 0
```

**説明:** 第1コマンドが失敗しても、第2コマンドは空入力で実行され、終了コードは第2コマンドのもの(0)になる。

---

### Test 2.2: 第2コマンドが見つからない

**Pipex:**
```bash
./pipex infile "cat" "nonexistent_cmd2" outfile 2>&1
echo "Exit code: $?"
```

**Bash (期待される動作):**
```bash
< infile cat | nonexistent_cmd2 > expected 2>&1
echo "Exit code: $?"
```

**期待される出力:**
```
pipex: nonexistent_cmd2: command not found
Exit code: 127
```

**説明:** 第2コマンドが見つからない場合、終了コードは127になる。

---

### Test 2.3: 両方のコマンドが見つからない

**Pipex:**
```bash
./pipex infile "nonexistent1" "nonexistent2" outfile 2>&1
echo "Exit code: $?"
```

**Bash (期待される動作):**
```bash
< infile nonexistent1 | nonexistent2 > expected 2>&1
echo "Exit code: $?"
```

**期待される出力:**
```
pipex: nonexistent1: command not found
pipex: nonexistent2: command not found
Exit code: 127
```

**説明:** 両方失敗した場合でも、終了コードは第2コマンドのもの(127)になる。

---

## 3. ファイルエラーテスト

### Test 3.1: 入力ファイルが存在しない

**Pipex:**
```bash
./pipex nonexistent_file "cat" "wc -l" outfile 2>&1
echo "Exit code: $?"
cat outfile
```

**Bash (期待される動作):**
```bash
< nonexistent_file cat | wc -l > expected 2>&1
echo "Exit code: $?"
cat expected
```

**期待される出力:**
```
pipex: nonexistent_file: No such file or directory
       0
Exit code: 0
```

**説明:** 入力ファイルが存在しない場合、エラーメッセージを表示するが、空入力(`/dev/null`)でコマンドを実行する。これはbashの挙動と同じ。

---

### Test 3.2: 出力ファイルが書き込み不可

**Pipex:**
```bash
touch /tmp/readonly_file
chmod 000 /tmp/readonly_file
./pipex infile "cat" "wc -l" /tmp/readonly_file 2>&1
echo "Exit code: $?"
rm -f /tmp/readonly_file
```

**Bash (期待される動作):**
```bash
< infile cat | wc -l > /tmp/readonly_file 2>&1
echo "Exit code: $?"
```

**期待される出力:**
```
pipex: /tmp/readonly_file: Permission denied
Exit code: 1
```

**説明:** 出力ファイルを開けない場合、エラーメッセージを表示して終了する。

---

### Test 3.3: 入力ファイルが読み込み不可

**Pipex:**
```bash
touch /tmp/unreadable_file
chmod 000 /tmp/unreadable_file
./pipex /tmp/unreadable_file "cat" "wc -l" outfile 2>&1
echo "Exit code: $?"
cat outfile
rm -f /tmp/unreadable_file
```

**Bash (期待される動作):**
```bash
< /tmp/unreadable_file cat | wc -l > expected 2>&1
echo "Exit code: $?"
cat expected
```

**期待される出力:**
```
pipex: /tmp/unreadable_file: Permission denied
       0
Exit code: 0
```

**説明:** 読み込み不可の入力ファイルの場合も、エラーメッセージを表示して空入力で実行を続ける。

---

## 4. 引数エラーテスト

### Test 4.1: 引数が少なすぎる

**Pipex:**
```bash
./pipex infile "cat" 2>&1
echo "Exit code: $?"
```

**期待される出力:**
```
Usage: ./pipex file1 cmd1 cmd2 file2
Exit code: 1
```

---

### Test 4.2: 引数が多すぎる

**Pipex:**
```bash
./pipex infile "cat" "wc" outfile extra 2>&1
echo "Exit code: $?"
```

**期待される出力:**
```
Usage: ./pipex file1 cmd1 cmd2 file2
Exit code: 1
```

---

### Test 4.3: 引数なし

**Pipex:**
```bash
./pipex 2>&1
echo "Exit code: $?"
```

**期待される出力:**
```
Usage: ./pipex file1 cmd1 cmd2 file2
Exit code: 1
```

---

## 5. コマンド引数テスト

### Test 5.1: 引数付きコマンド

**Pipex:**
```bash
./pipex infile "cat -e" "grep a$" outfile
cat outfile
```

**Bash (期待される動作):**
```bash
< infile cat -e | grep 'a$' > expected
cat expected
```

**期待される出力:**
```
a$
a$
a$
```

---

### Test 5.2: 複数の引数を持つコマンド

**Pipex:**
```bash
echo -e "line1\nline2\nline3" > infile
./pipex infile "cat" "head -n 2" outfile
cat outfile
```

**Bash (期待される動作):**
```bash
< infile cat | head -n 2 > expected
cat expected
```

**期待される出力:**
```
line1
line2
```

---

## 6. 絶対パス・相対パステスト

### Test 6.1: 絶対パスでのコマンド指定

**Pipex:**
```bash
./pipex infile "/bin/cat" "/usr/bin/wc -l" outfile
cat outfile
```

**Bash (期待される動作):**
```bash
< infile /bin/cat | /usr/bin/wc -l > expected
cat expected
```

**期待される出力:**
```
       4
```

---

### Test 6.2: 相対パスでのコマンド指定

**Pipex:**
```bash
# 実行可能なスクリプトを作成
echo '#!/bin/bash' > /tmp/test_script.sh
echo 'cat' >> /tmp/test_script.sh
chmod +x /tmp/test_script.sh

./pipex infile "/tmp/test_script.sh" "wc -l" outfile
cat outfile
rm -f /tmp/test_script.sh
```

**Bash (期待される動作):**
```bash
< infile /tmp/test_script.sh | wc -l > expected
cat expected
```

**期待される出力:**
```
       4
```

---

## 7. 終了コードテスト

### Test 7.1: 正常終了

**Pipex:**
```bash
./pipex infile "cat" "wc -l" outfile
echo "Exit code: $?"
```

**期待される終了コード:** `0`

---

### Test 7.2: 第2コマンドが失敗

**Pipex:**
```bash
./pipex infile "cat" "grep nonexistent_pattern" outfile
echo "Exit code: $?"
```

**Bash (期待される動作):**
```bash
< infile cat | grep nonexistent_pattern > expected
echo "Exit code: $?"
```

**期待される終了コード:** `1` (grepがマッチしない場合)

---

### Test 7.3: コマンドが見つからない

**Pipex:**
```bash
./pipex infile "cat" "nonexistent" outfile 2>&1
echo "Exit code: $?"
```

**期待される終了コード:** `127`

---

### Test 7.4: 実行権限がない

**Pipex:**
```bash
echo '#!/bin/bash' > /tmp/no_exec.sh
echo 'cat' >> /tmp/no_exec.sh
chmod -x /tmp/no_exec.sh

./pipex infile "cat" "/tmp/no_exec.sh" outfile 2>&1
echo "Exit code: $?"
rm -f /tmp/no_exec.sh
```

**期待される出力:**
```
pipex: /tmp/no_exec.sh: Permission denied
Exit code: 126
```

**期待される終了コード:** `126`

---

## 8. エッジケーステスト

### Test 8.1: 空のファイル

**Pipex:**
```bash
touch empty_file
./pipex empty_file "cat" "wc -l" outfile
cat outfile
rm -f empty_file
```

**期待される出力:**
```
       0
```

---

### Test 8.2: 大きなファイル

**Pipex:**
```bash
seq 1 10000 > large_file
./pipex large_file "cat" "wc -l" outfile
cat outfile
rm -f large_file
```

**期待される出力:**
```
   10000
```

---

### Test 8.3: バイナリデータ

**Pipex:**
```bash
# バイナリファイルを作成
printf '\x00\x01\x02\x03' > binary_file
./pipex binary_file "cat" "wc -c" outfile
cat outfile
rm -f binary_file
```

**期待される出力:**
```
       4
```

---

## 9. 包括的なテストスクリプト

以下のスクリプトをコピーして実行することで、すべてのテストを一括実行できます：

```bash
#!/bin/bash

# 色の定義
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[0;33m'
NC='\033[0m' # No Color

# テストカウンター
TOTAL=0
PASSED=0
FAILED=0

# テスト用ファイルの準備
echo -e "a\na\na\nwdwd" > infile

# テスト関数
run_test() {
    local test_name="$1"
    local pipex_cmd="$2"
    local bash_cmd="$3"
    local check_exit_code="${4:-false}"

    TOTAL=$((TOTAL + 1))
    echo -e "\n${YELLOW}Test $TOTAL: $test_name${NC}"

    # Pipex実行
    eval "$pipex_cmd" > /tmp/pipex_out 2>&1
    local pipex_exit=$?

    # Bash実行
    eval "$bash_cmd" > /tmp/bash_out 2>&1
    local bash_exit=$?

    # 出力ファイルの比較
    if diff -q outfile expected > /dev/null 2>&1; then
        if [ "$check_exit_code" = "true" ]; then
            if [ $pipex_exit -eq $bash_exit ]; then
                echo -e "${GREEN}✓ PASSED${NC} (exit: $pipex_exit)"
                PASSED=$((PASSED + 1))
            else
                echo -e "${RED}✗ FAILED${NC} (pipex exit: $pipex_exit, bash exit: $bash_exit)"
                FAILED=$((FAILED + 1))
            fi
        else
            echo -e "${GREEN}✓ PASSED${NC}"
            PASSED=$((PASSED + 1))
        fi
    else
        echo -e "${RED}✗ FAILED${NC} (output differs)"
        FAILED=$((FAILED + 1))
    fi
}

# テスト実行
run_test "Basic pipeline" \
    "./pipex infile 'cat' 'wc -l' outfile" \
    "< infile cat | wc -l > expected"

run_test "Grep filtering" \
    "./pipex infile 'cat' 'grep a' outfile" \
    "< infile cat | grep a > expected"

run_test "Command not found (cmd1)" \
    "./pipex infile 'nonexistent1' 'wc -l' outfile" \
    "< infile nonexistent1 | wc -l > expected" \
    true

run_test "Command not found (cmd2)" \
    "./pipex infile 'cat' 'nonexistent2' outfile" \
    "< infile cat | nonexistent2 > expected" \
    true

run_test "Nonexistent input file" \
    "./pipex nonexistent_file 'cat' 'wc -l' outfile" \
    "< nonexistent_file cat | wc -l > expected" \
    true

# 結果表示
echo -e "\n========================================="
echo -e "${YELLOW}Test Summary${NC}"
echo -e "========================================="
echo -e "Total:  $TOTAL"
echo -e "${GREEN}Passed: $PASSED${NC}"
echo -e "${RED}Failed: $FAILED${NC}"
echo -e "=========================================\n"

# クリーンアップ
rm -f infile outfile expected /tmp/pipex_out /tmp/bash_out

# 終了コード
[ $FAILED -eq 0 ] && exit 0 || exit 1
```

---

## 10. メモリリークテスト

### Valgrind でのテスト

```bash
valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes \
    ./pipex infile "cat" "wc -l" outfile
```

**期待される結果:**
- `All heap blocks were freed -- no leaks are possible`
- ファイルディスクリプタリークなし

---

### Address Sanitizer でのテスト

```bash
make debug
./pipex infile "cat" "wc -l" outfile
```

**期待される結果:**
- エラーなし
- `LeakSanitizer: detected memory leaks` が出ないこと

---

## 重要な挙動の注意点

1. **終了コードは第2コマンドのものを返す**
   - 第1コマンドが失敗しても、第2コマンドが成功すれば終了コードは0
   - これはbashのパイプラインと同じ挙動

2. **入力ファイルエラー時の挙動**
   - 入力ファイルが開けない場合、エラーメッセージを表示
   - `/dev/null`を入力として使用し、実行は続行
   - これによりbashと同じ挙動を実現

3. **出力ファイルエラー時の挙動**
   - 出力ファイルが開けない場合、エラーメッセージを表示して終了
   - 終了コードは1

4. **コマンドが見つからない場合**
   - 終了コード: 127
   - エラーメッセージ: `pipex: command: command not found`

5. **実行権限がない場合**
   - 終了コード: 126
   - エラーメッセージ: `pipex: command: Permission denied`
