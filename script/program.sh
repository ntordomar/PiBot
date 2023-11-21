ROOT="$(dirname "$0")"


cd "../bin"
make clean
make all
cd ../script
./start.sh program