#!/usr/bin/perl

my $line, @line_items, %allocation_map, @abandonned, $mem_loc;

foreach $line ( <STDIN> ) {
    chomp( $line );
    @line_items = split(',', $line);
    if ( $#line_items <= 1 ) {
        continue;
    }
    if ( $line_items[0] eq "M" || $line_items[0] eq "C" || $line_items[0] eq "S" ) {
        $allocation_map{$line_items[1]} = $line;
    } elsif ( $line_items[0] eq "R" ) {
        if ( $#line_items <= 2 ) {
            continue;
        }
        delete $allocation_map{$line_items[1]};
        $allocation_map{$line_items[2]} = $line;
    } elsif ( $line_items[0] eq "F" ) {
        delete $allocation_map{$line_items[1]};
    }
}

@abandonned = keys %allocation_map;
for $mem_loc (@abandonned) {
    print "$allocation_map{$mem_loc}\n";
}