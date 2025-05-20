if [ -d debug ]; then
  rm -rf debug
fi
mkdir debug
cd debug
echo "Debugging in progress..."
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . --config Debug
