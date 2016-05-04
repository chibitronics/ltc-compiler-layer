#!/usr/bin/perl -w
use warnings;
use strict;

my @c_syscalls;
my @cxx_syscalls;
my $num_syscalls = 0;

print "#include <stdint.h>\n";
print "#include \"Arduino.h\"\n";

my $fh;
open($fh, '<', "syscalls-c.txt") or die("Couldn't open syscalls-c: $!");
print "extern \"C\" {\n";
while (my $line = <$fh>) {
  chomp $line;
  if ($line =~ /^!/) {
    $line =~ s/^!//g;
  }
  else {
    print "    extern uint32_t $line;\n";
  }
  $num_syscalls++;
  push(@c_syscalls, $line);
}
print "};\n";
close($fh);

open($fh, '<', "syscalls-c++.txt") or die("Couldn't open syscalls-c++: $!");
while (my $line = <$fh>) {
  chomp $line;
  push(@cxx_syscalls, $line);
  $num_syscalls++;
  print "extern $line;\n";
}
close($fh);

if ($num_syscalls >= 256) {
  die("Too many syscalls: Limited to 256 (not $num_syscalls)\n");
}

print "uint32_t *SysCall_Table[] = {\n";
for my $syscall(@c_syscalls) {
  print "    (uint32_t *)&$syscall,\n";
}
for my $syscall(@cxx_syscalls) {
# extern bool isLowerCase(int c);

  my ($name) = $syscall =~ /^[^\)]+ (\w+)\(/;
  $syscall =~ s/$name/\(\*\)/g;
  print "    (uint32_t *)static_cast<$syscall>(&$name),\n";
}
print "};\n";
