# ★ starhex - CLI Number Base & Hex Toolkit cho Arch Linux

**starhex** là công cụ dòng lệnh (CLI) siêu nhanh, viết bằng C++ chuẩn hiện đại, được tối ưu riêng cho **Arch Linux**. Công cụ này tự động nhận diện định dạng đầu vào và chuyển đổi linh hoạt giữa các hệ cơ số (Hex, Dec, Bin, Oct, Base64, Text, Color RGB, IPv4, Float/Double IEEE-754, Unix Timestamp) cùng các phép toán bitwise và endianness.

---

## 🚀 Tính năng nổi bật

- **Tự động nhận diện hệ cơ số (Auto-detection):**
  - `0x...`, `...h`, chữ cái `[A-F]` $\rightarrow$ Hexadecimal (Base 16)
  - `0b...`, `...b` $\rightarrow$ Binary (Base 2)
  - `0o...`, `...o` $\rightarrow$ Octal (Base 8)
  - `42`, `-100` $\rightarrow$ Decimal (Base 10)
  - `#FF5733`, `rgb(255, 87, 51)` $\rightarrow$ Color RGB / HSV
  - `192.168.1.1` $\rightarrow$ IPv4 Address
  - `3.14159` $\rightarrow$ IEEE-754 Floating point
  - Văn bản có khoảng trắng/chữ cái thường $\rightarrow$ ASCII String
- **Cú pháp ngắn gọn nhất:**
  - `starhex 42 -t2` $\rightarrow$ `101010` (Base 10 sang Base 2)
  - `starhex 0x3D -t10` $\rightarrow$ `61` (Hex sang Base 10)
  - `starhex 0b101010 -t16` $\rightarrow$ `2a` (Bin sang Hex)
  - `starhex 0o755 -t10` $\rightarrow$ `493` (Oct sang Base 10)
- **Arbitrary Precision (BigInt):**
  - Không bị giới hạn 64-bit! Hỗ trợ xử lý số cực lớn (128-bit, 256-bit hash, 512-bit, v.v.).
- **Tùy biến định dạng:**
  - `-p` / `--prefix`: Thêm tiền tố (`0x`, `0b`, `0o`, `#`).
  - `-w<N>` / `--width <N>`: Zero-padding theo số bit hoặc ký tự (ví dụ: `-w8`, `-w16`, `-w32`, `-w64`).
  - `-g<N>` / `--group <N>`: Nhóm ký tự cách nhau khoảng trắng (ví dụ: `-g4` nhóm theo nibble `0010 1010`).
  - `-u` / `-l`: In hoa chữ cái (`2A`) hoặc in thường (`2a`).
  - `-q` / `--quiet`: Trả về kết quả thuần túy (thuận tiện cho pipe/bash script).
- **Dashboard kiểm tra đa hệ số (Inspector Mode):**
  - Chạy `starhex <giá trị>` mà không truyền `-t` sẽ hiển thị bảng thông tin chi tiết: Decimal, Hex, Octal, Binary, ASCII, Bit Length, Popcount (Hamming weight), Endian Swap (16/32/64-bit), Preview màu sắc RGB (Truecolor ANSI), IPv4, Unix Timestamp.
- **Hỗ trợ Stdin Pipe:**
  - `echo "0xDEADBEEF" | starhex -t10`
  - `cat hashes.txt | starhex -t10`
- **Tích hợp sẵn gói cho Arch Linux:**
  - Có sẵn `PKGBUILD`, `Makefile`, `CMakeLists.txt`, `install.sh`.
  - Shell completion cho `bash`, `zsh`, `fish`.
  - Man page `man starhex`.

---

## 📦 Hướng dẫn cài đặt trên Arch Linux

### Cách 1: Sử dụng `makepkg` (Chuẩn Arch Linux)

