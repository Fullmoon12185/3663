 #!/bin/bash
 
 make DISABLE_LINT=1
 
GREEN='\033[0;32m'
NC='\033[0m' 
echo "Hello, World!"
cp -v -f "./Bin_MIPS_MARLON_128M_DVBT_SZ_NPN/MERGE.bin" "./Bin_MIPS_MARLON_128M_DVBT_SZ_NPN/UBC_TV32.BIN" 
printf "${GREEN}Copy UBC_TV32.BIN Done!${NC}\n"