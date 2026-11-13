/* dit nhau by lovevsick */ #include <iostream>
/* dit nhau by lovevsick */ #include <string>
/* dit nhau by lovevsick */ #include <vector>
/* dit nhau by lovevsick */ #include <algorithm>
/* dit nhau by lovevsick */ #include <sstream>
/* dit nhau by lovevsick */ #include <iomanip>
/* dit nhau by lovevsick */ #include <cstdint>
/* dit nhau by lovevsick */ #include <cctype>
/* dit nhau by lovevsick */ #include <cstring>
/* dit nhau by lovevsick */ #include <ctime>
/* dit nhau by lovevsick */ #include <cmath>
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */ #if defined(_WIN32)
/* dit nhau by lovevsick */ #include <io.h>
/* dit nhau by lovevsick */ #define IS_TTY() _isatty(_fileno(stdout))
/* dit nhau by lovevsick */ #else
/* dit nhau by lovevsick */ #include <unistd.h>
/* dit nhau by lovevsick */ #define IS_TTY() isatty(fileno(stdout))
/* dit nhau by lovevsick */ #endif
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */ struct Config {
/* dit nhau by lovevsick */     std::string input;
/* dit nhau by lovevsick */     int force_base = 0;
/* dit nhau by lovevsick */     std::string force_type = "";
/* dit nhau by lovevsick */     int target_base = 0;
/* dit nhau by lovevsick */     std::string target_type = "";
/* dit nhau by lovevsick */     bool uppercase = false;
/* dit nhau by lovevsick */     bool lowercase = false;
/* dit nhau by lovevsick */     bool prefix = false;
/* dit nhau by lovevsick */     bool no_prefix = false;
/* dit nhau by lovevsick */     int width = 0;
/* dit nhau by lovevsick */     int group = 0;
/* dit nhau by lovevsick */     bool quiet = false;
/* dit nhau by lovevsick */     bool force_all = false;
/* dit nhau by lovevsick */     bool use_color = true;
/* dit nhau by lovevsick */     int endian_swap = 0;
/* dit nhau by lovevsick */     bool op_not = false;
/* dit nhau by lovevsick */     std::string op_and = "";
/* dit nhau by lovevsick */     std::string op_or = "";
/* dit nhau by lovevsick */     std::string op_xor = "";
/* dit nhau by lovevsick */     int op_shl = 0;
/* dit nhau by lovevsick */     int op_shr = 0;
/* dit nhau by lovevsick */ };
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */ struct BigInt {
/* dit nhau by lovevsick */     bool negative = false;
/* dit nhau by lovevsick */     std::vector<uint32_t> limbs;
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */     BigInt() { limbs.push_back(0); }
/* dit nhau by lovevsick */     BigInt(uint64_t val) {
/* dit nhau by lovevsick */         limbs.push_back((uint32_t)(val & 0xFFFFFFFFULL));
/* dit nhau by lovevsick */         if ((val >> 32) > 0) limbs.push_back((uint32_t)(val >> 32));
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */     void trim() {
/* dit nhau by lovevsick */         while (limbs.size() > 1 && limbs.back() == 0) limbs.pop_back();
/* dit nhau by lovevsick */         if (limbs.size() == 1 && limbs[0] == 0) negative = false;
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */     bool is_zero() const { return limbs.size() <= 1 && limbs[0] == 0; }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */     int bit_length() const {
/* dit nhau by lovevsick */         if (is_zero()) return 0;
/* dit nhau by lovevsick */         int top = 32 - __builtin_clz(limbs.back());
/* dit nhau by lovevsick */         return (int)(limbs.size() - 1) * 32 + top;
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */     int popcount() const {
/* dit nhau by lovevsick */         int cnt = 0;
/* dit nhau by lovevsick */         for (uint32_t l : limbs) cnt += __builtin_popcount(l);
/* dit nhau by lovevsick */         return cnt;
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */     uint64_t to_u64() const {
/* dit nhau by lovevsick */         uint64_t res = limbs[0];
/* dit nhau by lovevsick */         if (limbs.size() > 1) res |= ((uint64_t)limbs[1] << 32);
/* dit nhau by lovevsick */         return res;
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */     static int char_val_std(char c) {
/* dit nhau by lovevsick */         if (c >= '0' && c <= '9') return c - '0';
/* dit nhau by lovevsick */         if (c >= 'a' && c <= 'z') return c - 'a' + 10;
/* dit nhau by lovevsick */         if (c >= 'A' && c <= 'Z') return c - 'A' + 10;
/* dit nhau by lovevsick */         return -1;
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */     static int char_val_ext(char c) {
/* dit nhau by lovevsick */         if (c >= '0' && c <= '9') return c - '0';
/* dit nhau by lovevsick */         if (c >= 'a' && c <= 'z') return c - 'a' + 10;
/* dit nhau by lovevsick */         if (c >= 'A' && c <= 'Z') return c - 'A' + 36;
/* dit nhau by lovevsick */         return -1;
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */     static BigInt from_string(const std::string& str, int base) {
/* dit nhau by lovevsick */         BigInt res;
/* dit nhau by lovevsick */         res.limbs.clear();
/* dit nhau by lovevsick */         res.limbs.push_back(0);
/* dit nhau by lovevsick */         size_t start = 0;
/* dit nhau by lovevsick */         if (!str.empty() && str[0] == '-') {
/* dit nhau by lovevsick */             res.negative = true;
/* dit nhau by lovevsick */             start = 1;
/* dit nhau by lovevsick */         } else if (!str.empty() && str[0] == '+') {
/* dit nhau by lovevsick */             start = 1;
/* dit nhau by lovevsick */         }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */         for (size_t i = start; i < str.size(); ++i) {
/* dit nhau by lovevsick */             char c = str[i];
/* dit nhau by lovevsick */             if (c == '_' || c == ' ' || c == '\'' || c == ',') continue;
/* dit nhau by lovevsick */             int d = (base <= 36) ? char_val_std(c) : char_val_ext(c);
/* dit nhau by lovevsick */             if (d < 0 || d >= base) continue;
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */             uint64_t carry = (uint64_t)d;
/* dit nhau by lovevsick */             for (size_t j = 0; j < res.limbs.size(); ++j) {
/* dit nhau by lovevsick */                 uint64_t cur = (uint64_t)res.limbs[j] * base + carry;
/* dit nhau by lovevsick */                 res.limbs[j] = (uint32_t)(cur & 0xFFFFFFFFULL);
/* dit nhau by lovevsick */                 carry = cur >> 32;
/* dit nhau by lovevsick */             }
/* dit nhau by lovevsick */             while (carry > 0) {
/* dit nhau by lovevsick */                 res.limbs.push_back((uint32_t)(carry & 0xFFFFFFFFULL));
/* dit nhau by lovevsick */                 carry >>= 32;
/* dit nhau by lovevsick */             }
/* dit nhau by lovevsick */         }
/* dit nhau by lovevsick */         res.trim();
/* dit nhau by lovevsick */         return res;
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */     std::string to_string(int base, bool upper = false) const {
/* dit nhau by lovevsick */         if (is_zero()) return "0";
/* dit nhau by lovevsick */         std::vector<uint32_t> temp = limbs;
/* dit nhau by lovevsick */         std::string result;
/* dit nhau by lovevsick */         const char* digits_lower = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
/* dit nhau by lovevsick */         const char* digits_upper = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
/* dit nhau by lovevsick */         const char* digits = upper ? digits_upper : digits_lower;
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */         while (temp.size() > 1 || (temp.size() == 1 && temp[0] > 0)) {
/* dit nhau by lovevsick */             uint64_t rem = 0;
/* dit nhau by lovevsick */             for (int i = (int)temp.size() - 1; i >= 0; --i) {
/* dit nhau by lovevsick */                 uint64_t cur = temp[i] + (rem << 32);
/* dit nhau by lovevsick */                 temp[i] = (uint32_t)(cur / base);
/* dit nhau by lovevsick */                 rem = cur % base;
/* dit nhau by lovevsick */             }
/* dit nhau by lovevsick */             while (temp.size() > 1 && temp.back() == 0) temp.pop_back();
/* dit nhau by lovevsick */             result.push_back(digits[rem]);
/* dit nhau by lovevsick */         }
/* dit nhau by lovevsick */         if (negative) result.push_back('-');
/* dit nhau by lovevsick */         std::reverse(result.begin(), result.end());
/* dit nhau by lovevsick */         return result;
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */     BigInt bitwise_and(const BigInt& other) const {
/* dit nhau by lovevsick */         BigInt res;
/* dit nhau by lovevsick */         res.limbs.resize(std::min(limbs.size(), other.limbs.size()));
/* dit nhau by lovevsick */         for (size_t i = 0; i < res.limbs.size(); ++i) res.limbs[i] = limbs[i] & other.limbs[i];
/* dit nhau by lovevsick */         res.trim();
/* dit nhau by lovevsick */         return res;
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */     BigInt bitwise_or(const BigInt& other) const {
/* dit nhau by lovevsick */         BigInt res;
/* dit nhau by lovevsick */         size_t max_size = std::max(limbs.size(), other.limbs.size());
/* dit nhau by lovevsick */         res.limbs.resize(max_size, 0);
/* dit nhau by lovevsick */         for (size_t i = 0; i < max_size; ++i) {
/* dit nhau by lovevsick */             uint32_t a = (i < limbs.size()) ? limbs[i] : 0;
/* dit nhau by lovevsick */             uint32_t b = (i < other.limbs.size()) ? other.limbs[i] : 0;
/* dit nhau by lovevsick */             res.limbs[i] = a | b;
/* dit nhau by lovevsick */         }
/* dit nhau by lovevsick */         res.trim();
/* dit nhau by lovevsick */         return res;
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */     BigInt bitwise_xor(const BigInt& other) const {
/* dit nhau by lovevsick */         BigInt res;
/* dit nhau by lovevsick */         size_t max_size = std::max(limbs.size(), other.limbs.size());
/* dit nhau by lovevsick */         res.limbs.resize(max_size, 0);
/* dit nhau by lovevsick */         for (size_t i = 0; i < max_size; ++i) {
/* dit nhau by lovevsick */             uint32_t a = (i < limbs.size()) ? limbs[i] : 0;
/* dit nhau by lovevsick */             uint32_t b = (i < other.limbs.size()) ? other.limbs[i] : 0;
/* dit nhau by lovevsick */             res.limbs[i] = a ^ b;
/* dit nhau by lovevsick */         }
/* dit nhau by lovevsick */         res.trim();
/* dit nhau by lovevsick */         return res;
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */     BigInt bitwise_not(int width_bits = 0) const {
/* dit nhau by lovevsick */         BigInt res = *this;
/* dit nhau by lovevsick */         if (width_bits <= 0) width_bits = std::max(8, ((bit_length() + 7) / 8) * 8);
/* dit nhau by lovevsick */         size_t num_limbs = (width_bits + 31) / 32;
/* dit nhau by lovevsick */         if (res.limbs.size() < num_limbs) res.limbs.resize(num_limbs, 0);
/* dit nhau by lovevsick */         for (size_t i = 0; i < num_limbs; ++i) res.limbs[i] = ~res.limbs[i];
/* dit nhau by lovevsick */         int excess_bits = (int)num_limbs * 32 - width_bits;
/* dit nhau by lovevsick */         if (excess_bits > 0) {
/* dit nhau by lovevsick */             uint32_t mask = (excess_bits >= 32) ? 0 : (0xFFFFFFFFU >> excess_bits);
/* dit nhau by lovevsick */             res.limbs[num_limbs - 1] &= mask;
/* dit nhau by lovevsick */         }
/* dit nhau by lovevsick */         res.trim();
/* dit nhau by lovevsick */         return res;
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */     BigInt shift_left(int n) const {
/* dit nhau by lovevsick */         if (is_zero() || n <= 0) return *this;
/* dit nhau by lovevsick */         int limb_shift = n / 32;
/* dit nhau by lovevsick */         int bit_shift = n % 32;
/* dit nhau by lovevsick */         BigInt res;
/* dit nhau by lovevsick */         res.limbs.assign(limb_shift, 0);
/* dit nhau by lovevsick */         uint32_t carry = 0;
/* dit nhau by lovevsick */         for (uint32_t l : limbs) {
/* dit nhau by lovevsick */             uint64_t cur = ((uint64_t)l << bit_shift) | carry;
/* dit nhau by lovevsick */             res.limbs.push_back((uint32_t)(cur & 0xFFFFFFFFULL));
/* dit nhau by lovevsick */             carry = (uint32_t)(cur >> 32);
/* dit nhau by lovevsick */         }
/* dit nhau by lovevsick */         if (carry > 0) res.limbs.push_back(carry);
/* dit nhau by lovevsick */         res.trim();
/* dit nhau by lovevsick */         return res;
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */     BigInt shift_right(int n) const {
/* dit nhau by lovevsick */         if (is_zero() || n <= 0) return *this;
/* dit nhau by lovevsick */         int limb_shift = n / 32;
/* dit nhau by lovevsick */         int bit_shift = n % 32;
/* dit nhau by lovevsick */         if (limb_shift >= (int)limbs.size()) return BigInt(0);
/* dit nhau by lovevsick */         BigInt res;
/* dit nhau by lovevsick */         for (size_t i = limb_shift; i < limbs.size(); ++i) {
/* dit nhau by lovevsick */             uint32_t cur = limbs[i] >> bit_shift;
/* dit nhau by lovevsick */             if (bit_shift > 0 && i + 1 < limbs.size()) {
/* dit nhau by lovevsick */                 cur |= (limbs[i + 1] << (32 - bit_shift));
/* dit nhau by lovevsick */             }
/* dit nhau by lovevsick */             res.limbs.push_back(cur);
/* dit nhau by lovevsick */         }
/* dit nhau by lovevsick */         res.trim();
/* dit nhau by lovevsick */         return res;
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */     BigInt swap_endian(int bits) const {
/* dit nhau by lovevsick */         uint64_t val = to_u64();
/* dit nhau by lovevsick */         if (bits == 16) {
/* dit nhau by lovevsick */             uint16_t v = (uint16_t)val;
/* dit nhau by lovevsick */             return BigInt((uint16_t)__builtin_bswap16(v));
/* dit nhau by lovevsick */         } else if (bits == 32) {
/* dit nhau by lovevsick */             return BigInt((uint32_t)__builtin_bswap32((uint32_t)val));
/* dit nhau by lovevsick */         } else if (bits == 64) {
/* dit nhau by lovevsick */             return BigInt((uint64_t)__builtin_bswap64(val));
/* dit nhau by lovevsick */         }
/* dit nhau by lovevsick */         return *this;
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */ };
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */ static std::string trim_spaces(const std::string& str) {
/* dit nhau by lovevsick */     size_t first = str.find_first_not_of(" \t\r\n");
/* dit nhau by lovevsick */     if (first == std::string::npos) return "";
/* dit nhau by lovevsick */     size_t last = str.find_last_not_of(" \t\r\n");
/* dit nhau by lovevsick */     return str.substr(first, last - first + 1);
/* dit nhau by lovevsick */ }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */ static bool is_plain_text(const std::string& s) {
/* dit nhau by lovevsick */     std::string clean = trim_spaces(s);
/* dit nhau by lovevsick */     if (clean.size() >= 2 && (clean.substr(0, 2) == "0x" || clean.substr(0, 2) == "0X" ||
/* dit nhau by lovevsick */                               clean.substr(0, 2) == "0b" || clean.substr(0, 2) == "0B" ||
/* dit nhau by lovevsick */                               clean.substr(0, 2) == "0o" || clean.substr(0, 2) == "0O")) {
/* dit nhau by lovevsick */         return false;
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */     for (char c : clean) {
/* dit nhau by lovevsick */         if (std::isalnum((unsigned char)c) && !std::isxdigit((unsigned char)c)) {
/* dit nhau by lovevsick */             return true;
/* dit nhau by lovevsick */         }
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */     return false;
/* dit nhau by lovevsick */ }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */ static std::string base64_encode(const std::string& in) {
/* dit nhau by lovevsick */     static const char* tbl = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
/* dit nhau by lovevsick */     std::string out;
/* dit nhau by lovevsick */     int val = 0, valb = -6;
/* dit nhau by lovevsick */     for (unsigned char c : in) {
/* dit nhau by lovevsick */         val = (val << 8) + c;
/* dit nhau by lovevsick */         valb += 8;
/* dit nhau by lovevsick */         while (valb >= 0) {
/* dit nhau by lovevsick */             out.push_back(tbl[(val >> valb) & 0x3F]);
/* dit nhau by lovevsick */             valb -= 6;
/* dit nhau by lovevsick */         }
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */     if (valb > -6) out.push_back(tbl[((val << 8) >> (valb + 8)) & 0x3F]);
/* dit nhau by lovevsick */     while (out.size() % 4) out.push_back('=');
/* dit nhau by lovevsick */     return out;
/* dit nhau by lovevsick */ }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */ static std::string base64_decode(const std::string& in) {
/* dit nhau by lovevsick */     std::vector<int> T(256, -1);
/* dit nhau by lovevsick */     const char* tbl = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
/* dit nhau by lovevsick */     for (int i = 0; i < 64; i++) T[(unsigned char)tbl[i]] = i;
/* dit nhau by lovevsick */     std::string out;
/* dit nhau by lovevsick */     int val = 0, valb = -8;
/* dit nhau by lovevsick */     for (unsigned char c : in) {
/* dit nhau by lovevsick */         if (T[c] == -1) break;
/* dit nhau by lovevsick */         val = (val << 6) + T[c];
/* dit nhau by lovevsick */         valb += 6;
/* dit nhau by lovevsick */         if (valb >= 0) {
/* dit nhau by lovevsick */             out.push_back(char((val >> valb) & 0xFF));
/* dit nhau by lovevsick */             valb -= 8;
/* dit nhau by lovevsick */         }
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */     return out;
/* dit nhau by lovevsick */ }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */ static std::string text_to_hex(const std::string& s, bool upper = false) {
/* dit nhau by lovevsick */     std::ostringstream oss;
/* dit nhau by lovevsick */     for (unsigned char c : s) {
/* dit nhau by lovevsick */         oss << std::hex << std::setw(2) << std::setfill('0');
/* dit nhau by lovevsick */         if (upper) oss << std::uppercase;
/* dit nhau by lovevsick */         oss << (int)c;
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */     return oss.str();
/* dit nhau by lovevsick */ }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */ static std::string hex_to_text(const std::string& h) {
/* dit nhau by lovevsick */     std::string clean;
/* dit nhau by lovevsick */     for (char c : h) if (isxdigit((unsigned char)c)) clean.push_back(c);
/* dit nhau by lovevsick */     if (clean.size() % 2 != 0) clean = "0" + clean;
/* dit nhau by lovevsick */     std::string out;
/* dit nhau by lovevsick */     for (size_t i = 0; i < clean.size(); i += 2) {
/* dit nhau by lovevsick */         std::string byte_str = clean.substr(i, 2);
/* dit nhau by lovevsick */         char byte_val = (char)std::strtol(byte_str.c_str(), nullptr, 16);
/* dit nhau by lovevsick */         out.push_back(byte_val);
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */     return out;
/* dit nhau by lovevsick */ }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */ static std::string pad_string(const std::string& s, int width, char pad = '0') {
/* dit nhau by lovevsick */     if ((int)s.size() >= width) return s;
/* dit nhau by lovevsick */     return std::string(width - s.size(), pad) + s;
/* dit nhau by lovevsick */ }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */ static std::string group_string(const std::string& s, int group_size, char sep = ' ') {
/* dit nhau by lovevsick */     if (group_size <= 0 || s.empty()) return s;
/* dit nhau by lovevsick */     std::string out;
/* dit nhau by lovevsick */     int rem = (int)s.size() % group_size;
/* dit nhau by lovevsick */     size_t i = 0;
/* dit nhau by lovevsick */     if (rem > 0) {
/* dit nhau by lovevsick */         out += s.substr(0, rem);
/* dit nhau by lovevsick */         i = rem;
/* dit nhau by lovevsick */         if (i < s.size()) out.push_back(sep);
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */     for (; i < s.size(); i += group_size) {
/* dit nhau by lovevsick */         out += s.substr(i, group_size);
/* dit nhau by lovevsick */         if (i + group_size < s.size()) out.push_back(sep);
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */     return out;
/* dit nhau by lovevsick */ }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */ static bool parse_ipv4(const std::string& str, uint32_t& ip) {
/* dit nhau by lovevsick */     int parts[4];
/* dit nhau by lovevsick */     char extra;
/* dit nhau by lovevsick */     if (sscanf(str.c_str(), "%d.%d.%d.%d%c", &parts[0], &parts[1], &parts[2], &parts[3], &extra) == 4) {
/* dit nhau by lovevsick */         for (int i = 0; i < 4; ++i) if (parts[i] < 0 || parts[i] > 255) return false;
/* dit nhau by lovevsick */         ip = ((uint32_t)parts[0] << 24) | ((uint32_t)parts[1] << 16) | ((uint32_t)parts[2] << 8) | (uint32_t)parts[3];
/* dit nhau by lovevsick */         return true;
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */     return false;
/* dit nhau by lovevsick */ }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */ static std::string format_ipv4(uint32_t ip) {
/* dit nhau by lovevsick */     std::ostringstream oss;
/* dit nhau by lovevsick */     oss << ((ip >> 24) & 0xFF) << "." << ((ip >> 16) & 0xFF) << "." << ((ip >> 8) & 0xFF) << "." << (ip & 0xFF);
/* dit nhau by lovevsick */     return oss.str();
/* dit nhau by lovevsick */ }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */ static bool parse_rgb_string(const std::string& str, uint32_t& rgb) {
/* dit nhau by lovevsick */     int r, g, b;
/* dit nhau by lovevsick */     if (sscanf(str.c_str(), "rgb(%d,%d,%d)", &r, &g, &b) == 3 ||
/* dit nhau by lovevsick */         sscanf(str.c_str(), "rgb(%d, %d, %d)", &r, &g, &b) == 3 ||
/* dit nhau by lovevsick */         sscanf(str.c_str(), "%d,%d,%d", &r, &g, &b) == 3) {
/* dit nhau by lovevsick */         if (r >= 0 && r <= 255 && g >= 0 && g <= 255 && b >= 0 && b <= 255) {
/* dit nhau by lovevsick */             rgb = ((uint32_t)r << 16) | ((uint32_t)g << 8) | (uint32_t)b;
/* dit nhau by lovevsick */             return true;
/* dit nhau by lovevsick */         }
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */     return false;
/* dit nhau by lovevsick */ }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */ static std::string format_rgb(uint32_t rgb) {
/* dit nhau by lovevsick */     int r = (rgb >> 16) & 0xFF;
/* dit nhau by lovevsick */     int g = (rgb >> 8) & 0xFF;
/* dit nhau by lovevsick */     int b = rgb & 0xFF;
/* dit nhau by lovevsick */     std::ostringstream oss;
/* dit nhau by lovevsick */     oss << "rgb(" << r << ", " << g << ", " << b << ")";
/* dit nhau by lovevsick */     return oss.str();
/* dit nhau by lovevsick */ }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */ static std::string format_hsv(uint32_t rgb) {
/* dit nhau by lovevsick */     double r = ((rgb >> 16) & 0xFF) / 255.0;
/* dit nhau by lovevsick */     double g = ((rgb >> 8) & 0xFF) / 255.0;
/* dit nhau by lovevsick */     double b = (rgb & 0xFF) / 255.0;
/* dit nhau by lovevsick */     double cmax = std::max({r, g, b});
/* dit nhau by lovevsick */     double cmin = std::min({r, g, b});
/* dit nhau by lovevsick */     double diff = cmax - cmin;
/* dit nhau by lovevsick */     double h = 0, s = (cmax == 0) ? 0 : (diff / cmax), v = cmax;
/* dit nhau by lovevsick */     if (diff > 0.00001) {
/* dit nhau by lovevsick */         if (cmax == r) h = fmod((60 * ((g - b) / diff) + 360), 360.0);
/* dit nhau by lovevsick */         else if (cmax == g) h = fmod((60 * ((b - r) / diff) + 120), 360.0);
/* dit nhau by lovevsick */         else if (cmax == b) h = fmod((60 * ((r - g) / diff) + 240), 360.0);
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */     std::ostringstream oss;
/* dit nhau by lovevsick */     oss << "hsv(" << (int)std::round(h) << ", " << (int)std::round(s * 100) << "%, " << (int)std::round(v * 100) << "%)";
/* dit nhau by lovevsick */     return oss.str();
/* dit nhau by lovevsick */ }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */ static std::string format_time_utc(time_t t) {
/* dit nhau by lovevsick */     struct tm* tm_info = gmtime(&t);
/* dit nhau by lovevsick */     if (!tm_info) return "Invalid Time";
/* dit nhau by lovevsick */     char buf[64];
/* dit nhau by lovevsick */     strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S UTC", tm_info);
/* dit nhau by lovevsick */     return std::string(buf);
/* dit nhau by lovevsick */ }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */ static void detect_input(const std::string& raw, int& base, std::string& clean, std::string& detected_name) {
/* dit nhau by lovevsick */     clean = trim_spaces(raw);
/* dit nhau by lovevsick */     if (clean.empty()) {
/* dit nhau by lovevsick */         base = 10;
/* dit nhau by lovevsick */         detected_name = "Decimal";
/* dit nhau by lovevsick */         return;
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */     if (clean.size() >= 2 && (clean.substr(0, 2) == "0x" || clean.substr(0, 2) == "0X")) {
/* dit nhau by lovevsick */         base = 16;
/* dit nhau by lovevsick */         clean = clean.substr(2);
/* dit nhau by lovevsick */         detected_name = "Hexadecimal";
/* dit nhau by lovevsick */         return;
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */     if (clean.size() >= 2 && (clean.substr(0, 2) == "0b" || clean.substr(0, 2) == "0B")) {
/* dit nhau by lovevsick */         base = 2;
/* dit nhau by lovevsick */         clean = clean.substr(2);
/* dit nhau by lovevsick */         detected_name = "Binary";
/* dit nhau by lovevsick */         return;
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */     if (clean.size() >= 2 && (clean.substr(0, 2) == "0o" || clean.substr(0, 2) == "0O")) {
/* dit nhau by lovevsick */         base = 8;
/* dit nhau by lovevsick */         clean = clean.substr(2);
/* dit nhau by lovevsick */         detected_name = "Octal";
/* dit nhau by lovevsick */         return;
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */     if (clean[0] == '#') {
/* dit nhau by lovevsick */         base = 16;
/* dit nhau by lovevsick */         clean = clean.substr(1);
/* dit nhau by lovevsick */         detected_name = "Hex Color";
/* dit nhau by lovevsick */         return;
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */     char last = clean.back();
/* dit nhau by lovevsick */     if ((last == 'h' || last == 'H') && clean.size() > 1) {
/* dit nhau by lovevsick */         bool all_hex = true;
/* dit nhau by lovevsick */         for (size_t i = 0; i < clean.size() - 1; ++i) {
/* dit nhau by lovevsick */             if (!isxdigit((unsigned char)clean[i])) { all_hex = false; break; }
/* dit nhau by lovevsick */         }
/* dit nhau by lovevsick */         if (all_hex) {
/* dit nhau by lovevsick */             base = 16;
/* dit nhau by lovevsick */             clean = clean.substr(0, clean.size() - 1);
/* dit nhau by lovevsick */             detected_name = "Hexadecimal";
/* dit nhau by lovevsick */             return;
/* dit nhau by lovevsick */         }
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */     if ((last == 'b' || last == 'B') && clean.size() > 1) {
/* dit nhau by lovevsick */         bool all_bin = true;
/* dit nhau by lovevsick */         for (size_t i = 0; i < clean.size() - 1; ++i) {
/* dit nhau by lovevsick */             if (clean[i] != '0' && clean[i] != '1') { all_bin = false; break; }
/* dit nhau by lovevsick */         }
/* dit nhau by lovevsick */         if (all_bin) {
/* dit nhau by lovevsick */             base = 2;
/* dit nhau by lovevsick */             clean = clean.substr(0, clean.size() - 1);
/* dit nhau by lovevsick */             detected_name = "Binary";
/* dit nhau by lovevsick */             return;
/* dit nhau by lovevsick */         }
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */     if ((last == 'o' || last == 'O') && clean.size() > 1) {
/* dit nhau by lovevsick */         bool all_oct = true;
/* dit nhau by lovevsick */         for (size_t i = 0; i < clean.size() - 1; ++i) {
/* dit nhau by lovevsick */             if (clean[i] < '0' || clean[i] > '7') { all_oct = false; break; }
/* dit nhau by lovevsick */         }
/* dit nhau by lovevsick */         if (all_oct) {
/* dit nhau by lovevsick */             base = 8;
/* dit nhau by lovevsick */             clean = clean.substr(0, clean.size() - 1);
/* dit nhau by lovevsick */             detected_name = "Octal";
/* dit nhau by lovevsick */             return;
/* dit nhau by lovevsick */         }
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */     if ((last == 'd' || last == 'D') && clean.size() > 1) {
/* dit nhau by lovevsick */         bool all_dec = true;
/* dit nhau by lovevsick */         for (size_t i = 0; i < clean.size() - 1; ++i) {
/* dit nhau by lovevsick */             if (!isdigit((unsigned char)clean[i])) { all_dec = false; break; }
/* dit nhau by lovevsick */         }
/* dit nhau by lovevsick */         if (all_dec) {
/* dit nhau by lovevsick */             base = 10;
/* dit nhau by lovevsick */             clean = clean.substr(0, clean.size() - 1);
/* dit nhau by lovevsick */             detected_name = "Decimal";
/* dit nhau by lovevsick */             return;
/* dit nhau by lovevsick */         }
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */     bool has_hex_letter = false;
/* dit nhau by lovevsick */     bool all_hex = true;
/* dit nhau by lovevsick */     size_t start = (clean[0] == '-' || clean[0] == '+') ? 1 : 0;
/* dit nhau by lovevsick */     for (size_t i = start; i < clean.size(); ++i) {
/* dit nhau by lovevsick */         char c = clean[i];
/* dit nhau by lovevsick */         if (isxdigit((unsigned char)c)) {
/* dit nhau by lovevsick */             if ((c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')) has_hex_letter = true;
/* dit nhau by lovevsick */         } else {
/* dit nhau by lovevsick */             all_hex = false;
/* dit nhau by lovevsick */         }
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */     if (all_hex && has_hex_letter) {
/* dit nhau by lovevsick */         base = 16;
/* dit nhau by lovevsick */         detected_name = "Hexadecimal";
/* dit nhau by lovevsick */         return;
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */     base = 10;
/* dit nhau by lovevsick */     detected_name = "Decimal";
/* dit nhau by lovevsick */ }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */ static void print_help() {
/* dit nhau by lovevsick */     std::cout << "starhex - Ultra-fast versatile CLI number base, hex & encoding toolkit for Arch Linux\n";
/* dit nhau by lovevsick */     std::cout << "\n";
/* dit nhau by lovevsick */     std::cout << "USAGE:\n";
/* dit nhau by lovevsick */     std::cout << "    starhex <INPUT> [OPTIONS]\n";
/* dit nhau by lovevsick */     std::cout << "    starhex [OPTIONS] <INPUT>\n";
/* dit nhau by lovevsick */     std::cout << "    echo <INPUT> | starhex [OPTIONS]\n";
/* dit nhau by lovevsick */     std::cout << "\n";
/* dit nhau by lovevsick */     std::cout << "CONVERSION TARGETS (-t<target>):\n";
/* dit nhau by lovevsick */     std::cout << "    -t2,  -tb,  -tbin       Convert to Binary (base 2)\n";
/* dit nhau by lovevsick */     std::cout << "    -t8,  -to,  -toct       Convert to Octal (base 8)\n";
/* dit nhau by lovevsick */     std::cout << "    -t10, -td,  -tdec       Convert to Decimal (base 10)\n";
/* dit nhau by lovevsick */     std::cout << "    -t16, -tx,  -th, -thex  Convert to Hexadecimal (base 16)\n";
/* dit nhau by lovevsick */     std::cout << "    -t<N>                   Convert to arbitrary base (2 <= N <= 62)\n";
/* dit nhau by lovevsick */     std::cout << "    -ts,  -tstr, -ttext     Convert hex/bytes to ASCII string\n";
/* dit nhau by lovevsick */     std::cout << "    -tb64                   Convert to Base64\n";
/* dit nhau by lovevsick */     std::cout << "    -trgb                   Convert hex number to rgb(r, g, b)\n";
/* dit nhau by lovevsick */     std::cout << "    -thsv                   Convert hex number to hsv(h, s%, v%)\n";
/* dit nhau by lovevsick */     std::cout << "    -tip                    Convert number to IPv4 address\n";
/* dit nhau by lovevsick */     std::cout << "    -tfloat                 Convert hex to 32-bit float (IEEE 754)\n";
/* dit nhau by lovevsick */     std::cout << "    -tdouble                Convert hex to 64-bit double (IEEE 754)\n";
/* dit nhau by lovevsick */     std::cout << "    -ttime                  Convert unix timestamp to human UTC date\n";
/* dit nhau by lovevsick */     std::cout << "\n";
/* dit nhau by lovevsick */     std::cout << "INPUT DETECTION & OVERRIDE (-f<from>):\n";
/* dit nhau by lovevsick */     std::cout << "    Input base is automatically detected:\n";
/* dit nhau by lovevsick */     std::cout << "      0x... or ...h or A-F letters  -> Hexadecimal (base 16)\n";
/* dit nhau by lovevsick */     std::cout << "      0b... or ...b                 -> Binary (base 2)\n";
/* dit nhau by lovevsick */     std::cout << "      0o... or ...o                 -> Octal (base 8)\n";
/* dit nhau by lovevsick */     std::cout << "      #RRGGBB                       -> Hex Color\n";
/* dit nhau by lovevsick */     std::cout << "      rgb(r, g, b)                  -> RGB Color\n";
/* dit nhau by lovevsick */     std::cout << "      A.B.C.D                       -> IPv4 Address\n";
/* dit nhau by lovevsick */     std::cout << "      Digits 0-9                    -> Decimal (base 10)\n";
/* dit nhau by lovevsick */     std::cout << "    Force input base: -f2, -f8, -f10, -f16, -fstr, -fb64, -fip, -frgb\n";
/* dit nhau by lovevsick */     std::cout << "\n";
/* dit nhau by lovevsick */     std::cout << "FORMATTING OPTIONS:\n";
/* dit nhau by lovevsick */     std::cout << "    -p,  --prefix           Add base prefix (0x, 0b, 0o, #)\n";
/* dit nhau by lovevsick */     std::cout << "    -np, --no-prefix        Omit prefix (default for -t)\n";
/* dit nhau by lovevsick */     std::cout << "    -u,  --upper            Output uppercase digits (A-F)\n";
/* dit nhau by lovevsick */     std::cout << "    -l,  --lower            Output lowercase digits (a-f)\n";
/* dit nhau by lovevsick */     std::cout << "    -w<N>, --width <N>      Zero-pad output to N digits / bit width\n";
/* dit nhau by lovevsick */     std::cout << "    -g<N>, --group <N>      Group digits by N characters with spaces\n";
/* dit nhau by lovevsick */     std::cout << "    -q,  --quiet            Raw output only (no extra newlines/headers)\n";
/* dit nhau by lovevsick */     std::cout << "    -a,  --all              Display complete multi-base inspection table\n";
/* dit nhau by lovevsick */     std::cout << "    -c,  --color            Force colored output\n";
/* dit nhau by lovevsick */     std::cout << "    -nc, --no-color         Disable colored output\n";
/* dit nhau by lovevsick */     std::cout << "\n";
/* dit nhau by lovevsick */     std::cout << "DATA & BITWISE OPERATIONS:\n";
/* dit nhau by lovevsick */     std::cout << "    -e16, -e32, -e64        Endianness byte swap (16, 32, or 64-bit)\n";
/* dit nhau by lovevsick */     std::cout << "    --not                   Bitwise NOT (invert bits)\n";
/* dit nhau by lovevsick */     std::cout << "    --and <VAL>             Bitwise AND with VAL\n";
/* dit nhau by lovevsick */     std::cout << "    --or  <VAL>             Bitwise OR with VAL\n";
/* dit nhau by lovevsick */     std::cout << "    --xor <VAL>             Bitwise XOR with VAL\n";
/* dit nhau by lovevsick */     std::cout << "    --shl <N>               Shift left by N bits\n";
/* dit nhau by lovevsick */     std::cout << "    --shr <N>               Shift right by N bits\n";
/* dit nhau by lovevsick */     std::cout << "\n";
/* dit nhau by lovevsick */     std::cout << "EXAMPLES:\n";
/* dit nhau by lovevsick */     std::cout << "    starhex 42 -t2                 -> 101010\n";
/* dit nhau by lovevsick */     std::cout << "    starhex 42 -t2 -p              -> 0b101010\n";
/* dit nhau by lovevsick */     std::cout << "    starhex 42 -t2 -w8 -g4         -> 0010 1010\n";
/* dit nhau by lovevsick */     std::cout << "    starhex 0x3D -t10              -> 61\n";
/* dit nhau by lovevsick */     std::cout << "    starhex 0b101010 -t16          -> 2a\n";
/* dit nhau by lovevsick */     std::cout << "    starhex 0xdeadbeef -t10        -> 3735928559\n";
/* dit nhau by lovevsick */     std::cout << "    starhex \"#FF5733\" -trgb        -> rgb(255, 87, 51)\n";
/* dit nhau by lovevsick */     std::cout << "    starhex \"rgb(255, 87, 51)\" -tx -> FF5733\n";
/* dit nhau by lovevsick */     std::cout << "    starhex 192.168.1.1 -tx        -> C0A80101\n";
/* dit nhau by lovevsick */     std::cout << "    starhex 0xC0A80101 -tip        -> 192.168.1.1\n";
/* dit nhau by lovevsick */     std::cout << "    starhex \"Hello World\" -thex    -> 48656c6c6f20576f726c64\n";
/* dit nhau by lovevsick */     std::cout << "    starhex 0x3D                   -> Full multi-base inspection dashboard\n";
/* dit nhau by lovevsick */ }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */ static void parse_cli_args(int argc, char* argv[], Config& cfg) {
/* dit nhau by lovevsick */     for (int i = 1; i < argc; ++i) {
/* dit nhau by lovevsick */         std::string arg = argv[i];
/* dit nhau by lovevsick */         if (arg == "-h" || arg == "--help") {
/* dit nhau by lovevsick */             print_help();
/* dit nhau by lovevsick */             exit(0);
/* dit nhau by lovevsick */         }
/* dit nhau by lovevsick */         if (arg == "-v" || arg == "--version") {
/* dit nhau by lovevsick */             std::cout << "starhex 1.0.0 (Arch Linux x86_64)" << std::endl;
/* dit nhau by lovevsick */             exit(0);
/* dit nhau by lovevsick */         }
/* dit nhau by lovevsick */         if (arg == "-p" || arg == "--prefix") { cfg.prefix = true; cfg.no_prefix = false; continue; }
/* dit nhau by lovevsick */         if (arg == "-np" || arg == "--no-prefix") { cfg.no_prefix = true; cfg.prefix = false; continue; }
/* dit nhau by lovevsick */         if (arg == "-u" || arg == "--upper") { cfg.uppercase = true; continue; }
/* dit nhau by lovevsick */         if (arg == "-l" || arg == "--lower") { cfg.lowercase = true; continue; }
/* dit nhau by lovevsick */         if (arg == "-q" || arg == "--quiet") { cfg.quiet = true; continue; }
/* dit nhau by lovevsick */         if (arg == "-a" || arg == "--all") { cfg.force_all = true; continue; }
/* dit nhau by lovevsick */         if (arg == "-c" || arg == "--color") { cfg.use_color = true; continue; }
/* dit nhau by lovevsick */         if (arg == "-nc" || arg == "--no-color") { cfg.use_color = false; continue; }
/* dit nhau by lovevsick */         if (arg == "--not") { cfg.op_not = true; continue; }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */         if (arg == "-e16") { cfg.endian_swap = 16; continue; }
/* dit nhau by lovevsick */         if (arg == "-e32") { cfg.endian_swap = 32; continue; }
/* dit nhau by lovevsick */         if (arg == "-e64") { cfg.endian_swap = 64; continue; }
/* dit nhau by lovevsick */         if (arg == "--endian" && i + 1 < argc) { cfg.endian_swap = std::atoi(argv[++i]); continue; }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */         if (arg == "--and" && i + 1 < argc) { cfg.op_and = argv[++i]; continue; }
/* dit nhau by lovevsick */         if (arg == "--or" && i + 1 < argc) { cfg.op_or = argv[++i]; continue; }
/* dit nhau by lovevsick */         if (arg == "--xor" && i + 1 < argc) { cfg.op_xor = argv[++i]; continue; }
/* dit nhau by lovevsick */         if (arg == "--shl" && i + 1 < argc) { cfg.op_shl = std::atoi(argv[++i]); continue; }
/* dit nhau by lovevsick */         if (arg == "--shr" && i + 1 < argc) { cfg.op_shr = std::atoi(argv[++i]); continue; }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */         if (arg.rfind("-w", 0) == 0 && arg.size() > 2) {
/* dit nhau by lovevsick */             cfg.width = std::atoi(arg.substr(2).c_str());
/* dit nhau by lovevsick */             continue;
/* dit nhau by lovevsick */         }
/* dit nhau by lovevsick */         if ((arg == "-w" || arg == "--width") && i + 1 < argc) {
/* dit nhau by lovevsick */             cfg.width = std::atoi(argv[++i]);
/* dit nhau by lovevsick */             continue;
/* dit nhau by lovevsick */         }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */         if (arg.rfind("-g", 0) == 0 && arg.size() > 2) {
/* dit nhau by lovevsick */             cfg.group = std::atoi(arg.substr(2).c_str());
/* dit nhau by lovevsick */             continue;
/* dit nhau by lovevsick */         }
/* dit nhau by lovevsick */         if ((arg == "-g" || arg == "--group") && i + 1 < argc) {
/* dit nhau by lovevsick */             cfg.group = std::atoi(argv[++i]);
/* dit nhau by lovevsick */             continue;
/* dit nhau by lovevsick */         }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */         if (arg.rfind("-t", 0) == 0) {
/* dit nhau by lovevsick */             std::string tval = arg.substr(2);
/* dit nhau by lovevsick */             if (tval.empty() && i + 1 < argc) tval = argv[++i];
/* dit nhau by lovevsick */             if (tval == "2" || tval == "b" || tval == "bin") cfg.target_base = 2;
/* dit nhau by lovevsick */             else if (tval == "8" || tval == "o" || tval == "oct") cfg.target_base = 8;
/* dit nhau by lovevsick */             else if (tval == "10" || tval == "d" || tval == "dec") cfg.target_base = 10;
/* dit nhau by lovevsick */             else if (tval == "16" || tval == "x" || tval == "h" || tval == "hex") cfg.target_base = 16;
/* dit nhau by lovevsick */             else if (tval == "s" || tval == "str" || tval == "text") cfg.target_type = "str";
/* dit nhau by lovevsick */             else if (tval == "b64" || tval == "base64") cfg.target_type = "b64";
/* dit nhau by lovevsick */             else if (tval == "rgb") cfg.target_type = "rgb";
/* dit nhau by lovevsick */             else if (tval == "hsv") cfg.target_type = "hsv";
/* dit nhau by lovevsick */             else if (tval == "ip" || tval == "ipv4") cfg.target_type = "ip";
/* dit nhau by lovevsick */             else if (tval == "float") cfg.target_type = "float";
/* dit nhau by lovevsick */             else if (tval == "double") cfg.target_type = "double";
/* dit nhau by lovevsick */             else if (tval == "time") cfg.target_type = "time";
/* dit nhau by lovevsick */             else cfg.target_base = std::atoi(tval.c_str());
/* dit nhau by lovevsick */             continue;
/* dit nhau by lovevsick */         }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */         if (arg.rfind("-f", 0) == 0) {
/* dit nhau by lovevsick */             std::string fval = arg.substr(2);
/* dit nhau by lovevsick */             if (fval.empty() && i + 1 < argc) fval = argv[++i];
/* dit nhau by lovevsick */             if (fval == "2" || fval == "b" || fval == "bin") cfg.force_base = 2;
/* dit nhau by lovevsick */             else if (fval == "8" || fval == "o" || fval == "oct") cfg.force_base = 8;
/* dit nhau by lovevsick */             else if (fval == "10" || fval == "d" || fval == "dec") cfg.force_base = 10;
/* dit nhau by lovevsick */             else if (fval == "16" || fval == "x" || fval == "h" || fval == "hex") cfg.force_base = 16;
/* dit nhau by lovevsick */             else if (fval == "s" || fval == "str") cfg.force_type = "str";
/* dit nhau by lovevsick */             else if (fval == "b64") cfg.force_type = "b64";
/* dit nhau by lovevsick */             else if (fval == "ip") cfg.force_type = "ip";
/* dit nhau by lovevsick */             else if (fval == "rgb") cfg.force_type = "rgb";
/* dit nhau by lovevsick */             else if (fval == "float") cfg.force_type = "float";
/* dit nhau by lovevsick */             else if (fval == "double") cfg.force_type = "double";
/* dit nhau by lovevsick */             else cfg.force_base = std::atoi(fval.c_str());
/* dit nhau by lovevsick */             continue;
/* dit nhau by lovevsick */         }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */         if (arg[0] != '-' || arg == "-") {
/* dit nhau by lovevsick */             if (cfg.input.empty()) cfg.input = arg;
/* dit nhau by lovevsick */             else cfg.input += " " + arg;
/* dit nhau by lovevsick */         }
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */ }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */ static void print_inspector_dashboard(const std::string& raw_input, const std::string& detected_name, const BigInt& bi, bool use_color) {
/* dit nhau by lovevsick */     const std::string c_cyan = use_color ? "\033[1;36m" : "";
/* dit nhau by lovevsick */     const std::string c_green = use_color ? "\033[1;32m" : "";
/* dit nhau by lovevsick */     const std::string c_yellow = use_color ? "\033[1;33m" : "";
/* dit nhau by lovevsick */     const std::string c_magenta = use_color ? "\033[1;35m" : "";
/* dit nhau by lovevsick */     const std::string c_blue = use_color ? "\033[1;34m" : "";
/* dit nhau by lovevsick */     const std::string c_reset = use_color ? "\033[0m" : "";
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */     std::string dec_val = bi.to_string(10);
/* dit nhau by lovevsick */     std::string hex_val_upper = bi.to_string(16, true);
/* dit nhau by lovevsick */     std::string hex_val_lower = bi.to_string(16, false);
/* dit nhau by lovevsick */     std::string oct_val = bi.to_string(8);
/* dit nhau by lovevsick */     std::string bin_val = bi.to_string(2);
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */     int bit_len = bi.bit_length();
/* dit nhau by lovevsick */     int byte_width = std::max(1, (bit_len + 7) / 8);
/* dit nhau by lovevsick */     int padded_bin_width = byte_width * 8;
/* dit nhau by lovevsick */     std::string bin_padded = pad_string(bin_val, padded_bin_width);
/* dit nhau by lovevsick */     std::string bin_grouped = group_string(bin_padded, 4);
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */     uint64_t u64 = bi.to_u64();
/* dit nhau by lovevsick */     char ascii_char = (u64 >= 32 && u64 <= 126) ? (char)u64 : '.';
/* dit nhau by lovevsick */     std::string ascii_desc;
/* dit nhau by lovevsick */     if (u64 >= 32 && u64 <= 126) ascii_desc = std::string("'") + ascii_char + "'";
/* dit nhau by lovevsick */     else ascii_desc = "[Non-printable]";
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */     std::cout << c_cyan << "┌────────────────────────────────────────────────────────────────────────┐" << c_reset << "\n";
/* dit nhau by lovevsick */     std::cout << c_cyan << "│  STARHEX - Arch Linux Multi-Base Inspector                             │" << c_reset << "\n";
/* dit nhau by lovevsick */     std::cout << c_cyan << "├───────────────────────┬────────────────────────────────────────────────┤" << c_reset << "\n";
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */     auto print_row = [&](const std::string& key, const std::string& val) {
/* dit nhau by lovevsick */         std::cout << c_cyan << "│ " << c_yellow << std::left << std::setw(21) << key << c_cyan << " │ " << c_reset
/* dit nhau by lovevsick */                   << std::left << std::setw(46) << val << c_cyan << " │" << c_reset << "\n";
/* dit nhau by lovevsick */     };
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */     print_row("Input", raw_input + " (" + detected_name + ")");
/* dit nhau by lovevsick */     print_row("Decimal (base 10)", dec_val);
/* dit nhau by lovevsick */     print_row("Hexadecimal (base 16)", "0x" + hex_val_upper + " (" + hex_val_lower + ")");
/* dit nhau by lovevsick */     print_row("Octal (base 8)", "0o" + oct_val);
/* dit nhau by lovevsick */     print_row("Binary (base 2)", "0b" + bin_grouped + " (" + std::to_string(bit_len) + "-bit)");
/* dit nhau by lovevsick */     print_row("ASCII Char", ascii_desc);
/* dit nhau by lovevsick */     print_row("Bit Length", std::to_string(bit_len) + " bits (" + std::to_string(byte_width) + " bytes)");
/* dit nhau by lovevsick */     print_row("Active Bits (Popcnt)", std::to_string(bi.popcount()));
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */     if (bit_len <= 16) {
/* dit nhau by lovevsick */         BigInt sw16 = bi.swap_endian(16);
/* dit nhau by lovevsick */         print_row("Endian Swap 16-bit", "0x" + pad_string(sw16.to_string(16, true), 4) + " (dec: " + sw16.to_string(10) + ")");
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */     if (bit_len <= 32) {
/* dit nhau by lovevsick */         BigInt sw32 = bi.swap_endian(32);
/* dit nhau by lovevsick */         print_row("Endian Swap 32-bit", "0x" + pad_string(sw32.to_string(16, true), 8) + " (dec: " + sw32.to_string(10) + ")");
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */     if (bit_len <= 64) {
/* dit nhau by lovevsick */         BigInt sw64 = bi.swap_endian(64);
/* dit nhau by lovevsick */         print_row("Endian Swap 64-bit", "0x" + pad_string(sw64.to_string(16, true), 16) + " (dec: " + sw64.to_string(10) + ")");
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */     if (bit_len <= 24 && !bi.is_zero()) {
/* dit nhau by lovevsick */         uint32_t rgb = (uint32_t)(u64 & 0xFFFFFF);
/* dit nhau by lovevsick */         int r = (rgb >> 16) & 0xFF;
/* dit nhau by lovevsick */         int g = (rgb >> 8) & 0xFF;
/* dit nhau by lovevsick */         int b = rgb & 0xFF;
/* dit nhau by lovevsick */         std::string swatch = use_color ? ("\033[48;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m   \033[0m ") : "";
/* dit nhau by lovevsick */         print_row("Color (RGB / HSV)", swatch + format_rgb(rgb) + " " + format_hsv(rgb));
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */     if (bit_len <= 32) {
/* dit nhau by lovevsick */         print_row("IPv4 Address", format_ipv4((uint32_t)u64));
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */     if (u64 >= 1000000000ULL && u64 <= 2500000000ULL) {
/* dit nhau by lovevsick */         print_row("Unix Timestamp", format_time_utc((time_t)u64));
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */     std::cout << c_cyan << "└───────────────────────┴────────────────────────────────────────────────┘" << c_reset << "\n";
/* dit nhau by lovevsick */ }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */ int main(int argc, char* argv[]) {
/* dit nhau by lovevsick */     Config cfg;
/* dit nhau by lovevsick */     cfg.use_color = IS_TTY();
/* dit nhau by lovevsick */     parse_cli_args(argc, argv, cfg);
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */     if (cfg.input.empty() || cfg.input == "-") {
/* dit nhau by lovevsick */         std::string line;
/* dit nhau by lovevsick */         while (std::getline(std::cin, line)) {
/* dit nhau by lovevsick */             line = trim_spaces(line);
/* dit nhau by lovevsick */             if (line.empty()) continue;
/* dit nhau by lovevsick */             std::vector<char*> fake_argv;
/* dit nhau by lovevsick */             fake_argv.push_back(argv[0]);
/* dit nhau by lovevsick */             for (int i = 1; i < argc; ++i) {
/* dit nhau by lovevsick */                 if (std::string(argv[i]) != "-" && std::string(argv[i]) != cfg.input) {
/* dit nhau by lovevsick */                     fake_argv.push_back(argv[i]);
/* dit nhau by lovevsick */                 }
/* dit nhau by lovevsick */             }
/* dit nhau by lovevsick */             std::string line_copy = line;
/* dit nhau by lovevsick */             fake_argv.push_back(&line_copy[0]);
/* dit nhau by lovevsick */             Config sub_cfg;
/* dit nhau by lovevsick */             sub_cfg.use_color = false;
/* dit nhau by lovevsick */             sub_cfg.quiet = true;
/* dit nhau by lovevsick */             parse_cli_args((int)fake_argv.size(), fake_argv.data(), sub_cfg);
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */             int base = 10;
/* dit nhau by lovevsick */             std::string clean, detected_name;
/* dit nhau by lovevsick */             detect_input(sub_cfg.input, base, clean, detected_name);
/* dit nhau by lovevsick */             BigInt bi = BigInt::from_string(clean, base);
/* dit nhau by lovevsick */             int target = sub_cfg.target_base > 0 ? sub_cfg.target_base : 10;
/* dit nhau by lovevsick */             std::cout << bi.to_string(target, sub_cfg.uppercase) << "\n";
/* dit nhau by lovevsick */         }
/* dit nhau by lovevsick */         return 0;
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */     std::string raw_input = cfg.input;
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */     if (cfg.force_type == "str" || cfg.target_type == "str" ||
/* dit nhau by lovevsick */         ((cfg.target_base == 16 || cfg.target_type == "hex" || cfg.target_type == "b64") && is_plain_text(raw_input))) {
/* dit nhau by lovevsick */         if (cfg.target_type == "str") {
/* dit nhau by lovevsick */             std::string s = hex_to_text(raw_input);
/* dit nhau by lovevsick */             std::cout << s << (cfg.quiet ? "" : "\n");
/* dit nhau by lovevsick */             return 0;
/* dit nhau by lovevsick */         } else if (cfg.target_type == "b64") {
/* dit nhau by lovevsick */             std::cout << base64_encode(raw_input) << "\n";
/* dit nhau by lovevsick */             return 0;
/* dit nhau by lovevsick */         } else {
/* dit nhau by lovevsick */             std::string h = text_to_hex(raw_input, cfg.uppercase);
/* dit nhau by lovevsick */             std::cout << h << (cfg.quiet ? "" : "\n");
/* dit nhau by lovevsick */             return 0;
/* dit nhau by lovevsick */         }
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */     if (cfg.force_type == "b64" || cfg.target_type == "b64") {
/* dit nhau by lovevsick */         if (cfg.target_type == "b64") {
/* dit nhau by lovevsick */             std::cout << base64_encode(raw_input) << "\n";
/* dit nhau by lovevsick */             return 0;
/* dit nhau by lovevsick */         } else {
/* dit nhau by lovevsick */             std::cout << base64_decode(raw_input) << "\n";
/* dit nhau by lovevsick */             return 0;
/* dit nhau by lovevsick */         }
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */     uint32_t ip_val = 0;
/* dit nhau by lovevsick */     if (cfg.force_type == "ip" || parse_ipv4(raw_input, ip_val)) {
/* dit nhau by lovevsick */         if (cfg.target_type == "ip") {
/* dit nhau by lovevsick */             std::cout << format_ipv4(ip_val) << "\n";
/* dit nhau by lovevsick */             return 0;
/* dit nhau by lovevsick */         }
/* dit nhau by lovevsick */         BigInt bi((uint64_t)ip_val);
/* dit nhau by lovevsick */         int tbase = cfg.target_base > 0 ? cfg.target_base : 16;
/* dit nhau by lovevsick */         std::string res = bi.to_string(tbase, cfg.uppercase);
/* dit nhau by lovevsick */         if (cfg.prefix) {
/* dit nhau by lovevsick */             if (tbase == 16) res = "0x" + res;
/* dit nhau by lovevsick */             else if (tbase == 2) res = "0b" + res;
/* dit nhau by lovevsick */         }
/* dit nhau by lovevsick */         std::cout << res << "\n";
/* dit nhau by lovevsick */         return 0;
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */     uint32_t rgb_val = 0;
/* dit nhau by lovevsick */     if (cfg.force_type == "rgb" || parse_rgb_string(raw_input, rgb_val)) {
/* dit nhau by lovevsick */         if (cfg.target_base == 16 || cfg.target_type == "hex" || cfg.target_base == 0) {
/* dit nhau by lovevsick */             std::ostringstream oss;
/* dit nhau by lovevsick */             oss << std::hex << std::setw(6) << std::setfill('0');
/* dit nhau by lovevsick */             if (cfg.uppercase) oss << std::uppercase;
/* dit nhau by lovevsick */             oss << rgb_val;
/* dit nhau by lovevsick */             std::string res = oss.str();
/* dit nhau by lovevsick */             if (cfg.prefix) res = "#" + res;
/* dit nhau by lovevsick */             std::cout << res << "\n";
/* dit nhau by lovevsick */             return 0;
/* dit nhau by lovevsick */         }
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */     if (cfg.target_type == "float") {
/* dit nhau by lovevsick */         float f;
/* dit nhau by lovevsick */         if (raw_input.find('.') != std::string::npos) {
/* dit nhau by lovevsick */             f = std::stof(raw_input);
/* dit nhau by lovevsick */             uint32_t u;
/* dit nhau by lovevsick */             std::memcpy(&u, &f, sizeof(float));
/* dit nhau by lovevsick */             std::ostringstream oss;
/* dit nhau by lovevsick */             oss << "0x" << std::hex << std::setw(8) << std::setfill('0') << u;
/* dit nhau by lovevsick */             std::cout << oss.str() << "\n";
/* dit nhau by lovevsick */         } else {
/* dit nhau by lovevsick */             uint32_t u = (uint32_t)std::strtoul(raw_input.c_str(), nullptr, 16);
/* dit nhau by lovevsick */             std::memcpy(&f, &u, sizeof(float));
/* dit nhau by lovevsick */             std::cout << f << "\n";
/* dit nhau by lovevsick */         }
/* dit nhau by lovevsick */         return 0;
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */     if (cfg.target_type == "double") {
/* dit nhau by lovevsick */         double d;
/* dit nhau by lovevsick */         if (raw_input.find('.') != std::string::npos) {
/* dit nhau by lovevsick */             d = std::stod(raw_input);
/* dit nhau by lovevsick */             uint64_t u;
/* dit nhau by lovevsick */             std::memcpy(&u, &d, sizeof(double));
/* dit nhau by lovevsick */             std::ostringstream oss;
/* dit nhau by lovevsick */             oss << "0x" << std::hex << std::setw(16) << std::setfill('0') << u;
/* dit nhau by lovevsick */             std::cout << oss.str() << "\n";
/* dit nhau by lovevsick */         } else {
/* dit nhau by lovevsick */             uint64_t u = (uint64_t)std::strtoull(raw_input.c_str(), nullptr, 16);
/* dit nhau by lovevsick */             std::memcpy(&d, &u, sizeof(double));
/* dit nhau by lovevsick */             std::cout << d << "\n";
/* dit nhau by lovevsick */         }
/* dit nhau by lovevsick */         return 0;
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */     int in_base = 10;
/* dit nhau by lovevsick */     std::string clean_input, detected_name;
/* dit nhau by lovevsick */     detect_input(raw_input, in_base, clean_input, detected_name);
/* dit nhau by lovevsick */     if (cfg.force_base > 0) in_base = cfg.force_base;
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */     BigInt val = BigInt::from_string(clean_input, in_base);
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */     if (cfg.endian_swap > 0) val = val.swap_endian(cfg.endian_swap);
/* dit nhau by lovevsick */     if (cfg.op_not) val = val.bitwise_not(cfg.width);
/* dit nhau by lovevsick */     if (!cfg.op_and.empty()) {
/* dit nhau by lovevsick */         int ob = 10; std::string oc, on; detect_input(cfg.op_and, ob, oc, on);
/* dit nhau by lovevsick */         val = val.bitwise_and(BigInt::from_string(oc, ob));
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */     if (!cfg.op_or.empty()) {
/* dit nhau by lovevsick */         int ob = 10; std::string oc, on; detect_input(cfg.op_or, ob, oc, on);
/* dit nhau by lovevsick */         val = val.bitwise_or(BigInt::from_string(oc, ob));
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */     if (!cfg.op_xor.empty()) {
/* dit nhau by lovevsick */         int ob = 10; std::string oc, on; detect_input(cfg.op_xor, ob, oc, on);
/* dit nhau by lovevsick */         val = val.bitwise_xor(BigInt::from_string(oc, ob));
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */     if (cfg.op_shl > 0) val = val.shift_left(cfg.op_shl);
/* dit nhau by lovevsick */     if (cfg.op_shr > 0) val = val.shift_right(cfg.op_shr);
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */     if (cfg.target_type == "rgb") {
/* dit nhau by lovevsick */         uint32_t rgb = (uint32_t)(val.to_u64() & 0xFFFFFF);
/* dit nhau by lovevsick */         std::cout << format_rgb(rgb) << "\n";
/* dit nhau by lovevsick */         return 0;
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */     if (cfg.target_type == "hsv") {
/* dit nhau by lovevsick */         uint32_t rgb = (uint32_t)(val.to_u64() & 0xFFFFFF);
/* dit nhau by lovevsick */         std::cout << format_hsv(rgb) << "\n";
/* dit nhau by lovevsick */         return 0;
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */     if (cfg.target_type == "ip") {
/* dit nhau by lovevsick */         std::cout << format_ipv4((uint32_t)val.to_u64()) << "\n";
/* dit nhau by lovevsick */         return 0;
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */     if (cfg.target_type == "time") {
/* dit nhau by lovevsick */         std::cout << format_time_utc((time_t)val.to_u64()) << "\n";
/* dit nhau by lovevsick */         return 0;
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */     if (cfg.target_base == 0 && !cfg.force_all && !cfg.quiet && cfg.target_type.empty()) {
/* dit nhau by lovevsick */         print_inspector_dashboard(raw_input, detected_name, val, cfg.use_color);
/* dit nhau by lovevsick */         return 0;
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */     int out_base = (cfg.target_base > 0) ? cfg.target_base : 10;
/* dit nhau by lovevsick */     bool is_upper = cfg.uppercase;
/* dit nhau by lovevsick */     std::string out_str = val.to_string(out_base, is_upper);
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */     if (cfg.width > 0) out_str = pad_string(out_str, cfg.width);
/* dit nhau by lovevsick */     if (cfg.group > 0) out_str = group_string(out_str, cfg.group);
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */     if (cfg.prefix) {
/* dit nhau by lovevsick */         if (out_base == 16) out_str = (is_upper ? "0X" : "0x") + out_str;
/* dit nhau by lovevsick */         else if (out_base == 2) out_str = (is_upper ? "0B" : "0b") + out_str;
/* dit nhau by lovevsick */         else if (out_base == 8) out_str = (is_upper ? "0O" : "0o") + out_str;
/* dit nhau by lovevsick */     }
/* dit nhau by lovevsick */
/* dit nhau by lovevsick */     std::cout << out_str << "\n";
/* dit nhau by lovevsick */     return 0;
/* dit nhau by lovevsick */ }
