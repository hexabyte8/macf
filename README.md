# macf - MAC Address Formatter

`macf` is a command-line utility for converting MAC addresses into different formats. It can process a single MAC address or multiple MAC addresses from a file. 

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
