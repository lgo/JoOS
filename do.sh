#!/bin/sh

#things to do: objdir, outdir, boot, kernel,
OBJDIR="./objs"
OUTDIR="./out"
BOOT="./boot"
KERNEL="./kernel"


CC="i586-elf-gcc"
AS="i586-elf-as"
LD="i586-elf-gcc"

clean() {
  echo -ne "cleaning loose build files ..."
  rm -rf $OBJDIR $OUTDIR
  echo " [done]"
}

build() {
  if [ ! -d "$OBJDIR" ]; then
    mkdir $OBJDIR $OUTDIR;
  fi
  
  #boot -> asm
  echo -ne "building boot ..."
  flist=`cd $BOOT/; ls *.s;`
  `cd ../`
  for f in $flist; do
    `$AS $BOOT/$f -o $OBJDIR/${f/.s/.o}`
  done
  echo " [done]"

  #kernel -> asm
  #echo -ne "building kernel ..."
  #flist=`cd $KERNEL/; ls *.s;`
  #`cd ../`
  #for f in $flist; do
  #  `$AS $KERNEL/$f -o $OBJDIR/${f/.s/.o}`
  #done

  #kernel -> c
  echo -ne "building kernel ..."
  flist=`cd $KERNEL/; ls *.c;`
  `cd ../`
  for f in $flist; do
    `$CC -c $KERNEL/$f -o $OBJDIR/${f/.c/.o} -std=gnu99 -ffreestanding -O2 -Wall -Wextra -I./inc`
  done
  echo " [done]"

  #linker
  echo -ne "generating binary ..."
  `$LD -T linker.ld -o $OUTDIR/kernel.bin -ffreestanding -O2 -nostdlib -lgcc $OBJDIR/*.o`
  echo " [done]"
}

run() {
  echo -ne "starting QEMU test environment from kernel binary ..."
  qemu-system-i386 -kernel $OUTDIR/kernel.bin &
  echo " [done]"
}

commit() {
  cmd="git commit -a -m\"log\""
  echo $cmd
  git add .;
  git commit -a -m"$log"
  git push;
}

show_help() {
  echo "-clean | -c     do a clean up"
  echo "-build | -b     do a build"
  echo "-run   | -r     run a emulation"
  echo "-commit| -g     do a git commit"
}

all() {
  clean;
  build;
  run;
}

while [ $# -gt 0 ] 
do
  case $1 in
    -help|-h) show_help; exit 0;;
    -clean|-c) clean; exit 0;;
    -build|-b) clean; build; exit 0;;
    -run|-r) clean; build; run; exit 0;;
    -commit|-g) shift; log=$1; commit; exit 0;
  esac
  shift
done

show_help $DEFAULT;
