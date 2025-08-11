#!/bin/bash

set -e # Exit immediately if a command fails

PROGRAM_NAME="macf"
MAN_PAGE_NAME="macf.1"
INSTALL_DIR="/usr/local/bin"
MAN_DIR="/usr/local/share/man/man1"

usage() {
  echo "Usage: $0 [--uninstall]"
  echo "Install or uninstall the macf program."
}

install() {
  echo "Installing $PROGRAM_NAME..."

  echo "Compiling $PROGRAM_NAME..."
  gcc -o "$PROGRAM_NAME" main.c

  echo "Moving $PROGRAM_NAME to $INSTALL_DIR..."
  sudo mv "$PROGRAM_NAME" "$INSTALL_DIR/$PROGRAM_NAME"

  echo "Installing man page..."
  sudo mkdir -p "$MAN_DIR"
  sudo cp "$MAN_PAGE_NAME" "$MAN_DIR/$MAN_PAGE_NAME"
  sudo mandb

  echo "$PROGRAM_NAME installed successfully."
}

uninstall() {
  echo "Uninstalling $PROGRAM_NAME..."

  echo "Removing $PROGRAM_NAME from $INSTALL_DIR..."
  sudo rm -f "$INSTALL_DIR/$PROGRAM_NAME"

  echo "Removing man page from $MAN_DIR..."
  sudo rm -f "$MAN_DIR/$MAN_PAGE_NAME"
  sudo mandb

  echo "$PROGRAM_NAME uninstalled successfully."
}

if [[ "$1" == "--uninstall" ]]; then
  uninstall
elif [[ "$1" == "--help" ]]; then
  usage
else
  install
fi
