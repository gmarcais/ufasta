# ufasta: utility to handle fasta files

## Compilation

``` Shell
./configure
make
make install
```

If compiling from the git tree, first run `autoreconf -fi`.

For the `grep` sub-commands to work, the BOOST regexp library must be installed.

## Usage

`ufasta -h` gives the list of subcommands: one, sizes, head, tail, rc, n50, stats, extract, format, hsort, sort, dsort, rsort, hgrep, dgrep, split.
Similarly every subcommand has its own help.
Here is a quick description of the subcommands:

* `one`: remove the new lines in the data section. Hence, all the sequences are written on one line. In some sense, it is the opposite of the `format` subcommand.
* `format`: reformat the data sections. The data is written in lines of the same length, it can changes the content in upper/lower case.
* `sizes`: print the amount of sequence in each section
* `head`: like UNIX head. Display the first 10 sequences
* `tail`: like UNIX tail. Display the last 10 sequences
* `rc`: reverse complement every sequence
* `n50`, `stats`: display stats about the sequences: N50, E size, total size, etc.
* `extract`: extract a sequence whose header match given names
* `hsort`, `sort`: sort file based on header content
* `dsort`: sort the data sections
* `hgreap`: output sequences whose header match the regular expression (if BOOST regexp available)
* `dgresp`: output sequences whose sequence match the regular expression (if BOOST regexp available)
* `split`: split a fasta file into many files
