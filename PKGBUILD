# Maintainer: Shauna <Shauna.Recto@gmail.com>

_pkgname=audioDebugger
pkgname=audio-debugger
pkgver=0.01
pkgrel=1
pkgdesc="Audio Debugger tool developed for use in RoboCup"
url="https://github.com/Pyanyaleri/audioDebugger"
arch=('x86_64')
license=('GPL-3.0-or-later')
depends=(alsa-lib fftw fmt glfw glib2 libportal lua protobuf)
makedepends=(base-devel git)
provides=("${pkgname%-git}")
source=(https://github.com/Pyanyaleri/audioDebugger/archive/refs/heads/main.zip)
sha256sums=('SKIP')

otherPkgname=audioDebugger

build() {
	cd "$srcdir/$_pkgname-main"
	mkdir -p build
	make
}

package() {
	cd "$srcdir/$_pkgname-main"
	install -dm755 "$pkgdir/opt/$_pkgname"
	install -Dm755 "build/$_pkgname" "$pkgdir/opt/$_pkgname/$_pkgname"
	cp -r assets/ "$pkgdir/opt/$_pkgname/assets/"
	install -Dm755 settings.lua "$pkgdir/opt/$_pkgname/settings.lua"
	install -Dm644 assets/${pkgname}.desktop "${pkgdir}/usr/share/applications/${pkgname}.desktop"
}