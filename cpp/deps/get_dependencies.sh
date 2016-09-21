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

