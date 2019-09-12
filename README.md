This is a project for the primitives in Elliptic Curve Cryptography (ECC). It is forked from https://github.com/esxgx/easy-ecc while some modifications have been done to make it  easier to build and read, as well as to make it a little more efficient.

There are several branches for different versions, ECC-8, ECC-32 and ECC-64, aiming at platforms with different sizes of words. Algorithms in these branches are generally the same.

# Use 

## compiling 

```sh
mkdir build-release 
cd build-release
cmake ../
make
`` 

or 

```sh 
mkdir build-debug 
cd build-debug 
cmake -DCMAKE_BUILD_TYPE=Debug ../
make 
``` 

## run the test programs 

```
./bin/keytest         -- key make, encrypt, decrypt, sign and verify 
./bin/pointtest       -- test scalar multipication or point addition
./bin/fieldtest       -- test number multiplication and squaring
```

