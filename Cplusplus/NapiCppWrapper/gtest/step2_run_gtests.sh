# NapiCpp Wrapper tests are only for OSX.

# get dependencies, but don't build napi.
# !!!!!!!!!! QA *must* provide the following files !!!!!!!!!!
# json-napi.h, libnapi-net.a, libnapi.a into the api/wrappers/cpp/deps folder.
cd ../deps
/bin/sh get_dependencies.sh dont-build-napi
cd ../gtest

# clean gtest project
xcodebuild clean -project osx/NapiCppWrapperTests.xcodeproj

# we need this so the gtests project can access the gtest framework in libs/osx
export DYLD_FRAMEWORK_PATH=libs/osx

# build gtest project
xcodebuild -project osx/NapiCppWrapperTests.xcodeproj

# napi needs this config file to be able to initialize
cp ../deps/config.json .

# run the tests!
osx/build/Release/NapiCppWrapperTests

# all output (e.g. napi logs) are written to the directory where this script is executed from,
# currently api/wrappers/cpp/gtest
