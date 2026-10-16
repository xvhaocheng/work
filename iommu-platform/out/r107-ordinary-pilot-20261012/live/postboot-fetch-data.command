mkdir -m 700 /tmp/r107-stage111 &&
mkdir -m 700 /tmp/r107-stage111/data &&
wget -Y off -T 60 -q -O /tmp/r107-stage111/idma-finite http://192.168.77.1:18080/idma-finite &&
wget -Y off -T 60 -q -O /tmp/r107-stage111/data/worker0-cpu_source.bin http://192.168.77.1:18080/worker0-cpu_source.bin &&
wget -Y off -T 60 -q -O /tmp/r107-stage111/data/worker0-initial_gpu.bin http://192.168.77.1:18080/worker0-initial_gpu.bin &&
wget -Y off -T 60 -q -O /tmp/r107-stage111/data/worker0-restored_gpu.bin http://192.168.77.1:18080/worker0-restored_gpu.bin &&
wget -Y off -T 60 -q -O /tmp/r107-stage111/data/worker1-cpu_source.bin http://192.168.77.1:18080/worker1-cpu_source.bin &&
wget -Y off -T 60 -q -O /tmp/r107-stage111/data/worker1-initial_gpu.bin http://192.168.77.1:18080/worker1-initial_gpu.bin &&
wget -Y off -T 60 -q -O /tmp/r107-stage111/data/worker1-restored_gpu.bin http://192.168.77.1:18080/worker1-restored_gpu.bin &&
wget -Y off -T 60 -q -O /tmp/r107-stage111/FINITE_DATA_SHA256SUMS http://192.168.77.1:18080/FINITE_DATA_SHA256SUMS &&
cd /tmp/r107-stage111 &&
sha256sum -c FINITE_DATA_SHA256SUMS &&
chmod 700 idma-finite
