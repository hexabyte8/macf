# macf - MAC Address Formatter

![Build](https://github.com/hexabyte8/macf/workflows/Build/badge.svg)

`macf` is a command-line utility for converting MAC addresses into different formats. It can process a single MAC address or multiple MAC addresses from a file.

## Download

Pre-built binaries are available for Linux and macOS:

### From GitHub Actions
1. Go to the [Actions tab](https://github.com/hexabyte8/macf/actions)
2. Click on the latest successful "Build" workflow run
3. Download the artifact for your platform:
   - `macf-linux-x86_64` for Linux (x86_64)
   - `macf-macos-arm64` for macOS (Apple Silicon ARM64)
4. Extract the archive and make the binary executable: `chmod +x macf`

### From Releases
When a release is published, download pre-built binaries from the [Releases page](https://github.com/hexabyte8/macf/releases):
- `macf-linux-x86_64.tar.gz` for Linux (x86_64)
- `macf-macos-arm64.tar.gz` for macOS (Apple Silicon ARM64)

Extract and install:
```bash
tar -xzf macf-*.tar.gz
cd macf-*
chmod +x macf
sudo mv macf /usr/local/bin/
sudo mv macf.1 /usr/local/share/man/man1/
```

## Build from Source

To compile from source, you need GCC:

```bash
gcc -o macf main.c
```

Or use the provided installation script:

```bash
./install.sh
``` 

## Features

- Convert MAC addresses to various formats:
  - Cisco format: `0000.0000.0000`
  - Dash-separated format: `00-00-00-00-00-00`
  - Colon-separated format: `00:00:00:00:00:00`
  - Raw format (no separators): `000000000000`
- Supports single MAC address input or file-based input.
- Outputs results to standard output or a specified file.
- Verbose mode for detailed output.

## Usage

### Syntax

```bash
macf [MAC_ADDRESS] FORMAT [-f FILE] [-v]
