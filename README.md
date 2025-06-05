A simple pathtracer.

Building:
```bash
# clone repo
git clone git@github.com:mps0/pt.git

# get minifb module
git submodule update --init

#build folder
mkdir build && cd build

# configure and build
cmake .. && cmake --build .
```

First render:

<img src="imgs/first.png" width="517">
<img src="imgs/glass.png" width="517">

Comparing standard path tracing with photon mapping:

Standard path tracing:

<img src="imgs/pt.png" width="517">

Photonmap:

<img src="imgs/photonmap_N25.png" width="517">


Care has to be taken with how lights are sampled (directly or indirectly),
as well as how photons are shot and how much energy each photon has for
the two methods to converge to the same result.


Photonmap Glass:

<img src="imgs/photonmap_glass.png" width="517">
