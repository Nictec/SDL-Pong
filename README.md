# SDL-Pong
A C++/Rust/Wasm project for learning SDL and developing a simple Game from scratch.
## Native
Only Supported on Linux/Mac.
Needs SDL
Linux:
```bash
make linux
make run
```

Mac OS:
```bash
make mac
make run
```

## WebAssembly
Only Tested on Linux
Needs emscripten
```bash
make wasm-html
make run-html
# open 'localhost:8008' in a browser
```
or node: (doesn't work yet)
```bash
make wasm-node
make run-node
```

## Rust
Only Tested on Linux
Needs SDL, cargo
```bash
cargo run
```
