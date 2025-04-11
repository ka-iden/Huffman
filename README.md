# Huffman

A C++ compression and decompression program that uses Huffman Coding.  
For a final high school project, but now I'm rewriting it for fun.
>[!CAUTION]
> This project is currently undergoing a complete rewrite.  
> Nothing will work at the moment. There's no point in trying to compile.

## Building

You can build this on Windows using the included Visual Studio 2022 solution.  

## Usage

The program will only take in `.txt` and `.huff`, the extensions I arbitrarily chose :^)  
Usage is very simple and can only be used from the command line, being:

```console
Huffman.exe /path/to/input.extension path/to/output.extension
```

You can go from `.txt` to `.huff` or from `.huff` to `.txt`.

## Examples

Some examples of the program running is provided in [here](Tests/)  
They were generated with these two commands:

```console
Huffman.exe input.txt encoded.huff
Huffman.exe encoded.huff output.txt
```

## License

This project is licensed under the [MIT License].

[MIT License]: LICENSE
