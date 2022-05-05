mkdir build 
cd build/
cmake .. -DCMAKE_PREFIX_PATH=~/Qt/5.15.2/gcc_64
make
ls
cqtdeployer -bin CryptoSignDesktop
mkdir program
mkdir program/CryptoSignDesktop
cp -R DistributionKit/* program/CryptoSignDesktop
cd ..
cp -R libs/application_jar/* build/program/CryptoSignDesktop
echo Готово!
