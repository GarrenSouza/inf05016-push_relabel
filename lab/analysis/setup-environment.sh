#!/bin/bash

echo "cleaning & creating directories..."
rm -rf data graphs bin
mkdir graphs data

echo "provisioning binaries..."
cmake ../../ -DCMAKE_BUILD_TYPE=Release -Bbin
cmake --build bin

echo "generating commands for graph generation..."
echo -e "#!/bin/bash\n" > generate-graphs.sh
chmod +x generate-graphs.sh

python3 generate-graphs_commands.py >> generate-graphs.sh

echo "generating graphs..."
./generate-graphs.sh
