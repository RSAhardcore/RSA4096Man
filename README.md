# RSA4096

RSA-4096 implementation with complete Montgomery REDC algorithm.

## RISC-V 64-bit Compilation

This project supports cross-compilation for RISC-V 64-bit architecture using the GNU RISC-V toolchain.

### Prerequisites

Install the RISC-V GNU toolchain:
```bash
# Ubuntu/Debian
sudo apt-get install gcc-riscv64-linux-gnu

# Or build from source
git clone https://github.com/riscv/riscv-gnu-toolchain
```

### Build Commands

```bash
# Build for RISC-V 64-bit
make ARCH=riscv64

# Or use the convenience target
make riscv64

# Full production build for RISC-V
make riscv64-production

# Check compiler and architecture info
make ARCH=riscv64 version
```

### Architecture Flags

The RISC-V build uses:
- `riscv64-linux-gnu-gcc` compiler
- `-march=rv64gc` (RV64 with GC extensions)
- `-mabi=lp64d` (LP64D ABI for double precision floating point)

### Build Status

- ✅ Native x86_64 compilation
- ✅ RISC-V 64-bit cross-compilation support
- ✅ Complete Montgomery REDC implementation
- ✅ RSA-4096 capable big integer arithmetic