Mở terminal trong thư mục dự án trên Arch Linux và chạy:
```bash
makepkg -si
```
Lệnh này sẽ tự động biên dịch, đóng gói thành file `.pkg.tar.zst` và cài đặt vào hệ thống Arch Linux. Lệnh `starhex` sẽ sẵn sàng trên toàn hệ thống tại `/usr/bin/starhex`.

### Cách 2: Sử dụng script `install.sh`

```bash
chmod +x install.sh
./install.sh
```

### Cách 3: Sử dụng `make` hoặc `cmake` trực tiếp

```bash
# Bằng Makefile
make
sudo make install

# Hoặc bằng CMake
cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr
cmake --build build
sudo cmake --install build
```

---

## 📖 Bảng tra cứu lệnh & Tuỳ chọn

| Tuỳ chọn | Chức năng | Ví dụ |
| :--- | :--- | :--- |
| `-t2`, `-tb`, `-tbin` | Đổi sang Binary (Cơ số 2) | `starhex 42 -t2` $\rightarrow$ `101010` |
| `-t8`, `-to`, `-toct` | Đổi sang Octal (Cơ số 8) | `starhex 42 -t8` $\rightarrow$ `52` |
| `-t10`, `-td`, `-tdec`| Đổi sang Decimal (Cơ số 10)| `starhex 0x3D -t10` $\rightarrow$ `61` |
| `-t16`, `-tx`, `-thex`| Đổi sang Hexadecimal (Cơ số 16) | `starhex 42 -t16` $\rightarrow$ `2a` |
| `-t<N>` | Đổi sang bất kỳ cơ số nào ($2 \le N \le 62$) | `starhex 255 -t36` $\rightarrow$ `73` |
| `-ts`, `-tstr` | Chuyển hex sang chuỗi văn bản ASCII | `starhex 48656c6c6f -ts` $\rightarrow$ `Hello` |
| `-tb64` | Mã hoá Base64 | `starhex "Hello" -tb64` $\rightarrow$ `SGVsbG8=` |
| `-trgb` | Chuyển hex sang `rgb(r, g, b)` | `starhex "#FF5733" -trgb` $\rightarrow$ `rgb(255, 87, 51)` |
| `-thsv` | Chuyển hex sang `hsv(h, s%, v%)` | `starhex "#FF5733" -thsv` $\rightarrow$ `hsv(11, 80%, 100%)` |
| `-tip` | Chuyển số hex/dec sang IPv4 | `starhex 0xC0A80101 -tip` $\rightarrow$ `192.168.1.1` |
| `-tfloat` | Chuyển đổi float IEEE-754 32-bit | `starhex 3.14159 -tfloat` $\rightarrow$ `0x40490fd0` |
| `-tdouble`| Chuyển đổi double IEEE-754 64-bit | `starhex 3.14159 -tdouble` $\rightarrow$ `0x400921fb54442d18` |
| `-ttime` | Chuyển timestamp Unix sang giờ UTC | `starhex 1700000000 -ttime` $\rightarrow$ UTC date string |
| `-p` / `--prefix` | Thêm tiền tố định dạng (`0x`, `0b`, `0o`, `#`) | `starhex 42 -t2 -p` $\rightarrow$ `0b101010` |
| `-np` / `--no-prefix` | Bỏ qua tiền tố (mặc định khi dùng `-t`) | `starhex 0x3D -t2 -np` $\rightarrow$ `111101` |
| `-w<N>` | Độn 0 (zero-padding) theo độ dài N | `starhex 42 -t2 -w8` $\rightarrow$ `00101010` |
| `-g<N>` | Gom nhóm các ký tự theo từng cụm N | `starhex 42 -t2 -w8 -g4` $\rightarrow$ `0010 1010` |
| `-u` / `-l` | Chữ hoa (`-u`) hoặc chữ thường (`-l`) | `starhex 42 -t16 -u` $\rightarrow$ `2A` |
| `-e16`, `-e32`, `-e64` | Đảo thứ tự byte (Endianness swap) | `starhex 0x1234 -e16 -tx -p` $\rightarrow$ `0x3412` |
| `--not` | Phép toán Bitwise NOT | `starhex 0x0F --not -tx` $\rightarrow$ Inverted hex |
| `--and <val>` | Phép toán Bitwise AND | `starhex 0x0F --and 0x33 -tx` $\rightarrow$ `3` |
| `--or <val>` | Phép toán Bitwise OR | `starhex 0x0F --or 0x30 -tx` $\rightarrow$ `3f` |
| `--xor <val>` | Phép toán Bitwise XOR | `starhex 0x0F --xor 0xFF -tx` $\rightarrow$ `f0` |
| `--shl <N>` | Dịch trái N bit | `starhex 0x01 --shl 4 -tx` $\rightarrow$ `10` |
| `--shr <N>` | Dịch phải N bit | `starhex 0x10 --shr 2 -tx` $\rightarrow$ `4` |
| `-a` / `--all` | Bắt buộc hiển thị bảng dashboard kiểm tra | `starhex 42 -a` |
| `-q` / `--quiet` | Chế độ im lặng, chỉ xuất kết quả thuần túy | `starhex 42 -t2 -q` |
| `-h` / `--help` | Hiển thị bảng trợ giúp chi tiết | `starhex --help` |

