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
    unshift(@$call, $idx);

    if ($call->[1] eq "c") {
      if (defined($call->[3])) {
        my @subcalls = split(/\s*,\s*/, $call->[3]);
        for my $sc(@subcalls) {
          my @new_call = @$call;
          $new_call[3] = $sc;
          push (@c_syscalls, \@new_call);
        }
      }
      else {
        $call->[3] = $call->[2];
        push (@c_syscalls, $call);
      }
      $idx++;
    }

    if ($call->[1] eq "x") {
      push (@cxx_methods, $call);
      $idx++;
    }

    push (@cxx_member_methods, $call) if ($call->[1] eq "m");
    push (@cxx_constructors, $call) if ($call->[1] eq "C");
    push (@cxx_destructors, $call) if ($call->[1] eq "D");
  }

  print $app_fh "#include <stdint.h>\n";
  print $app_fh "#include \"Arduino-types.h\"\n";

#  if (@c_syscalls) {
#    open(my $test_fh, '>', "eabi-lookup.c");
#    for my $c_syscall(@c_syscalls) {
#      my ($idx, $code, $sysname, $name) = @$c_syscall;
#      next unless $sysname =~ /qfp_/;
#      print $test_fh "extern float $sysname(void);\n";
#      print $test_fh "float $name(void) { return $sysname(); }\n";
#    }
#    close($test_fh);
#  }

  print $os_fh ".section .rodata\n";
  print $os_fh ".global SysCall_Table\n";
  print $os_fh "SysCall_Table:\n";

  if (@c_syscalls) {
    my %seen_c_syscalls;

    print $app_fh "extern \"C\" {\n";
    for my $c_syscall(@c_syscalls) {
      my ($idx, $code, $sysname, $name) = @$c_syscall;

      $table[$idx] = $c_syscall;

#      if (!exists($seen_c_syscalls{$sysname})) {
#        print $os_fh "extern uint32_t $sysname;\n";
#        $seen_c_syscalls{$sysname} = 1;
#      }
      print $app_fh "    __attribute__((naked))\n";
      print $app_fh "    void $name(void) {\n";
      print $app_fh "      asm(\"svc #$idx\");\n";
      print $app_fh "    }\n";
    }
    print $app_fh "};\n";
  }

  for my $cxx_method(@cxx_methods) {
    my ($idx, $code, $name, $rettype, $args, $names) = @$cxx_method;

    if (!defined($names)) {
      $names = "";
    }
    $table[$idx] = $cxx_method;
#    print $os_fh "extern uint32_t $name;\n";

    print $app_fh "  __attribute__((naked))\n";
    print $app_fh "  $rettype $name($args) {\n";
    print $app_fh "    asm(\"svc #$idx\");\n";
    print $app_fh "  }\n";
  }
  print $os_fh "\n";

  # Generate the syscall table
  # TODO: Remove this __attribute__
#  print $os_fh "__attribute__((section(\".rodata\")))\n";
#  print $os_fh "const uint32_t * const SysCall_Table[] = {\n";

  for my $i (0 .. @table-1) {
    my $syscall = $table[$i];
    if (!defined($syscall)) {
      print $os_fh ".short 0\n";
#      print $os_fh "    (const uint32_t *)0,\n";
    }
    elsif ($syscall->[1] eq "c") {
      my ($idx, $code, $sysname, $name) = @$syscall;
      $sysname = $name if (!defined($sysname));
      print $os_fh ".short $sysname\n";
#      print $os_fh "    (const uint32_t *)&$sysname,\n";
    }
    elsif ($syscall->[1] eq "x") {
      my ($idx, $code, $name, $rettype, $args, $names) = @$syscall;
      print $os_fh ".short $name\n";
#      print $os_fh "    (const uint32_t *)&$name\n";
    }
    else {
      print $os_fh ".short 0\n";
#      print $os_fh "    (const uint32_t *)0,\n";
    }
  }
  print $os_fh ".size SysCall_Table, .-SysCall_Table\n";
#  print $os_fh "};\n";
}

my @syscalls;

open(my $os_syscalls_fh, '>', "syscalls-os.s") or die("Couldn't open syscalls-os.s: $!");
open(my $app_syscalls_fh, '>', "syscalls-app.cpp") or die("Couldn't open syscalls-app.cpp: $!");

# Read in the syscalls DB
open(my $input_fh, '<', "syscalls-db.txt")
    or die("Couldn't open syscalls-db.txt: $!");
while (my $line = <$input_fh>) {
  chomp $line;
  next if ($line =~ /^\s*\#/);
  next if ($line =~ /^\s*$/);

  my @fields = split(/\s*\|\s*/, $line);
  push(@syscalls, \@fields);
}

generate_files(\@syscalls, $os_syscalls_fh, $app_syscalls_fh);
