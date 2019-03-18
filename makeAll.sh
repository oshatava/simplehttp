echo "Clean"
rm ./box
rm ./lib/libserver.a
echo "Build server lib"
cd ./lib
cmake .>out.txt
make>out.txt
rm ./out.txt
echo "Build box"
cd ../
cmake .>out.txt
make>out.txt
rm ./out.txt
echo "Run box"
echo ./box