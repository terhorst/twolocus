all:
	CFLAGS='-O3' CC="${HOME}/opt/bin/gcc" CXX="${HOME}/opt/bin/g++" python setup.py build_ext -i
clean:
	python setup.py clean
