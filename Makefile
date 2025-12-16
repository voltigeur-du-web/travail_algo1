PYTHON_VERSION = "3.11.6"
VENV_NAME := $(shell cat .python-version)


setup-venv: _install_python_version _install_virtualenv _install_local_package _setup_venv_cheers


_install_python_version:
	# Ensures that the version of python required by CMP is installed
	echo "\n>Install Python version in pyenv if it doesn't exist yet..."
	if pyenv versions | grep -q $(PYTHON_VERSION); then \
		echo "- python $(PYTHON_VERSION) installation was found in pyenv"; \
	else \
		echo "- python $(PYTHON_VERSION) installation was not found in pyenv, installing it..."; \
		pyenv install $(PYTHON_VERSION); \
	fi

_install_virtualenv:
	# Resets the CMP virtual env
	echo "\n>Installing virtual env..."
	echo "- Uninstalling previous venv..."
	pyenv uninstall -f $(VENV_NAME)
	echo "- Installing venv..."
	pyenv virtualenv $(PYTHON_VERSION) $(VENV_NAME)
	echo "- Upgrading pip..."
	pip install --upgrade pip wheel

_setup_venv_cheers:
	echo "\n>Project python venv and git hooks are set up, you're ready to rock 🚀\n"

_install_local_package:
	echo "\n>Installing project packages and dependencies..."
	pip install -r requirements.txt


compile-requirements-txt:
	pip-compile --no-emit-index-url -o requirements.txt

compile_dijkstra_shared_lib:
	echo "\n>Compiling dijkstra shared library..."
	rm -f advanced_dijkstra_algo_c/build/libdijkstra.so
	gcc -shared -fPIC -o dijkstra_algo_c/build/libdijkstra.so \
		advanced_dijkstra_algo_c/src/jsonize.c \
		advanced_dijkstra_algo_c/src/dijkstra.c \
		advanced_dijkstra_algo_c/src/nodes.c \
		advanced_dijkstra_algo_c/src/pq_tasbinaire_pointeur.c
	echo "\n>Compilation done 🚀\n"

compile_dijkstra_shared_lib_debug:
	echo "\n>Compiling dijkstra shared library..."
	rm -f dijkstra_algo_c/build/libdijkstra.so
	gcc -g -shared -fPIC -o dijkstra_algo_c/build/libdijkstra.so \
		advanced_dijkstra_algo_c/src/jsonize.c \
		advanced_dijkstra_algo_c/src/dijkstra.c \
		advanced_dijkstra_algo_c/src/nodes.c \
		advanced_dijkstra_algo_c/src/pq_tasbinaire_pointeur.c
	echo "\n>Compilation done 🚀\n"

run_tests_algo_c:
	echo "\n>Running tests for PQ..."
	cd advanced_dijkstra_algo_c/tests && gcc -o tests_priority_queue_pointeur.out ../src/pq_tasbinaire_pointeur.c tests_priority_queue_pointeur.c && ./tests_priority_queue_pointeur.out 
	echo "\n>Running tests for nodes methods..."
	cd advanced_dijkstra_algo_c/tests && gcc -o tests_nodes.out ../src/nodes.c tests_nodes.c && ./tests_nodes.out 
	echo "\n>Running tests for exploration and full dijkstra algo methods..."
	cd advanced_dijkstra_algo_c/tests && gcc -o dijkstra.out ../src/dijkstra.c ../src/nodes.c ../src/pq_tasbinaire_pointeur.c tests_dijkstra.c && ./dijkstra.out
	echo "\n>Running tests for json converter..."
	cd advanced_dijkstra_algo_c/tests && gcc -o tests_itinerary_to_json.out ../src/jsonize.c ../src/dijkstra.c ../src/nodes.c ../src/pq_tasbinaire_pointeur.c tests_itinerary_to_json.c && ./tests_itinerary_to_json.out
	echo "\n>All tests passed 🚀\n"
