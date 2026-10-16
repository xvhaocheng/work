test -r /tmp/r107-stage111/finite-result/receipt.json &&
tar -cf /tmp/r107-stage111/finite-result.tar -C /tmp/r107-stage111/finite-result . &&
sha256sum /tmp/r107-stage111/finite-result.tar &&
base64 /tmp/r107-stage111/finite-result.tar > /tmp/r107-stage111/finite-result.b64 &&
wc -c /tmp/r107-stage111/finite-result.b64 &&
wget -Y off -T 60 -q -O /tmp/r107-stage111/finite-result-transfer.json --post-file /tmp/r107-stage111/finite-result.b64 http://192.168.77.1:18080/finite-results &&
cat /tmp/r107-stage111/finite-result-transfer.json
