cd ca_sign_build
ca_sign_build.exe algo=ecdsa ca=../%1.bin sca=../%1.sbin load_address=10000000 jump_address=10000020 arguments= version=01010003 application_version=02030001 verbose=no
cd ..
rm -rf %2
bash build_application.sh %1.sbin %2 ../keys/maximtestcrk.key