mkdir -p ./deb_http_server
mkdir -p ./deb_http_server/bin
mkdir -p ./deb_http_server/DEBIAN
touch  ./deb_http_server/DEBIAN/control

cat << EOF > ./deb_http_server/DEBIAN/control
Package: http-server-dcsa
Version: 0.1Pre-Alpha
Architecture: amd64
Maintainer: Psareff
EOF

cp ./build/HTTPServer ./deb_http_server/bin/

dpkg-deb --build ./deb_http_server/ HTTPServer.deb
