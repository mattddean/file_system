# Milestone 0

## Abilities
- "Test `mmap`"
    - read instructions from `stdin` and print values at the locations specified by these instructions from a given file
- "Test Boot Sector"
    - print values at important locations within FAT boot sector from a given file

## Arguments
`--test-mmap`: perform "Test `mmap`" ability as defined above
`--test-boot-sector`: perform "Test Boot Sector" ability as defined above
`--image` <image_file>: the file from which to print values

## Build
`make`

## Run
`cat <instruction_file> | ./fs [--test-mmap | --test-boot-sector] --image <image_file>

## Notes
- does not yet handle unexpected input (please don't *not* pass in the name of an image file, for example)

# Milestone 1

## Additional Ability
- "Test Directory Entry"
    - print values at important locations for a directory entry at a given entry number

## Additional Arguments
`--test-directory-entry` <entry #>: the entry number of the directory entry for which to print values

## Build
`make`

## Run
./fs [ --test-mmap | --test-boot-sector | --test-directory-entry <entry #> ] --image <image_file>

## Notes
- the implementation of `--test-directory-entry` has a small number of failing tests

# Milestone 2

## Additional Ability
- "Test File Clusters"
    - display the list of clusters for a specific entry in the FAT

## Additional Arguments
`--test-file-clusters` <start_cluster #>: the index of the first cluster

## Build
`make`

## Run
./fs: [ --test-mmap | --test-boot-sector | --test-directory-entry <entry #> | --test-file-clusters <start_cluster #> ] --image <image_file>

# Milestone 3
- Not yet complete. I focused mainly on refactorization this week, and have not yet completed this milestone.

# Milestone 4
## Additional Ability
- "Test File Name"
- "Test File Contents"

## Additional Arguments
`--test-file-name <file_name>`: the file name to test
`--test-file-contents <file_name>`: the file name to test

## Known Problems
- "Test File Name" only currently works for files in the root directory.
- "Test File Contents" does not work for files larger than one cluster and for those that have a null byte in them.
- No Milestone 4 functionality is present.

## Run
./fs: [ --test-mmap | --test-boot-sector | --test-directory-entry <entry #> | --test-file-clusters <start_cluster #> | --test-file-name <file_name> | --test-file-contents <file_name> ] --image <image_file>

# Milestone 5
## Additional Ability
- "Test Num Entries"
- "Write FS Data"

## Additional Functionality
- "Test File Name" now works for all or nearly all tests.

## Known Problems
- "Milestone 4 is missing functionality besides "Test File Name"
- "Write FS Data" is not complete. It may write data to an empty cluster and update one of the FAT's but I have not tested that it accurately does this, and the data that it writes is a static string because I do not have Milestone 4 complete. Additionally, it does not create a directory entry for the file.

./fs: [ --test-mmap | --test-boot-sector | --test-directory-entry <entry #> | --test-file-clusters <start_cluster #> | --test-file-name <file_name> | --test-file-contents <file_name> | --test-num-entries | --write-fs-data ] --image <image_file>