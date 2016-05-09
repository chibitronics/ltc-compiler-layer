#!/usr/bin/perl -w
use warnings;
use strict;

sub generate_files {
  my ($syscalls, $os_fh, $app_fh) = @_;

  my @c_syscalls;
  my @cxx_methods;
  my @cxx_member_methods;
  my @cxx_constructors;
  my @cxx_destructors;
  my @table;

  if (@$syscalls >= 256) {
    die("Too many syscalls: Limited to 256 (not @$syscalls)\n");
  }

  my $idx = 0;
  for my $call(@$syscalls) {
    push(@$call, $idx);

    push (@c_syscalls, $call) if ($call->[0] eq "c");
    push (@cxx_methods, $call) if ($call->[0] eq "x");
    push (@cxx_member_methods, $call) if ($call->[0] eq "m");
    push (@cxx_constructors, $call) if ($call->[0] eq "C");
    push (@cxx_destructors, $call) if ($call->[0] eq "D");
    $idx++;
  }

  print $os_fh "#include <stdint.h>\n";
  print $app_fh "#include <stdint.h>\n";
  print $os_fh "#include \"Arduino.h\"\n";
  print $app_fh "#include \"Arduino.h\"\n";

  if (@c_syscalls) {
    print $os_fh "extern \"C\" {\n";
    print $app_fh "extern \"C\" {\n";
    for my $c_syscall(@c_syscalls) {
      my ($code, $name, $idx) = @$c_syscall;
      $table[$idx] = $c_syscall;
      print $os_fh "    extern uint32_t $name;\n";
      print $app_fh "    __attribute__((naked))\n";
      print $app_fh "    void $name(void) {\n";
      print $app_fh "      asm(\"svc #$idx\");\n";
      print $app_fh "      asm(\"bx lr    \");\n";
      print $app_fh "    }\n";
    }
    print $os_fh "};\n";
    print $app_fh "};\n";
  }

  for my $cxx_method(@cxx_methods) {
    my ($code, $name, $rettype, $args, $names, $idx) = @$cxx_method;

    if (!defined($idx)) {
      $idx = $names;
      $names = "";
    }
    $table[$idx] = $cxx_method;
    print $os_fh "extern $rettype $name($args);\n";

      print $app_fh "  __attribute__((naked))\n";
      print $app_fh "  $rettype $name($args) {\n";
      print $app_fh "    asm(\"svc #$idx\");\n";
      print $app_fh "    asm(\"bx lr    \");\n";
      print $app_fh "  }\n";
  }

  # Generate the syscall table
  print $os_fh "uint32_t *SysCall_Table[] = {\n";

  for my $i (0 .. @table-1) {
    my $syscall = $table[$i];
    if (!defined($syscall)) {
      print $os_fh "    (uint32_t *)0,\n";
    }
    elsif ($syscall->[0] eq "c") {
      my ($code, $name, $idx) = @$syscall;
      print $os_fh "    (uint32_t *)&$name,\n";
    }
    elsif ($syscall->[0] eq "x") {
      my ($code, $name, $rettype, $args, $names) = @$syscall;
      print $os_fh "    (uint32_t *)static_cast<$rettype (*)($args)>(&$name),\n";
    }
    else {
      print $os_fh "    (uint32_t *)0,\n";
    }
  }
  print $os_fh "};\n";
}

my @syscalls;

open(my $os_syscalls_fh, '>', "syscalls-os.cpp") or die("Couldn't open syscalls-os.cpp: $!");
open(my $app_syscalls_fh, '>', "syscalls-app.cpp") or die("Couldn't open syscalls-app.cpp: $!");

# Read in the syscalls DB
open(my $input_fh, '<', "syscalls-db.txt")
    or die("Couldn't open syscalls-db.txt: $!");
while (my $line = <$input_fh>) {
  chomp $line;
  next if ($line =~ /^^\s*\#/);

  my @fields = split(/\s*\|\s*/, $line);
  push(@syscalls, \@fields);
}

generate_files(\@syscalls, $os_syscalls_fh, $app_syscalls_fh);


=cut
# Read in the C++ syscalls
open(my $cxx_syscalls_fh, '<', "syscalls-c++.txt")
    or die("Couldn't open syscalls-c++: $!");

while (my $line = <$cxx_syscalls_fh>) {
  chomp $line;

  # Check to see if it's a constructor, which gets a special wrapper function.
  if ($line !~ /^[^\(]+ ([^\)]+)\(/) {
    print STDERR "[$line] is constructor\n";
    my ($class, $args) = $line =~ /^([^:]+)::[^\(]+\(([^\)]*)\)$/;
    print "Class: $class  Args: $args\n";
    $line = "$class *new__" . $class . "__(" . $args . ")";
    print $os_syscalls_fh "static $line {\n";
    print $os_syscalls_fh "  return new $class($args);\n";
    print $os_syscalls_fh "}\n";
  }
  else {
  }

  push(@cxx_syscalls, $line);
  $num_syscalls++;
}
close($cxx_syscalls_fh);


# Ensure we don't have more than 256 syscalls (in the future we'll overload
# svc #255 to support this)
if ($num_syscalls >= 256) {
  die("Too many syscalls: Limited to 256 (not $num_syscalls)\n");
}

print $os_syscalls_fh "uint32_t *SysCall_Table[] = {\n";
for my $syscall(@c_syscalls) {
  print $os_syscalls_fh "    (uint32_t *)&$syscall,\n";
}
for my $syscall(@cxx_syscalls) {
# extern bool isLowerCase(int c);

  my ($name, $args) = $syscall =~ /^\s([^\(]+)\(.*/;
  print STDERR "name: $name  Syscall: $syscall  Args: $args\n";
  $syscall =~ s/$name/\(\*\)/g;
  print $os_syscalls_fh "    (uint32_t *)static_cast<$syscall>(&$name),\n";
}
print $os_syscalls_fh "};\n";
