# ACCSS - a css compressor written in ansi c
C++ Wrapper for node-gyp
Documentation see https://github.com/acwtools/accss

## Lib
```
    var accssnode = require('accssnode');
    var res = compressor.minify(cssString);
    OR
    var res = compressor.minify(cssString, {"restructure" : true, 'preserve-splitted' : false, 'compat' : 'none'});
```

## Standalone Usage:
```
    accssnode
        reads from stdin, prints to stdout
    accssnode <filename> [<filename>...]
        minimizes the CSS in files <filename> and outputs the result to stdout
        multiple files while be combined to one output stream
    accssnode -o <out_filename>
        reads from stdin, writes output to specified file
    accssnode -o <out_filename> <filename> [<filename>...]
        minimizes the CSS in <filename> and writes output to specified file
         multiple files while be combined to one output file
    accssnode -r
    accssnode --restructure-off
        turns structure minimization off
    accssnode -h
    accssnode --help
        shows usage information
    accssnode -v
    accssnode --version
        shows the version number
    accssnode -s
    accssnode --stats
        prints cpmpression statistic to stdout if outfile is specified
    accssnode -p
    accssnode --preserve-splitted
        keep splitted shorthand propertys

        before compression
            .mt1{ margin-top:0; } .mt2{ margin-top:100px; }.mt1{ margin-left:0; margin-bottom:0; margin-right:0; }
        without option
            .mt2{margin-top:100px}.mt1{margin:0}
        with option
            .mt1{margin-top:0}.other{margin-top:100px}.mt1{margin-left:0;margin-bottom:0;margin-right:0}
        Note: consider restructure your css to .mt1.mt2{margin-top:100px}
    accssnode -c
    accssnode --compat
        compatibility to browser versions

        If set, following property/value sets and all below are not compressed/merged to preserve progressive enhancements for older browsers.
        The default value is all, which means maximum compatibility.
        Set this option to none if you don't use progressive enhancements as this increases compression speed and ratio.

        Example:
            none
                before compression
                    .ex{ display:block; display: inline-block; }
                after compression
                    .ex{display:inline-block}
            ie7
                before compression
                    .ex{ display:block; display: inline-block; }
                after compression
                    .ex{display:block;display:inline-block}

        Possible values:
        all
            default setting
        ie7
            display: inline-block
            clip
                comma syntax
        ie8
            background-position:
                background offsets
                http://www.w3.org/TR/css3-background/#background-position
            background / background-image
                multiple backgrounds
                http://www.w3.org/TR/css3-background/#the-background-image
            css3 colors
                http://www.w3.org/TR/css3-color/
                hsl, hsla, rgba
            cursor:
                http://www.w3.org/TR/css3-ui/#cursor
            viewport units
                http://www.w3.org/TR/css3-values/#viewport-relative-lengths
                vw
                vh
            root em units
                http://www.w3.org/TR/css3-values/#font-relative-lengths
        ie9
            calc as unit value
                http://www.w3.org/TR/css3-values/#calc
            viewport units
                vm IE9 only (wrong implementation of vmin)
                vmin
            css gradients
                http://www.w3.org/TR/css3-images/#gradients
                linear-gradient
                radial-gradient
                http://www.w3.org/TR/css3-images/#repeating-gradients
                repeating-linear-gradient
                repeating-radial-gradient
        ie10
            viewport units:
                vmax
        ie11
            cursor:
                zoom-in and zoom-out
        chrome
            text-decoration
                http://www.w3.org/TR/css-text-decor-3/#line-decoration
        future
            image
                http://dev.w3.org/csswg/css-images-3/#image-notation
            image-set
                http://dev.w3.org/csswg/css-images-3/#image-set-notation
        none
            compress/merge everthing
```