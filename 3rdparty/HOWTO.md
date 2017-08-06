== Add new 3rd party repo as a submodule
git submodule add https://github.com/msgpack/msgpack-c.git msgpack-c
cd msgpack-c
# Look for the desired tag name at https://github.com/msgpack/msgpack-c/releases
git checkout cpp-2.1.2
cd ..
git add msgpack-c
git commit -m "added/updated submodule to v2.1.2"

