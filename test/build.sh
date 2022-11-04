export project_path=/home/liudy/Seafile/WorkSpace/memfs/plasma/
g++ --std=c++17 -I $project_path -I $project_path/common -I $project_path/common/thirdparty -L $project_path/build main.cpp -lplasma_client -o main

# g++ --std=c++17 -I $project_path -I $project_path/common -I $project_path/common/thirdparty -L $project_path/build create.cpp -lplasma_client -o main

# g++ --std=c++17 -I $project_path -I $project_path/common -I $project_path/common/thirdparty -L $project_path/build pthread.cpp -lpthread -lplasma_client -o t

g++ --std=c++17 -I $project_path -I $project_path/common -I $project_path/common/thirdparty -L $project_path/build put.c -lplasma_client -o put
