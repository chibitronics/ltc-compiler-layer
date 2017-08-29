#!/usr/bin/perl -w
use warnings;
use strict;

sub generate_files {
  my ($syscalls, $os_fh, $app_fh, $c_app_fh) = @_;

  my @c_syscalls;
  my @c_ext_syscalls;
  my @cxx_methods;
  my @cxx_member_methods;
  my @cxx_constructors;
  my @cxx_destructors;
  my @reserved;
  my @table;

  if (@$syscalls >= 256) {
    die("Too many syscalls: Limited to 256 (not @$syscalls)\n");
  }

  my $idx = 0;
  for my $call(@$syscalls) {
    unshift(@$call, $idx);

    if ($call->[1] eq "c") {
      my ($call_name) = ($call->[2]);
      if (defined($call->[3])) {
        my @subcalls = split(/\s*,\s*/, $call->[3]);

        # Add an alias for the syscall name, to avoid confusion
        if (!grep(/^$call_name$/, @subcalls)) {
          push(@subcalls, $call_name);
        }

        # For each alias, add it to the list of C calls
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
    elsif ($call->[1] eq "P") {
      my ($call_name) = ($call->[2]);
      if (defined($call->[5])) {
        my @subcalls = split(/\s*,\s*/, $call->[5]);

        # Add an alias for the syscall name, to avoid confusion
        if (!grep(/^$call_name$/, @subcalls)) {
          push(@subcalls, $call_name);
        }

        # For each alias, add it to the list of C calls
        for my $sc(@subcalls) {
          my @new_call = @$call;
          $new_call[5] = $sc;
          push (@c_ext_syscalls, \@new_call);
        }
      }
      else {
        $call->[5] = $call->[2];
        push (@c_ext_syscalls, $call);
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
    if ($call->[1] eq "r") {
      push (@reserved, $call);
      $idx++;
    }
  }

  print $app_fh "#include \"Arduino-types.h\"\n";

  print $os_fh ".section .rodata\n";
  print $os_fh ".global SysCall_Table\n";
  print $os_fh "SysCall_Table:\n";

  if (@c_syscalls) {
    my %seen_c_syscalls;

    print $c_app_fh ".syntax unified\n";
    print $c_app_fh ".cpu cortex-m0plus\n";
    print $c_app_fh ".fpu softvfp\n";
    print $c_app_fh ".eabi_attribute 20, 1\n";
    print $c_app_fh ".eabi_attribute 21, 1\n";
    print $c_app_fh ".eabi_attribute 23, 3\n";
    print $c_app_fh ".eabi_attribute 24, 1\n";
    print $c_app_fh ".eabi_attribute 25, 1\n";
    print $c_app_fh ".eabi_attribute 26, 1\n";
    print $c_app_fh ".eabi_attribute 30, 4\n";
    print $c_app_fh ".eabi_attribute 34, 0\n";
    print $c_app_fh ".eabi_attribute 18, 4\n";
    print $c_app_fh ".thumb\n";
    print $c_app_fh ".syntax unified\n";
    print $c_app_fh ".file   \"syscalls-app-c.S\"\n";
    print $c_app_fh ".text\n";
    print $c_app_fh "\n";
    print $c_app_fh ".cfi_sections   .debug_frame\n";

    for my $c_syscall(@c_syscalls) {
      my ($idx, $code, $sysname, $name) = @$c_syscall;

      $table[$idx] = $c_syscall;

      print $c_app_fh ".section	.text.$name,\"ax\",\%progbits\n";
      print $c_app_fh ".align	1\n";
      print $c_app_fh ".global	$name\n";
      print $c_app_fh ".code	16\n";
      print $c_app_fh ".thumb_func\n";
      print $c_app_fh ".type	$name, %function\n";
      print $c_app_fh "$name:\n";
      print $c_app_fh ".cfi_startproc\n";
      print $c_app_fh "@ Naked Function: prologue and epilogue provided by programmer.\n";
      print $c_app_fh "@ args = 0, pretend = 0, frame = 0\n";
      print $c_app_fh "@ frame_needed = 0, uses_anonymous_args = 0\n";
      print $c_app_fh ".syntax divided\n";
      print $c_app_fh "svc #$idx\n";
      print $c_app_fh ".thumb\n";
      print $c_app_fh ".syntax unified\n";
      print $c_app_fh ".cfi_endproc\n";
      print $c_app_fh ".size	$name, .-$name\n";
    }
  }

  if (@c_ext_syscalls) {
    my %seen_c_syscalls;

    for my $c_syscall(@c_ext_syscalls) {
      my ($idx, $code, $sysname, $rettype, $args, $name) = @$c_syscall;

      $table[$idx] = $c_syscall;

      print $c_app_fh "/* $rettype $name($args) { */\n";
      print $c_app_fh ".section	.text.$name,\"ax\",\%progbits\n";
      print $c_app_fh ".align	1\n";
      print $c_app_fh ".global	$name\n";
      print $c_app_fh ".code	16\n";
      print $c_app_fh ".thumb_func\n";
      print $c_app_fh ".type	$name, %function\n";
      print $c_app_fh "$name:\n";
      print $c_app_fh ".cfi_startproc\n";
      print $c_app_fh "@ Naked Function: prologue and epilogue provided by programmer.\n";
      print $c_app_fh "@ args = 0, pretend = 0, frame = 0\n";
      print $c_app_fh "@ frame_needed = 0, uses_anonymous_args = 0\n";
      print $c_app_fh ".syntax divided\n";
      print $c_app_fh "svc #$idx\n";
      print $c_app_fh ".thumb\n";
      print $c_app_fh ".syntax unified\n";
      print $c_app_fh ".cfi_endproc\n";
      print $c_app_fh ".size	$name, .-$name\n";
    }
  }

  for my $cxx_method(@cxx_methods) {
    my ($idx, $code, $name, $rettype, $args, $names) = @$cxx_method;

    if (!defined($names)) {
      $names = "";
    }
    $table[$idx] = $cxx_method;

    print $app_fh "  __attribute__((naked))\n";
    print $app_fh "  $rettype $name($args) {\n";
    print $app_fh "    asm(\"svc #$idx\");\n";
    print $app_fh "  }\n";
  }
  print $os_fh "\n";

  # Generate the syscall table

  for my $i (0 .. @table-1) {
    my $syscall = $table[$i];
    if (!defined($syscall)) {
      print $os_fh ".short 0\n";
    }
    elsif ($syscall->[1] eq "c") {
      my ($idx, $code, $sysname, $name) = @$syscall;
      $sysname = $name if (!defined($sysname));
      print $os_fh ".short $sysname\n";
    }
    elsif ($syscall->[1] eq "P") {
      my ($idx, $code, $sysname, $rettype, $args, $name) = @$syscall;
      $sysname = $name if (!defined($sysname));
      print $os_fh ".short $sysname\n";
    }
    elsif ($syscall->[1] eq "x") {
      my ($idx, $code, $name, $rettype, $args, $names) = @$syscall;
      print $os_fh ".short $name\n";
    }
    else {
      print $os_fh ".short 0\n";
    }
  }
  print $os_fh ".size SysCall_Table, .-SysCall_Table\n";
}

my @syscalls;

open(my $os_syscalls_fh, '>', "syscalls-os.s") or die("Couldn't open syscalls-os.s: $!");
open(my $app_syscalls_fh, '>', "syscalls-app.cpp") or die("Couldn't open syscalls-app.cpp: $!");
open(my $c_app_syscalls_fh, '>', "syscalls-c-app.S") or die("Couldn't open syscalls-c-app.S: $!");

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

generate_files(\@syscalls, $os_syscalls_fh, $app_syscalls_fh, $c_app_syscalls_fh);
