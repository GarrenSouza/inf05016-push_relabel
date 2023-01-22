#!/bin/bash

echo -e "#!/bin/bash\n" > analysis-batch.sh
chmod +x analysis-batch.sh

python3 generate-test_commands.py >> analysis-batch.sh

./analysis-batch.sh