---

## 💡 Ví dụ thực tế

```bash
# 1. Chuyển đổi cơ số cơ bản
starhex 42 -t2
# Kết quả: 101010

starhex 0x3D -t10
# Kết quả: 61

starhex 0b101010 -t16
# Kết quả: 2a

starhex 0o755 -t10
# Kết quả: 493

# 2. Định dạng nâng cao (Thêm prefix, zero-pad 8 bit, gom nhóm 4 bit)
starhex 42 -t2 -p -w8 -g4
# Kết quả: 0b0010 1010

# 3. Chuyển đổi màu sắc
starhex "#FF5733" -trgb
# Kết quả: rgb(255, 87, 51)

starhex "rgb(255, 87, 51)" -tx -p
# Kết quả: #ff5733

# 4. Chuyển đổi địa chỉ mạng IPv4
starhex 192.168.1.1 -tx -p
# Kết quả: 0xc0a80101

starhex 0xC0A80101 -tip
# Kết quả: 192.168.1.1

# 5. Chuyển đổi chuỗi ký tự sang Hex & ngược lại
starhex "Hello World" -thex
# Kết quả: 48656c6c6f20576f726c64

starhex 48656c6c6f20576f726c64 -ts
# Kết quả: Hello World

# 6. Đảo Endianness
starhex 0x1234 -e16 -tx -p
# Kết quả: 0x3412

starhex 0x12345678 -e32 -tx -p
# Kết quả: 0x78563412

# 7. Tính toán Bitwise
starhex 0x0F --and 0x33 -tx
# Kết quả: 3

starhex 0x01 --shl 8 -tx -p
# Kết quả: 0x100

# 8. Xem bảng Dashboard chi tiết
starhex 0x3D
```

---

## 🛠 Cấu trúc thư mục dự án trên Desktop

```
Desktop/starhex/
├── CMakeLists.txt              # Cấu hình build CMake cho Arch Linux
├── Makefile                    # Makefile tiêu chuẩn
├── PKGBUILD                    # Script đóng gói makepkg cho Arch Linux
├── install.sh                  # Script cài đặt tự động
├── src/
│   └── main.cpp                # Mã nguồn C++17 hoàn chỉnh
├── completions/
│   ├── starhex.bash            # Auto-completion cho Bash
│   ├── _starhex                # Auto-completion cho Zsh
│   └── starhex.fish            # Auto-completion cho Fish
├── doc/
│   └── starhex.1               # Man page UNIX
└── README.md                   # Tài liệu hướng dẫn sử dụng
```

---

## 📌 Gỡ cài đặt

Nếu đã cài qua `makepkg` (pacman):
```bash
sudo pacman -R starhex
```

Nếu đã cài qua `make install`:
```bash
sudo make uninstall
```
