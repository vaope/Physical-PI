build: 
	cmake -B build -DCMAKE_CXX_COMPILER=arm-buildroot-linux-gnueabihf-g++ -DCMAKE_SYSTEM_NAME=Linux -DCMAKE_SYSTEM_PROCESSOR=arm -DCMAKE_CXX_STANDARD=11 
	cmake --build build

clean:
	rm -rf build
	rm workspace/main

move:
	cp -r workspace/ ~/nfs_rootfs/test_project1/

