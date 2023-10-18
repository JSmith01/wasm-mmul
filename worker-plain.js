import loadWasmMmul from './mmul-plain.mjs';
let module;

function doMultiplication(size) {
    const arraySize = size * size;
    const outP = module._alloc_buf(size);
    const out = new Int32Array(module.HEAPU8.buffer, outP, arraySize);
    const inA = module._alloc_buf(size);
    const a = new Int32Array(module.HEAPU8.buffer, inA, arraySize);
    const inB = module._alloc_buf(size);
    const b = new Int32Array(module.HEAPU8.buffer, inB, arraySize);

    a.fill(1);
    b.fill(1);

    const ts = performance.now();
    module._mmul(outP, inA, inB, size);
    const mulTime = performance.now() - ts;
    const outMul = out.slice();

    Promise.resolve().then(() => {
        if (outMul.some(v => v !== size)) {
            console.warn('!!! Calculated incorrectly');
        }
    });

    module._free_buf(outP);
    module._free_buf(inA);
    module._free_buf(inB);

    return mulTime;
}

const modulePromise = loadWasmMmul().then(m => {
    module = m;
    self.postMessage('ready');
});

self.onmessage = async ({ data }) => {
    await modulePromise;

    self.postMessage(doMultiplication(data.size));
}
