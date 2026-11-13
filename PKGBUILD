pkgname=starhex
pkgver=1.0.0
pkgrel=1
pkgdesc="Ultra-fast versatile CLI number base, hex & encoding toolkit for Arch Linux"
arch=('x86_64' 'aarch64')
url="https://github.com/lovevsick/starhex"
license=('MIT')
depends=('glibc' 'gcc-libs')
makedepends=('cmake' 'make' 'gcc')
source=()

build() {
    cd "$startdir"
    cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr
    cmake --build build -j$(nproc 2>/dev/null || echo 2)
}

package() {
    cd "$startdir"
    DESTDIR="$pkgdir" cmake --install build
}
