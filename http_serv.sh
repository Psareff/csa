mkdir -p ./deb_http_server
mkdir -p ./deb_http_server/usr/local/bin
mkdir -p ./deb_http_server/DEBIAN
mkdir -p ./deb_http_server/etc/systemd/system
touch  ./deb_http_server/DEBIAN/control

cat << EOF > ./deb_http_server/DEBIAN/control
Package: HTTPServer
Version: 0.1Pre-Alpha
Architecture: amd64
Maintainer: Psareff
EOF

cp ./build/HTTPServer ./deb_http_server/usr/local/bin/

cp ./HTTPServer.service ./deb_http_server/etc/systemd/system/

cp ./postinst ./deb_http_server/DEBIAN/
cp ./prerm ./deb_http_server/DEBIAN/

chmod +x ./deb_http_server/DEBIAN/postinst ./deb_http_server/DEBIAN/prerm

dpkg-deb --build ./deb_http_server/ HTTPServer.deb
sudo dpkg -i HTTPServer.deb
