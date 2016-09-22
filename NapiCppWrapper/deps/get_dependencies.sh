if [ ! -d json ]
then
echo "Cloning nlohmann json library"
git clone https://github.com/nlohmann/json.git --branch master
else
echo "updating nlohmann json library"
cd json
git pull
cd ../
fi


if ! [ "$1" == "dont-build-napi" ]
then

    cd ../../../build-ninja
    git clean -xfd

    if [ `uname` == "Darwin" ]
    then
    echo "Building napi for mac"
    make genrelstatic
    make allstatic
    cp libnapi-net.a ../wrappers/cpp/deps
    cp libnapi.a ../wrappers/cpp/deps
    else
    echo "Building napi for win"
    cmake -G "Visual Studio 14 Win64" -DBUILD_SHARED_LIBS=OFF ..
    cmake --build .
    cp Debug/napi-net.lib ../wrappers/cpp/deps
    fi
    cp ../src/napi-service/json-napi.h ../wrappers/cpp/deps

fi