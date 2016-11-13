#ClusteredClumps: Finding Clustered-Clumps in Degenerate Strings
================================================================

*ClusteredClumps* is a tool that finds out clustered-clumps of 
- degenerate patterns in a solid text (Problem-1 or Search Type: A)
- solid patterns in a degenerate text (Problem-2 or Search Type: B)
- degenerate patterns in a degenerate text (Problem-3 or Search Type: C)

>A *degenerate string* is defined by the existence of one or more positions that are represented by sets of symbols from
>an alphabet Σ, unlike an accurate or certain (standard) string characterised by a single symbol at each position.
>For instance, {a,b} a c {b,c} a {b,c} is a degenerate string of length 6 over Σ = {a,b,c}.

>An *clustred-clump* is a maximal overlapping set of  occurrences (in the given text) of patterns from a given set.

The tool is based on the terminology and the algorithm described in the paper titled
[**Efficient Computation of Clustered-Clumps in Degenerate Strings**](http://dx.doi.org/10.1007/978-3-319-44944-9_45)
by *Costas S. Iliopoulos, Ritu Kundu, Manal Mohamed*

To compile ClusteredClumps, please follow the instructions given in file `INSTALL.md`


## Usage of the tool: 
./bin/cluclu <options>
with the following mandatory options:
- -s, --search <str>				'A' for Problem-1 (Solid Text and Degenerate Patterns) or 'B' for Problem-2 (Degenerate Text and Solid Patterns) or `C' for Problem-3 (Degenerate Text and Degenerate Patterns).
- -t, --text-input-file <str>		Input file  name for text (Elastic-degenerate string)
- -p, --pattern-input-file <str>	Input file  name for (degenerate) patterns.\
- -o, --output-file	<str>			Output filename.

 **Example:**  
```
./bin/cluclu -s C -t ./sample/sampleTextDegC.txt -p ./sample/samplePatternDeg.txt -o ./sample/sampleOutput.txt
```

Here, the degenerate text and degenerate patterns are given in files "sampleTextDegC.txt" and "samplePatternDeg.txt" which are in subfolder "sample" of current folder. 
Clustered-Clumps will be calculated and result will be written in the file "sampleOutput.txt" in subfolder "sample" of current folder.


## Notes
- Search Type can be one of the following:
  * **A** : for degenerate patterns and solid text (Problem-1)
  * **B** : for solid patterns and degenerate text (Problem-2)
  * **C** : for degenerate patterns and degenerate text (Problem-3)

- Current implementation does not have a provision to accommodate more than 255 symbols (size of unsigned character) due to the limitation of the external library used (SDSL). Therefore, (number of degnerate symbols in the text + number of patterns) for search B and C should not be more than (255-alphabet size).

- **Pattern file** is expected to be in the following format:
  * It starts with the alphabet given in the first line. Alphabet is case-sensitive.
  * From the second line, patterns start. Patterns are delimited by an empty line.
  * Each pattern starts with '>' followed by an identifier for the pattern. It is being ignored currently.
    - Starting from the next line (until an empty line or end of file is hit), each row contains 1(=>present) or 0(=>absent) corresponding to each symbol/letter specified in the first line.
    - A row represents a position/location in the pattern.
    - Only one '1' in a row represents a solid position while more than one '1's in the row implies that the position is non-solid (or degenerate).
    - Each row should have exactly same total number of '0's or '1's as that of letters/characters in the alphabet (given in the first line of the file).
    - There should be at least one '1' in a row.
    - White-space characters in a row are allowed (as these are ignored).
  * All solid patterns will also be given in the same format. It is the reponsibility of the user to ensure that there is only one '1' in a row in this case.

- **Solid Text file** is expected to be in the following format:
  * It is a sequence of letters from the alphabet specified by the corresponding pattern file.
  * White-space characters and new lines in the sequence are allowed (as these are ignored).
  * Sequence is assumed to end when either an empty line or end of file is hit.

- **Degenerate Text file** is expected to be in the following format:
  * It starts with the alphabet given in the first line. Alphabet is case-sensitive.
    - It should be same as the alphabet specified by the corresponding pattern file.
  * From the second line, text starts. It ends when either an empty line or end of file is hit.
  * It starts with a '>' followed by an identifier for the text. It is being ignored currently.
    - Starting from the next line (until an empty line or end of file is hit), each row contains 1(=>present) or 0(=>absent) corresponding to each symbol/letter specified in the first line.
    - A row represents a position/location in the text.
    - Only one '1' in a row represents a solid position while more than one '1's in the row implies that the position is non-solid (or degenerate).
    - Each row should have exactly same total number of '0's or '1's as that of letters/characters in the alphabet (given in the first line of the file).
    - There should be at least one '1' in a row.
    - White-space characters in a row are allowed (as these are ignored).

- **Output file** is in the following format:
 * It gives the following information (each in a separate line):
   - Size of the alphabet.
   - Number of patterns.
   - Total size of the patterns.
   - Size of the text.
   - d (Total number of degenerate/non-solid positions).
   - Number of clustred-clumps found.
   - Time used for calculation (after input file has been read in memory and before writing the results).
 * After that, clustred-clumps are reported delimited bt a ' ; '.
 * An clustred-clump is shown as a pair of two numbers (x, y) where x is the starting position (zero-based) of the occurrence and y is its length.


## To run the experiments: 
```
scripts/test.sh
```

It will generate three files analysis_searchA.txt, analysis_searchA.txt, and analysis_searchA.txt in the "experiments" folder after generating corresponding data files and running the tool on them. 


## External Libraries

 * For answering lcp(longest common prefix) queries, following libraries have been used:
  + [sdsl](https://github.com/simongog/sdsl-lite)
 * For testing [googletest](https://github.com/google/googletest) framework has been used.

