#!/bin/bash
# Script to create bootable ISO without mformat issues

ISO_DIR="$1"
ISO_OUTPUT="$2"

# Create a temporary mtools config that doesn't check anything
MTOOLS_CONF=$(mktemp)
cat > "$MTOOLS_CONF" << 'EOF'
mtools_skip_check=1
EOF

# Export the config and try grub-mkrescue
export MTOOLSRC="$MTOOLS_CONF"
export MTOOLS_SKIP_CHECK=1

grub-mkrescue -o "$ISO_OUTPUT" "$ISO_DIR" 2>&1
RESULT=$?

# Clean up
rm -f "$MTOOLS_CONF"

exit $RESULT
