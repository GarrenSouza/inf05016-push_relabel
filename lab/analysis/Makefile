
setup:
	./setup-environment.sh

analyze: run-tests.sh
	./run-tests.sh

clean:
	rm -rf data graphs bin generate-graphs.sh analysis-batch.sh

from-the-top:
	make clean
	make setup
	make analyze
